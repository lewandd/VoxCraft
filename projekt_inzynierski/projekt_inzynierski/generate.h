#pragma once
#include "octree.h"

#define SAME(a, b, c, d) ((a == b) && (b == c) && (c == d))

// struktury

struct CHUNK {
public:
    Octree* o[8];
    unsigned int VAO;
    float* data;
    int size;
};

// zmienne globalne

CHUNK* chunk[CHUNKS_COUNT][CHUNKS_COUNT];

float** noise[NOISE_MAP_COUNT][NOISE_MAP_COUNT];

unsigned int texture;

// deklaracje funkcji

void recSetMinMap(float*** minMap, int x, int y, int lvl);
float*** getMinMap(float** hMap);
CHUNK* generate_chunk(int x, int y);
void setVAO(int x, int y);

float vertices[] = {
//      position          texture     side
    1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   0.0f, // left side
    1.0f, 0.0f, 0.0f,   0.0f, 0.0f,   0.0f,
    0.0f, 0.0f, 0.0f,   1.0f, 0.0f,   0.0f,
    0.0f, 0.0f, 0.0f,   1.0f, 0.0f,   0.0f,
    0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   0.0f,
    1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   0.0f,

    0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   0.0f,// right side
    1.0f, 0.0f, 1.0f,   1.0f, 0.0f,   0.0f,
    1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   0.0f,
    1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   0.0f,
    0.0f, 1.0f, 1.0f,   0.0f, 1.0f,   0.0f,
    0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   0.0f,

    0.0f, 1.0f, 1.0f,   1.0f, 1.0f,   0.0f, // front
    0.0f, 1.0f, 0.0f,   0.0f, 1.0f,   0.0f,
    0.0f, 0.0f, 0.0f,   0.0f, 0.0f,   0.0f,
    0.0f, 0.0f, 0.0f,   0.0f, 0.0f,   0.0f,
    0.0f, 0.0f, 1.0f,   1.0f, 0.0f,   0.0f,
    0.0f, 1.0f, 1.0f,   1.0f, 1.0f,   0.0f,

    1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   0.0f, // back
    1.0f, 1.0f, 0.0f,   1.0f, 1.0f,   0.0f,
    1.0f, 1.0f, 1.0f,   0.0f, 1.0f,   0.0f,
    1.0f, 1.0f, 1.0f,   0.0f, 1.0f,   0.0f,
    1.0f, 0.0f, 1.0f,   0.0f, 0.0f,   0.0f,
    1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   0.0f,

    0.0f, 0.0f, 0.0f,   0.0f, 0.0f,   1.0f, // down
    1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   1.0f,
    1.0f, 0.0f, 1.0f,   1.0f, 1.0f,   1.0f,
    1.0f, 0.0f, 1.0f,   1.0f, 1.0f,   1.0f,
    0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   1.0f,
    0.0f, 0.0f, 0.0f,   0.0f, 0.0f,   1.0f,

    1.0f, 1.0f, 1.0f,   1.0f, 0.0f,   2.0f, // up
    1.0f, 1.0f, 0.0f,   1.0f, 1.0f,   2.0f,
    0.0f, 1.0f, 0.0f,   0.0f, 1.0f,   2.0f,
    0.0f, 1.0f, 0.0f,   0.0f, 1.0f,   2.0f,
    0.0f, 1.0f, 1.0f,   0.0f, 0.0f,   2.0f,
    1.0f, 1.0f, 1.0f,   1.0f, 0.0f,   2.0f,
};

float target_vertices[] = {
-0.02f,  0.0f,  0.0f,
 0.02f,  0.0f,  0.0f,
 0.0f, -0.02f,  0.0f,
 0.0f,  0.02f,  0.0f,
};

float interface_vertices[] = {
 0.1f,  0.0f,  0.0f, 1.0f, 0.0f,
 0.0f,  0.1f,  0.0f, 0.0f, 1.0f,
 0.0f,  0.0f,  0.0f, 0.0f, 0.0f,
 0.1f,  0.0f,  0.0f, 1.0f, 0.0f,
 0.1f,  0.1f,  0.0f, 1.0f, 1.0f,
 0.0f,  0.1f,  0.0f, 0.0f, 1.0f
};

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
    
    setVAO(x, y);

    // deallocate

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < MAX_DIM_SIZE >> i; ++j)
            delete minMap[i][j];
        delete minMap[i];
    }
    delete minMap;

    return ch;
}

void setVAO(int x, int y) {
    CHUNK* ch = chunk[x][y];
    Octree** cho = ch->o;

    // instanceVBO

    vector <float> vect;

    int sum = 0;
    for (int i = 0; i < 8; ++i) {
        if (cho[i] != NULL) {
            for (int lvl = 0; lvl < MAX_LEVEL + 1; ++lvl) {
                for (int tt = 0; tt < 5; ++tt) {
                    sum += cho[i]->fullBlocks[lvl][tt].size();
                    for (int j = 0; j < cho[i]->fullBlocks[lvl][tt].size(); j++) {
                        vect.push_back(cho[i]->fullBlocks[lvl][tt][j]->x + x * CHUNK_SIZE);
                        vect.push_back(cho[i]->fullBlocks[lvl][tt][j]->y + i * CHUNK_SIZE);
                        vect.push_back(cho[i]->fullBlocks[lvl][tt][j]->z + y * CHUNK_SIZE);
                        vect.push_back(1 << (MAX_LEVEL - cho[i]->fullBlocks[lvl][tt][j]->level));
                        //printf("lvl: %d\n", (MAX_LEVEL - cho[i]->fullBlocks[lvl][tt][j]->level));
                        vect.push_back(cho[i]->fullBlocks[lvl][tt][j]->type);
                        //vect.push_back(0.0f);//cho[i]->fullBlocks[lvl][tt][j]->type);
                    }
                }
            }
        }
    }

    ch->data = &vect[0];
    ch->size = sum;

    unsigned int instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 5 * sum, &ch->data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // prep vbo

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // side attribute
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // 
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

    glEnableVertexAttribArray(3);
    
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribDivisor(3, 1);

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribDivisor(4, 1);

    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(4 * sizeof(float)));
    glVertexAttribDivisor(5, 1);

    glBindTexture(GL_TEXTURE_2D_ARRAY, texture);

    glActiveTexture(GL_TEXTURE0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    ch->VAO = VAO;
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
