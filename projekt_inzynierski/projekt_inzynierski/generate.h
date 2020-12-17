#pragma once
#include "octree.h"

// struktury

struct CHUNK {
public:
    Octree* o[8];
};

// zmienne globalne

CHUNK* chunk[CHUNKS_COUNT][CHUNKS_COUNT];

float** noise[NOISE_MAP_COUNT][NOISE_MAP_COUNT];

// funkcje

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
        Perlin p2(NOISE_MAP_SIZE, FREQ1, 1);
        p2.setSeed(noise_x, noise_y);
        noise[noise_x][noise_y] = p2.getAll();
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

    Perlin p(hMap);
    p.setMinMaxMap();
    float*** minMap = p.getMinMap();
    float*** maxMap = p.getMaxMap();
    cho[1] = new Octree();
    cho[1]->addMinMap(minMap, 4, 0, 0, 0, 3, 0);

    // dirt top layer
    for (int i = 0; i < MAX_DIM_SIZE / 2; ++i) {
        for (int j = 0; j < MAX_DIM_SIZE / 2; ++j) {
            if (((int)(16 * minMap[1][i][j]) == (int)(16 * maxMap[1][i][j])) && ((int)(16 * minMap[1][i][j]) % 2 == 0)) {
                cho[1]->setFullBlock(i * 2, (int)(maxMap[1][i][j] * 16) + 16, j * 2, 2, MAX_LEVEL - 1);
            }
            else {
                cho[1]->setFullBlock(i * 2, (int)(hMap[2 * i][2 * j] * 16), j * 2, 2, MAX_LEVEL);
                cho[1]->setFullBlock(i * 2, (int)(hMap[2 * i][2 * j] * 16) + 1, j * 2, 2, MAX_LEVEL);

                cho[1]->setFullBlock(i * 2, (int)(hMap[2 * i][2 * j + 1] * 16), j * 2 + 1, 2, MAX_LEVEL);
                cho[1]->setFullBlock(i * 2, (int)(hMap[2 * i][2 * j + 1] * 16) + 1, j * 2 + 1, 2, MAX_LEVEL);

                cho[1]->setFullBlock(i * 2 + 1, (int)(hMap[2 * i + 1][2 * j] * 16), j * 2, 2, MAX_LEVEL);
                cho[1]->setFullBlock(i * 2 + 1, (int)(hMap[2 * i + 1][2 * j] * 16) + 1, j * 2, 2, MAX_LEVEL);

                cho[1]->setFullBlock(i * 2 + 1, (int)(hMap[2 * i + 1][2 * j + 1] * 16), j * 2 + 1, 2, MAX_LEVEL);
                cho[1]->setFullBlock(i * 2 + 1, (int)(hMap[2 * i + 1][2 * j + 1] * 16) + 1, j * 2 + 1, 2, MAX_LEVEL);
            }
            cho[1]->setFullBlock(i * 2, (int)(hMap[2 * i][2 * j] * 16) + 2, j * 2, 1, MAX_LEVEL);
            cho[1]->setFullBlock(i * 2 + 1, (int)(hMap[2 * i + 1][2 * j] * 16) + 2, j * 2, 1, MAX_LEVEL);
            cho[1]->setFullBlock(i * 2, (int)(hMap[2 * i][2 * j + 1] * 16) + 2, j * 2 + 1, 1, MAX_LEVEL);
            cho[1]->setFullBlock(i * 2 + 1, (int)(hMap[2 * i + 1][2 * j + 1] * 16) + 2, j * 2 + 1, 1, MAX_LEVEL);
        }
    }

    return ch;
}