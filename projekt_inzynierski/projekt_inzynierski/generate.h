#pragma once
#include "octree.h"
#include "Action.h"
#include "Change.h"

#define SAME(a, b, c, d) ((a == b) && (b == c) && (c == d))

Change changes[CHUNKS_COUNT][CHUNKS_COUNT];

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

void remData(CHUNK*, Octree*, Block*);
void addData(CHUNK*, Octree*, Block*);
void addSingleData(CHUNK*, Octree*, Block*, int);
Block* getNeighbour(CHUNK* ch, Octree* o, int bx, int by, int bz, int blvl, CHUNK** neighch, Octree** neigho, int i, int& nchx, int& nchy);
void updateChunk(CHUNK* ch, int neigh);

class CHUNK {
public:
    Octree* o[8];
    unsigned int VAO;
    unsigned int instanceVBO;
    float* data;
    int data_size;
    int x, y;
    bool set;
    float** hMap;
    CHUNK* n[4];

    CHUNK() {
        int max_size = 12000;
        this->data = new float[max_size];
        this->set = false;
        data_size = 0;

        hMap = new float* [CHUNK_SIZE];
        for (int i = 0; i < CHUNK_SIZE; ++i)
            hMap[i] = new float[CHUNK_SIZE];

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
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glVertexAttribDivisor(3, 1);

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glVertexAttribDivisor(4, 1);

        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(4 * sizeof(float)));
        glVertexAttribDivisor(5, 1);

        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));
        glVertexAttribDivisor(6, 1);

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

        data_size = 0;

        set = true;

        // generate data
        for (int i = 0; i < 8; ++i)
            o[i] = NULL;

        o[0] = new Octree(this, x, y, 0);
        o[0]->setFullBlock(0, 0, 0, 3, MAX_LEVEL - 4, false, false, false, false, false, false);

        // stone top layer

        setNoise(x, y);

        setNoise(x + 1, y);
        setNoise(x - 1, y);
        setNoise(x, y + 1);
        setNoise(x, y - 1);
        setNoise(x - 1, y - 1);
        setNoise(x - 1, y + 1);
        setNoise(x + 1, y - 1);
        setNoise(x + 1, y + 1);

        int noise_x = (x * CHUNK_SIZE) / NOISE_MAP_SIZE;
        int noise_y = (y * CHUNK_SIZE) / NOISE_MAP_SIZE;
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
                    o[1]->setFullBlock(i, (int)(hMap[i][j] * 16) + 16, j, 2, MAX_LEVEL - 1, false, false, false, false, false, false);
                }
                else {
                    o[1]->setFullBlock(i, (int)(hMap[i][j] * 16), j, 2, MAX_LEVEL, false, false, false, false, false, false);
                    o[1]->setFullBlock(i, (int)(hMap[i][j] * 16) + 1, j, 2, MAX_LEVEL, false, false, false, false, false, false);

                    o[1]->setFullBlock(i, (int)(hMap[i][j + 1] * 16), j + 1, 2, MAX_LEVEL, false, false, false, false, false, false);
                    o[1]->setFullBlock(i, (int)(hMap[i][j + 1] * 16) + 1, j + 1, 2, MAX_LEVEL, false, false, false, false, false, false);

                    o[1]->setFullBlock(i + 1, (int)(hMap[i + 1][j] * 16), j, 2, MAX_LEVEL, false, false, false, false, false, false);
                    o[1]->setFullBlock(i + 1, (int)(hMap[i + 1][j] * 16) + 1, j, 2, MAX_LEVEL, false, false, false, false, false, false);

                    o[1]->setFullBlock(i + 1, (int)(hMap[i + 1][j + 1] * 16), j + 1, 2, MAX_LEVEL, false, false, false, false, false, false);
                    o[1]->setFullBlock(i + 1, (int)(hMap[i + 1][j + 1] * 16) + 1, j + 1, 2, MAX_LEVEL, false, false, false, false, false, false);
                }
                o[1]->setFullBlock(i, (int)(hMap[i][j] * 16) + 2, j, 1, MAX_LEVEL, false, false, false, true, false, false);
                o[1]->setFullBlock(i + 1, (int)(hMap[i + 1][j] * 16) + 2, j, 1, MAX_LEVEL, false, false, false, true, false, false);
                o[1]->setFullBlock(i, (int)(hMap[i][j + 1] * 16) + 2, j + 1, 1, MAX_LEVEL, false, false, false, true, false, false);
                o[1]->setFullBlock(i + 1, (int)(hMap[i + 1][j + 1] * 16) + 2, j + 1, 1, MAX_LEVEL, false, false, false, true, false, false);
            }
        }

        bool treeMap[CHUNK_SIZE][CHUNK_SIZE];

        for (int i = 0; i < CHUNK_SIZE; ++i)
            for (int j = 0; j < CHUNK_SIZE; ++j)
                treeMap[i][j] = false;

        Perlin p(NOISE_MAP_SIZE, 64, 1);
        p.setSeed(noise_x, noise_y);
        float** treeNoise = p.getAll();

        srand(200 * x + y);
        for (int i = 0; i < CHUNK_SIZE; ++i) {
            for (int j = 0; j < CHUNK_SIZE; ++j) {
                int ind_x = (x * CHUNK_SIZE) % NOISE_MAP_SIZE + i;
                int ind_y = (y * CHUNK_SIZE) % NOISE_MAP_SIZE + j;
                if (treeNoise[ind_x][ind_y] < 0.5) {
                    // forest
                    if (rand() % 25 == 0) {
                        // add tree
                        
                        if (i >= 2 && i <= 13 && j >= 2 && j <= 13) {
                            bool haveSpace = true;
                            for (int k = -2; k <= 2; ++k)
                                for (int l = -2; l <= 2; ++l) {
                                    if (treeMap[i + k][j + l])
                                        haveSpace = false;
                                }

                            if (haveSpace) {
                                treeMap[i][j] = true;
                                int octreeId = 1;
                                
                                int height = int(hMap[i][j] * 16) + 3;
                                if (height > 15) {
                                    height %= 16;
                                    octreeId = 2;
                                    if (o[2] == NULL)
                                        o[2] = new Octree(this, x, y, 2);
                                }
                                o[octreeId]->setFullBlock(i, height, j, 4, 4, true, true, false, false, true, true);
                                
                                height = int(hMap[i][j] * 16) + 4;
                                if (height > 15) {
                                    height %= 16;
                                    octreeId = 2;
                                    if (o[2] == NULL)
                                        o[2] = new Octree(this, x, y, 2);
                                }
                                o[octreeId]->setFullBlock(i, height, j, 4, 4, true, true, false, false, true, true);
                                
                                height = int(hMap[i][j] * 16) + 5;
                                if (height > 15) {
                                    height %= 16;
                                    octreeId = 2;
                                    if (o[2] == NULL)
                                        o[2] = new Octree(this, x, y, 2);
                                }
                                o[octreeId]->setFullBlock(i, height, j, 4, 4, true, true, false, true, true, true);

                                height = int(hMap[i][j] * 16) + 6;
                                if (height > 15) {
                                    height %= 16;
                                    octreeId = 2;
                                    if (o[2] == NULL)
                                        o[2] = new Octree(this, x, y, 2);
                                }
                                o[octreeId]->setFullBlock(i, height, j-1, 5, 4, false, false, true, false, true, false);
                                o[octreeId]->setFullBlock(i, height, j, 5, 4, false, false, false, true, false, false);
                                o[octreeId]->setFullBlock(i, height, j+1, 5, 4, false, false, true, false, false, true);

                                o[octreeId]->setFullBlock(i-1, height, j-1, 5, 4, true, false, true, false, true, false);
                                o[octreeId]->setFullBlock(i-1, height, j, 5, 4, true, false, true, false, false, false);
                                o[octreeId]->setFullBlock(i-1, height, j+1, 5, 4, true, false, true, false, false, true);

                                o[octreeId]->setFullBlock(i+1, height, j-1, 5, 4, false, true, true, false, true, false);
                                o[octreeId]->setFullBlock(i+1, height, j, 5, 4, false, true, true, false, false, false);
                                o[octreeId]->setFullBlock(i+1, height, j+1, 5, 4, false, true, true, false, false, true);

                                height = int(hMap[i][j] * 16) + 7;
                                if (height > 15) {
                                    height %= 16;
                                    octreeId = 2;
                                    if (o[2] == NULL)
                                        o[2] = new Octree(this, x, y, 2);
                                }
                                o[octreeId]->setFullBlock(i, height, j - 1, 5, 4, false, false, false, false, true, true);
                                o[octreeId]->setFullBlock(i, height, j + 1, 5, 4, false, false, false, false, true, true);
                                o[octreeId]->setFullBlock(i - 1, height, j - 1, 5, 4, true, false, false, false, true, false);
                                o[octreeId]->setFullBlock(i - 1, height, j, 5, 4, true, true, true, false, false, false);
                                o[octreeId]->setFullBlock(i - 1, height, j + 1, 5, 4, true, false, false, false, false, true);
                                o[octreeId]->setFullBlock(i + 1, height, j - 1, 5, 4, false, true, false, false, true, false);
                                o[octreeId]->setFullBlock(i + 1, height, j, 5, 4, true, true, false, false, false, false);
                                o[octreeId]->setFullBlock(i + 1, height, j + 1, 5, 4, false, true, false, false, false, true);
                                
                                height = int(hMap[i][j] * 16) + 8;
                                if (height > 15) {
                                    height %= 16;
                                    octreeId = 2;
                                    if (o[2] == NULL)
                                        o[2] = new Octree(this, x, y, 2);
                                }
                                o[octreeId]->setFullBlock(i, height, j - 1, 5, 4, false, false, false, false, true, true);
                                o[octreeId]->setFullBlock(i, height, j + 1, 5, 4, false, false, false, false, true, true);
                                o[octreeId]->setFullBlock(i - 1, height, j - 1, 5, 4, true, false, false, false, true, false);
                                o[octreeId]->setFullBlock(i - 1, height, j, 5, 4, true, true, true, false, false, false);
                                o[octreeId]->setFullBlock(i - 1, height, j + 1, 5, 4, true, false, false, false, false, true);
                                o[octreeId]->setFullBlock(i + 1, height, j - 1, 5, 4, false, true, false, false, true, false);
                                o[octreeId]->setFullBlock(i + 1, height, j, 5, 4, true, true, false, false, false, false);
                                o[octreeId]->setFullBlock(i + 1, height, j + 1, 5, 4, false, true, false, false, false, true);
                                
                                height = int(hMap[i][j] * 16) + 9;
                                if (height > 15) {
                                    height %= 16;
                                    octreeId = 2;
                                    if (o[2] == NULL)
                                        o[2] = new Octree(this, x, y, 2);
                                }
                                o[octreeId]->setFullBlock(i, height, j - 1, 5, 4, false, false, false, true, true, false);
                                o[octreeId]->setFullBlock(i, height, j, 5, 4, false, false, true, true, false, false);
                                o[octreeId]->setFullBlock(i, height, j + 1, 5, 4, false, false, false, true, false, true);

                                o[octreeId]->setFullBlock(i - 1, height, j - 1, 5, 4, true, false, false, true, true, false);
                                o[octreeId]->setFullBlock(i - 1, height, j, 5, 4, true, false, false, true, false, false);
                                o[octreeId]->setFullBlock(i - 1, height, j + 1, 5, 4, true, false, false, true, false, true);

                                o[octreeId]->setFullBlock(i + 1, height, j - 1, 5, 4, false, true, false, true, true, false);
                                o[octreeId]->setFullBlock(i + 1, height, j, 5, 4, false, true, false, true, false, false);
                                o[octreeId]->setFullBlock(i + 1, height, j + 1, 5, 4, false, true, false, true, false, true);
                                
                            }
                        }
                    }
                }
            }
        }

        int exthMap[18][18];
        int extX = x;
        int extY = y;
        for (int i = -1; i < 17; ++i) {
            for (int j = -1; j < 17; ++j) {
                extX = x;
                extY = y;
                if      (i == -1) extX -= 1;
                else if (i == 16) extX += 1;
                if      (j == -1) extY -= 1;
                else if (j == 16) extY += 1;
                int noise_x = (extX * CHUNK_SIZE) / NOISE_MAP_SIZE;
                int noise_y = (extY * CHUNK_SIZE) / NOISE_MAP_SIZE;
                float val;
                if (extX < 0 || extX > CHUNKS_COUNT || extY < 0 || extY > CHUNKS_COUNT)
                    val = 100000;
                else {
                    if (noise_x < NOISE_MAP_COUNT && noise_y < NOISE_MAP_COUNT)
                        val = noise[noise_x][noise_y][(extX * CHUNK_SIZE) % NOISE_MAP_SIZE + ((i + 16) % 16)][(extY * CHUNK_SIZE) % NOISE_MAP_SIZE + ((j + 16) % 16)];
                    else
                        val = 100000;
                }
                exthMap[i + 1][j + 1] = int(val * 16.0);
            }
        }
        for (int i = 0; i < 16; ++i) {
            for (int j = 0; j < 16; ++j) {
                int h = exthMap[1 + i][1 + j];
                int h1 = exthMap[1 + i - 1][1 + j];
                int h2 = exthMap[1 + i + 1][1 + j];
                int h3 = exthMap[1 + i][1 + j - 1];
                int h4 = exthMap[1 + i][1 + j + 1];
                for (int k = h1+3; k < h+3; ++k) {
                    addSingleData(this, o[1], o[1]->getBlock(i, k, j), 0);
                }
                for (int k = h2+3; k < h + 3; ++k) {
                    addSingleData(this, o[1], o[1]->getBlock(i, k, j), 1);
                }
                for (int k = h3 + 3; k < h + 3; ++k) {
                    addSingleData(this, o[1], o[1]->getBlock(i, k, j), 4);
                }
                for (int k = h4 + 3; k < h + 3; ++k) {
                    addSingleData(this, o[1], o[1]->getBlock(i, k, j), 5);
                }
            }
        }

        applyChanges();

        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < MAX_DIM_SIZE >> i; ++j)
                delete[] minMap[i][j];
            delete[] minMap[i];
        }
        delete[] minMap;

        for (int i = 0; i < NOISE_MAP_SIZE; ++i)
            delete[] treeNoise[i];
        delete[] treeNoise;
        update();
    }

    void applyChanges() {
        Change change = changes[x][y];
        for (int i = 0; i < (int)change.action.size(); ++i) {
            Action action = change.action[i];
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
    }

    void setNoise(int chunk_x, int chunk_y) {
        int noise_x = (chunk_x * CHUNK_SIZE) / NOISE_MAP_SIZE;
        int noise_y = (chunk_y * CHUNK_SIZE) / NOISE_MAP_SIZE;

        if (((noise_x >= 0) && (noise_y >= 0)) && ((noise_x < NOISE_MAP_COUNT) && (noise_y < NOISE_MAP_COUNT))) {
            if (noise[noise_x][noise_y] == NULL) {
                Perlin p(NOISE_MAP_SIZE, FREQ1, 1);
                Perlin p2(NOISE_MAP_SIZE, FREQ1 / 2, 1);
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

                for (int i = 0; i < NOISE_MAP_SIZE; ++i)
                    delete[] p1Noise[i];
                delete[] p1Noise;
            }
        }
    }

    void clear() {
        for (int i = 0; i < 8; ++i) {
            if (o[i] != NULL) {
                o[i]->deleteAll();
                delete this->o[i];
            }
        }
    }

    void update() {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * data_size, data);
    }
};

