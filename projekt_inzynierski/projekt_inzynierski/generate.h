#pragma once
#include "octree.h"

#define SAME(a, b, c, d) ((a == b) && (b == c) && (c == d))

// struktury

struct CHUNK {
public:
    Octree* o[8];
    unsigned int VAO;
    unsigned int instanceVBO;
    float* data;
    int* data_size;
    int x, y;
    bool set;
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
void delete_chunk(CHUNK*);
void update_chunk(CHUNK*);

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

float select_vertices[] = {
    0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
};

// implementacje funkcji

void constructor_chunk(CHUNK* ch) {
    int max_size = 10000;// 5 * 16 * 16 * 16 * 8 * 0.5;
    ch->data = new float[max_size];
    ch->set = false;
    ch->data_size = new int;
    *(ch->data_size) = 0;

    // instanceVBO

    glGenBuffers(1, &ch->instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, ch->instanceVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * max_size, ch->data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // prep vbo

    unsigned int VBO;
    glGenVertexArrays(1, &ch->VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(ch->VAO);

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
    glBindBuffer(GL_ARRAY_BUFFER, ch->instanceVBO);

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
}

void change_chunk(CHUNK* ch, int x, int y) {
    // delete old chunk data
    delete_chunk(ch);
    
    *(ch->data_size) = 0;

    ch->x = x;
    ch->y = y;
    ch->set = true;

    // generate data
    for (int i = 0; i < 8; ++i)
        ch->o[i] = NULL;

    Octree** cho = ch->o;
    cho[0] = new Octree(ch->data, ch->data_size, ch->x, ch->y, 0);
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
    cho[1] = new Octree(ch->data, ch->data_size, ch->x, ch->y, 1);
    cho[1]->addMinMap(minMap, 4, 0, 0, 0, 3, 0);

    // dirt top layer
    for (int i = 0; i < MAX_DIM_SIZE; i += 2) {
        for (int j = 0; j < MAX_DIM_SIZE; j += 2) {
            if (SAME((int)(16 * hMap[i][j]), (int)(16 * hMap[i][j + 1]), (int)(16 * hMap[i + 1][j]), (int)(16 * hMap[i + 1][j + 1])) && ((int)(16 * hMap[i][j]) % 2 == 0)) {
                cho[1]->setFullBlock(i, (int)(hMap[i][j] * 16) + 16, j, 2, MAX_LEVEL - 1);
            }
            else {
                cho[1]->setFullBlock(i, (int)(hMap[i][j] * 16), j, 2, MAX_LEVEL);
                cho[1]->setFullBlock(i, (int)(hMap[i][j] * 16) + 1, j, 2, MAX_LEVEL);

                cho[1]->setFullBlock(i, (int)(hMap[i][j + 1] * 16), j + 1, 2, MAX_LEVEL);
                cho[1]->setFullBlock(i, (int)(hMap[i][j + 1] * 16) + 1, j + 1, 2, MAX_LEVEL);

                cho[1]->setFullBlock(i + 1, (int)(hMap[i + 1][j] * 16), j, 2, MAX_LEVEL);
                cho[1]->setFullBlock(i + 1, (int)(hMap[i + 1][j] * 16) + 1, j, 2, MAX_LEVEL);

                cho[1]->setFullBlock(i + 1, (int)(hMap[i + 1][j + 1] * 16), j + 1, 2, MAX_LEVEL);
                cho[1]->setFullBlock(i + 1, (int)(hMap[i + 1][j + 1] * 16) + 1, j + 1, 2, MAX_LEVEL);
            }
            cho[1]->setFullBlock(i, (int)(hMap[i][j] * 16) + 2, j, 1, MAX_LEVEL);
            cho[1]->setFullBlock(i + 1, (int)(hMap[i + 1][j] * 16) + 2, j, 1, MAX_LEVEL);
            cho[1]->setFullBlock(i, (int)(hMap[i][j + 1] * 16) + 2, j + 1, 1, MAX_LEVEL);
            cho[1]->setFullBlock(i + 1, (int)(hMap[i + 1][j + 1] * 16) + 2, j + 1, 1, MAX_LEVEL);
        }
    }
    
    delete[] minMap;

    update_chunk(ch);
}

void update_chunk(CHUNK* ch) {
    glBindVertexArray(ch->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, ch->instanceVBO);
    
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * (*(ch->data_size)), ch->data);
    //cout << ch->size << " \n";
}


void delete_chunk(CHUNK* ch) {
    if (ch->set) {
        for (int i = 0; i < 8; ++i) {
            if (ch->o[i] != NULL) {
                //printf("delete CHUNK data\n");
                ch->o[i]->deleteAll(); 
                delete ch->o[i];
            }
        }
    }
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
