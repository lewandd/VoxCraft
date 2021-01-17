#pragma once
#include "octree.h"

#define SAME(a, b, c, d) ((a == b) && (b == c) && (c == d))

// struktury

struct ACTION {
    int mode;
    int octree;
    int x;
    int y;
    int z;
    int type;

    ACTION(int mode_, int octree_, int x_, int y_, int z_, int type_) {
        mode = mode_;
        octree = octree_;
        x = x_;
        y = y_;
        z = z_;
        type = type_;
    }
};

struct CHANGE {
    vector<ACTION> action;
};

CHANGE changes[CHUNKS_COUNT][CHUNKS_COUNT];

float** noise[NOISE_MAP_COUNT][NOISE_MAP_COUNT];

unsigned int texture;

// deklaracje funkcji

void recSetMinMap(float*** minMap, int x, int y, int lvl);
float*** getMinMap(float** hMap);

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

class CHUNK {
public:
    Octree* o[8];
    unsigned int VAO;
    unsigned int instanceVBO;
    float* data;
    int* data_size;
    int x, y;
    bool set;

    CHUNK() {
        int max_size = 10000;// 5 * 16 * 16 * 16 * 8 * 0.5;
        this->data = new float[max_size];
        this->set = false;
        this->data_size = new int;
        *(this->data_size) = 0;

        // instanceVBO

        glGenBuffers(1, &this->instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, this->instanceVBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * max_size, this->data, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // prep vbo

        unsigned int VBO;
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindVertexArray(this->VAO);

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
        glBindBuffer(GL_ARRAY_BUFFER, this->instanceVBO);

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

    void setNew(int x_, int y_) {
        // delete old chunk data
        clear();

        this->x = x_;
        this->y = y_;

        *(data_size) = 0;

        set = true;

        // generate data
        for (int i = 0; i < 8; ++i)
            o[i] = NULL;

        o[0] = new Octree(this, x, y, 0);
        o[0]->setFullBlock(0, 0, 0, 3, MAX_LEVEL - 4);

        // stone top layer

        int noise_x = (x * CHUNK_SIZE) / NOISE_MAP_SIZE;
        int noise_y = (y * CHUNK_SIZE) / NOISE_MAP_SIZE;
        if (noise[noise_x][noise_y] == NULL) {
            Perlin p(NOISE_MAP_SIZE, FREQ1, 1);
            Perlin p2(NOISE_MAP_SIZE, FREQ1/2, 1);
            p.setSeed(noise_x, noise_y);
            p2.setSeed(noise_x, noise_y);
            float** p1Noise = p.getAll();
            float** p2Noise = p2.getAll();
            for (int i = 0; i < NOISE_MAP_SIZE; ++i) {
                for (int j = 0; j < NOISE_MAP_SIZE; ++j) {
                    p2Noise[i][j] = (p1Noise[i][j] + p2Noise[i][j]) / 2.0;
                }
            }
            noise[noise_x][noise_y] = p2Noise;
            //noise[noise_x][noise_y] = p.getAll();
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
        o[1] = new Octree(this, x, y, 1);
        o[1]->addMinMap(minMap, 4, 0, 0, 0, 3, 0);

        // dirt top layer
        for (int i = 0; i < MAX_DIM_SIZE; i += 2) {
            for (int j = 0; j < MAX_DIM_SIZE; j += 2) {
                if (SAME((int)(16 * hMap[i][j]), (int)(16 * hMap[i][j + 1]), (int)(16 * hMap[i + 1][j]), (int)(16 * hMap[i + 1][j + 1])) && ((int)(16 * hMap[i][j]) % 2 == 0)) {
                    o[1]->setFullBlock(i, (int)(hMap[i][j] * 16) + 16, j, 2, MAX_LEVEL - 1);
                }
                else {
                    o[1]->setFullBlock(i, (int)(hMap[i][j] * 16), j, 2, MAX_LEVEL);
                    o[1]->setFullBlock(i, (int)(hMap[i][j] * 16) + 1, j, 2, MAX_LEVEL);

                    o[1]->setFullBlock(i, (int)(hMap[i][j + 1] * 16), j + 1, 2, MAX_LEVEL);
                    o[1]->setFullBlock(i, (int)(hMap[i][j + 1] * 16) + 1, j + 1, 2, MAX_LEVEL);

                    o[1]->setFullBlock(i + 1, (int)(hMap[i + 1][j] * 16), j, 2, MAX_LEVEL);
                    o[1]->setFullBlock(i + 1, (int)(hMap[i + 1][j] * 16) + 1, j, 2, MAX_LEVEL);

                    o[1]->setFullBlock(i + 1, (int)(hMap[i + 1][j + 1] * 16), j + 1, 2, MAX_LEVEL);
                    o[1]->setFullBlock(i + 1, (int)(hMap[i + 1][j + 1] * 16) + 1, j + 1, 2, MAX_LEVEL);
                }
                o[1]->setFullBlock(i, (int)(hMap[i][j] * 16) + 2, j, 1, MAX_LEVEL);
                o[1]->setFullBlock(i + 1, (int)(hMap[i + 1][j] * 16) + 2, j, 1, MAX_LEVEL);
                o[1]->setFullBlock(i, (int)(hMap[i][j + 1] * 16) + 2, j + 1, 1, MAX_LEVEL);
                o[1]->setFullBlock(i + 1, (int)(hMap[i + 1][j + 1] * 16) + 2, j + 1, 1, MAX_LEVEL);
            }
        }

        CHANGE change = changes[x][y];
        for (int i = 0; i < (int)change.action.size(); ++i) {
            ACTION action = change.action[i];
            if (action.mode == 0) {
                // usuwanie
                o[action.octree]->remove(action.x, action.y, action.z);
            }
            else {
                // dodawanie
                if (o[action.octree] == NULL) {
                    o[action.octree] = new Octree(this, this->x, this->y, action.octree);
                }
                o[action.octree]->add(action.x, action.y, action.z, action.type);
            }
        }

        delete[] minMap;

        update();
    }

    void clear() {
        if (set) {
            for (int i = 0; i < 8; ++i) {
                if (o[i] != NULL) {
                    o[i]->deleteAll();
                    delete this->o[i];
                }
            }
        }
    }

    void update() {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * (*data_size), data);
    }
};

void addData(CHUNK* ch, Octree* oc, Block* b) {
    b->ind = *(ch->data_size);

    ch->data[*(ch->data_size)] = b->x + 16 * oc->x;
    ch->data[*(ch->data_size) + 1] = b->y + 16 * oc->o;
    ch->data[*(ch->data_size) + 2] = b->z + 16 * oc->y;
    ch->data[*(ch->data_size) + 3] = 1 << (MAX_LEVEL - b->level);
    ch->data[*(ch->data_size) + 4] = b->type;
    *(ch->data_size) = *(ch->data_size) + 5;
}

void remData(CHUNK* ch, Octree* oc, Block* b) {

    ch->data[b->ind] = ch->data[*(ch->data_size) - 5];
    ch->data[b->ind + 1] = ch->data[*(ch->data_size) - 4];
    ch->data[b->ind + 2] = ch->data[*(ch->data_size) - 3];
    ch->data[b->ind + 3] = ch->data[*(ch->data_size) - 2];
    ch->data[b->ind + 4] = ch->data[*(ch->data_size) - 1];

    int octreeID = (int)ch->data[*(ch->data_size) - 4] / 16;
    Octree *o = ch->o[octreeID];
    int block_x = (int)ch->data[*(ch->data_size) - 5] % 16;
    int block_y = (int)ch->data[*(ch->data_size) - 4] % 16;
    int block_z = (int)ch->data[*(ch->data_size) - 3] % 16;

    Block* n = o->getBlock(block_x, block_y, block_z);
    n->ind = b->ind;

    *(ch->data_size) = *(ch->data_size) - 5;
    b->ind = -1;
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