void addSingleData(CHUNK* ch, Octree* oc, Block* b, int i) {
    b->vis[i] = true;
    b->ind[i] = ch->data_size;

    ch->data[ch->data_size] = b->x + 16 * oc->x;
    ch->data[ch->data_size + 1] = b->y + 16 * oc->o;
    ch->data[ch->data_size + 2] = b->z + 16 * oc->y;
    ch->data[ch->data_size + 3] = 1 << (MAX_LEVEL - b->level);
    ch->data[ch->data_size + 4] = b->type;
    ch->data[ch->data_size + 5] = i;
    ch->data_size = ch->data_size + 6;
}

void addData(CHUNK* ch, Octree* oc, Block* b) {
    for (int i = 0; i < 6; ++i) {
        if (b->vis[i]) {
            b->ind[i] = ch->data_size;

            ch->data[ch->data_size] = b->x + 16 * oc->x;
            ch->data[ch->data_size + 1] = b->y + 16 * oc->o;
            ch->data[ch->data_size + 2] = b->z + 16 * oc->y;
            ch->data[ch->data_size + 3] = 1 << (MAX_LEVEL - b->level);
            ch->data[ch->data_size + 4] = b->type;
            ch->data[ch->data_size + 5] = i;
            ch->data_size = ch->data_size + 6;
        }
    }
}

