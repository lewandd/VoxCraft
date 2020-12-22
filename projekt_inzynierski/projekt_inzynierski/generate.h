#pragma once
#include "octree.h"

#define SAME(a, b, c, d) ((a == b) && (b == c) && (c == d))

// struktury

struct CHUNK {
public:
    Octree* o[8];
};

// zmienne globalne

CHUNK* chunk[CHUNKS_COUNT][CHUNKS_COUNT];

float** noise[NOISE_MAP_COUNT][NOISE_MAP_COUNT];

// deklaracje funkcji

void recSetMinMap(float*** minMap, int x, int y, int lvl);
float*** getMinMap(float** hMap);
CHUNK* generate_chunk(int x, int y);

// implementacje funkcji

CHUNK* generate_chunk(int x, int y) {
    chunk[x][y] = new CHUNK();
    CHUNK* ch = chunk[x][y];
    Octree** cho = ch->o;

    // stone layer

    cho[0] = new Octree();
    cho[0]->setFullBlock(0, 0, 0, 3, MAX_LEVEL - 4);

    // stone top layer

    int noise_x = (x * CHUNK_SIZE) / NOISE_MAP_SIZE;
    int noise_y = (y * CHUNK_SIZE) / NOISE_MAP_SIZE;
    if (noise[noise_x][noise_y] == NULL) {
        Perlin p(NOISE_MAP_SIZE, FREQ1, 1);
        p.setSeed(noise_x, noise_y);
        noise[noise_x][noise_y] = p.getAll();
    }

    float** hMap;
    hMap = new float* [CHUNK_SIZE];
    for (int i = 0; i < CHUNK_SIZE; ++i)
        hMap[i] = new float[CHUNK_SIZE];

    for (int i = 0; i < CHUNK_SIZE; ++i) {
        for (int j = 0; j < CHUNK_SIZE; ++j) {
            hMap[i][j] = noise[noise_x][noise_y][(x * CHUNK_SIZE) % NOISE_MAP_SIZE + i][(y * CHUNK_SIZE) % NOISE_MAP_SIZE + j];
        }
    }
    float*** minMap = getMinMap(hMap);
    cho[1] = new Octree();
    cho[1]->addMinMap(minMap, 4, 0, 0, 0, 3, 0);

    // dirt top layer
    for (int i = 0; i < MAX_DIM_SIZE; i += 2) {
        for (int j = 0; j < MAX_DIM_SIZE; j += 2) {
            if (SAME((int)(16 * hMap[i][j]), (int)(16 * hMap[i][j+1]), (int)(16 * hMap[i+1][j]), (int)(16 * hMap[i+1][j+1])) && ((int)(16 * hMap[i][j]) % 2 == 0)) {
                cho[1]->setFullBlock(i, (int)(hMap[i][j] * 16) + 16, j, 2, MAX_LEVEL - 1);
            }
            else {
                cho[1]->setFullBlock(i, (int)(hMap[i][j] * 16), j, 2, MAX_LEVEL);
                cho[1]->setFullBlock(i, (int)(hMap[i][j] * 16) + 1, j, 2, MAX_LEVEL);

                cho[1]->setFullBlock(i, (int)(hMap[i][j+1] * 16), j + 1, 2, MAX_LEVEL);
                cho[1]->setFullBlock(i, (int)(hMap[i][j+1] * 16) + 1, j + 1, 2, MAX_LEVEL);

                cho[1]->setFullBlock(i+1, (int)(hMap[i+1][j] * 16), j, 2, MAX_LEVEL);
                cho[1]->setFullBlock(i+1, (int)(hMap[i+1][j] * 16) + 1, j, 2, MAX_LEVEL);

                cho[1]->setFullBlock(i+1, (int)(hMap[i+1][j+1] * 16), j + 1, 2, MAX_LEVEL);
                cho[1]->setFullBlock(i+1, (int)(hMap[i+1][j+1] * 16) + 1, j + 1, 2, MAX_LEVEL);
            }
            cho[1]->setFullBlock(i, (int)(hMap[i][j] * 16) + 2, j, 1, MAX_LEVEL);
            cho[1]->setFullBlock(i+1, (int)(hMap[i+1][j] * 16) + 2, j, 1, MAX_LEVEL);
            cho[1]->setFullBlock(i, (int)(hMap[i][j+1] * 16) + 2, j + 1, 1, MAX_LEVEL);
            cho[1]->setFullBlock(i+1, (int)(hMap[i+1][j+1] * 16) + 2, j + 1, 1, MAX_LEVEL);
        }
    }

    // prep vbo

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    vector <int> vect;

    int sum = 0;
    for (int i = 0; i < 8; ++i) {
        if (cho[i] != NULL) {
            for (int lvl = 0; lvl < MAX_LEVEL + 1; ++lvl) {
                for (int tt = 0; tt < 5; ++tt) {
                    sum += cho[i]->fullBlocks[lvl][tt].size();
                    for (int j = 0; j < cho[i]->fullBlocks[lvl][tt].size(); j++) {
                        vect.push_back(cho[i]->fullBlocks[lvl][tt][j]->x);
                    }
                }
            }
        }
    }
    printf("%d\n", sum);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*sum, &vect[0], GL_STATIC_DRAW);
    
    // deallocate

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < MAX_DIM_SIZE >> i; ++j)
            delete minMap[i][j];
        delete minMap[i];
    }
    delete minMap;

    return ch;
}

float*** getMinMap(float** hMap) {
    // allocate
    float*** minMap = new float** [5];
    for (int i = 0; i < 5; ++i) {
        minMap[i] = new float* [MAX_DIM_SIZE >> i];
        for (int j = 0; j < MAX_DIM_SIZE >> i; ++j)
            minMap[i][j] = new float[MAX_DIM_SIZE >> i];
    }

    // set lowest level
    for (int i = 0; i < CHUNK_SIZE; ++i)
        for (int j = 0; j < CHUNK_SIZE; ++j)
            minMap[0][i][j] = hMap[i][j];

    recSetMinMap(minMap, 0, 0, 4);

    return minMap;
}

void recSetMinMap(float*** minMap, int x, int y, int lvl) {
    if (lvl > 0) {
        recSetMinMap(minMap, x * 2, y * 2, lvl - 1);
        recSetMinMap(minMap, x * 2, y * 2 + 1, lvl - 1);
        recSetMinMap(minMap, x * 2 + 1, y * 2, lvl - 1);
        recSetMinMap(minMap, x * 2 + 1, y * 2 + 1, lvl - 1);
        minMap[lvl][x][y] = min(min(minMap[lvl - 1][x * 2][y * 2],
            minMap[lvl - 1][x * 2][y * 2 + 1]),
            min(minMap[lvl - 1][x * 2 + 1][y * 2],
                minMap[lvl - 1][x * 2 + 1][y * 2 + 1]));
    }
}