void remData(CHUNK* ch, Octree* oc, Block* b) {
    for (int i = 0; i < 6; ++i) {
        if (b->ind[i] != -1) {
            ch->data[b->ind[i]] = ch->data[ch->data_size - 6];
            ch->data[b->ind[i] + 1] = ch->data[ch->data_size - 5];
            ch->data[b->ind[i] + 2] = ch->data[ch->data_size - 4];
            ch->data[b->ind[i] + 3] = ch->data[ch->data_size - 3];
            ch->data[b->ind[i] + 4] = ch->data[ch->data_size - 2];
            ch->data[b->ind[i] + 5] = ch->data[ch->data_size - 1];

            int octreeID = (int)ch->data[ch->data_size - 5] / 16;
            Octree* o = ch->o[octreeID];
            int block_x = (int)ch->data[ch->data_size - 6] % 16;
            int block_y = (int)ch->data[ch->data_size - 5] % 16;
            int block_z = (int)ch->data[ch->data_size - 4] % 16;

            Block* n = o->getBlock(block_x, block_y, block_z);
            n->ind[(int)ch->data[ch->data_size - 1]] = b->ind[i];
            
            ch->data_size = ch->data_size - 6;
            b->ind[i] = -1;
        }
    }
}

void updateChunk(CHUNK* ch, int neigh) {
    ch->n[neigh]->update();
}

Block* getNeighbour(CHUNK* ch, Octree* o, int bx, int by, int bz, int blvl, CHUNK** neighch, Octree** neigho, int i, int &nchx, int &nchy) {
    CHUNK* nch = ch;
    Octree* no = o;
    Block* nb = NULL;
    int bsize = (1 << int(MAX_LEVEL - blvl));
    int x = bx, y = by, z = bz;

    if (i == 0) {
        x = (x - 1 + 16) % 16;
        if (x == 15) {
            nch = ch->n[1];
            if (nch != NULL) {
                no = nch->o[o->o];
            }
        }
    }
    if (i == 1) {
        x = (x + bsize + 16) % 16;
        if (x == 0) {
            nch = ch->n[0];
            if (nch != NULL)
                no = nch->o[o->o];
        }
    }
    if (i == 2) {
        y = (y - 1 + 16) % 16;
        nch = ch;
        if (y == 15) {
            if (o->o > 0 && ch->o[o->o - 1] != NULL)
                no = ch->o[o->o - 1];
            else {
                no = NULL;
            }
        }
    }
    if (i == 3) {
        y = (y + bsize + 16) % 16;
        if (y == 0) {
            nch = ch;
            if (o->o < 7 && ch->o[o->o + 1] != NULL) {
                no = ch->o[o->o + 1];
            }
            else {
                no = NULL;
            }
        }
    }
    if (i == 4) {
        z = (z - 1 + 16) % 16;
        if (z == 15) {
            nch = ch->n[3];
            if (nch != NULL)
                no = nch->o[o->o];
        }
    }
    if (i == 5) {
        z = (z + bsize + 16) % 16;
        if (z == 0) {         
            nch = ch->n[2];
            if (nch != NULL) {
                no = nch->o[o->o];
            }
        }
    }

    *neighch = NULL;
    *neigho = NULL;

    if (nch != NULL && no != NULL) {
        *neighch = nch;
        *neigho = no;
        nb = no->getBlock(x, y, z);
        nchx = nch->x;
        nchy = nch->y;
    }
    else {
        nb = NULL;
        nchx = ch->x;
        nchy = ch->y;
    }

    return nb;
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
