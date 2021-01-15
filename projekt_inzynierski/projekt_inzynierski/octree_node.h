#pragma once
#include "constans.h"
using namespace std;


struct TRI_ARGS {
    float x, y, z;
    float scale;
    int type;

    TRI_ARGS(float x, float y, float z, float scale, int type) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->scale = scale;
        this->type = type;
    }

    void show() {
        printf("(%.0f, %.0f, %.0f), scale: %.0f, type: %d\n", x, y, z, scale, type);
    }
};

class Block {
public:

    Block* ch[8];
    int x, y, z;
    int type;
    int level;
    int size;
    int max_size;
    int ind = -1;

    Block(int level, int type, int x, int y, int z) {
        for (int i = 0; i < 8; ++i)
            ch[i] = NULL;
        this->x = (x >> (MAX_LEVEL - level)) << (MAX_LEVEL - level);
        this->y = (y >> (MAX_LEVEL - level)) << (MAX_LEVEL - level);
        this->z = (z >> (MAX_LEVEL - level)) << (MAX_LEVEL - level);
        this->type = type;
        this->level = level;
        this->size = 0;
        this->max_size = MAX_SIZE >> (level * 3);
    }

    bool isFull() {
        return size == max_size;
    }

    int isEmpty() {
        return size == 0;
    }

    int getLevel() {
        return level;
    }

    int getSize() {
        return size;
    }

    int getMaxSize() {
        return this->max_size;
    }

    int getType() {
        return type;
    }

    bool sameType() {
        return type;
    }

    TRI_ARGS intoTRI_ARGS() {
        return TRI_ARGS((float)x, (float)y, (float)z, (float) (1 << (MAX_LEVEL - level)), type);
    }

    bool mergeUpdateType() {
        if (type != 0)
            return true;
        if (size < max_size)
            return false;

        int new_type = ch[0]->type;
        if (new_type == 0)
            return false;

        for (int i = 1; i < 8; ++i)
            if (ch[i]->type != new_type)
                return false;
        type = new_type;
        return true;
    }

    void setFull(int _type) {
        size = max_size;
        this->type = _type;
    }

    Block* setChild(int ind) {
        vector <int> pos = getChildPosition(ind);
        ch[ind] = new Block(level + 1, 0, pos[0], pos[1], pos[2]);
        return ch[ind];
    }

    void deleteChild(int x, int y, int z) {
        delete ch[getChildInd(x, y, z)];
        ch[getChildInd(x, y, z)] = NULL;
    }

    void deleteChild(int ind) {
        delete ch[ind];
        ch[ind] = NULL;
    }

    bool isSingleBlock() {
        return level == MAX_LEVEL;
    }

    void show() {
        if (isSingleBlock())
            printf(" Single Block ");
        else if (sameType())
            printf("   Full Block ");
        else
            printf("        Block ");
        printf("   %d  | ", level);
        printf("(%3d, %3d, %3d) | ", x, y, z);
        printf("   %3d   | ", DIM_SIZE[level]);
        printf(" (%3d, %3d)", x, x + DIM_SIZE[level] - 1);
        printf(" (%3d, %3d)", y, y + DIM_SIZE[level] - 1);
        printf(" (%3d, %3d) |", z, z + DIM_SIZE[level] - 1);
        printf(" %8d  | ", size);
        printf(" %8d  | ", max_size);
        printf(" %3d  |\n", type);
    }

    Block* getChild(int x, int y, int z) {
        return ch[getChildInd(x, y, z)];
    }

    Block* getChild(int ind) {
        return ch[ind];
    }

    int getChildInd(int x, int y, int z) {
        int offset = 1 << (MAX_LEVEL - level - 1);
        return ((x & offset) << 2 | ((y & offset) << 1) | ((z & offset))) >> (MAX_LEVEL - level - 1);
    }

    vector<int> getChildPosition(int c) {
        vector<int> pos;
        pos.push_back(x + DIM_SIZE[level + 1] * ((c >> 2) & 1));
        pos.push_back(y + DIM_SIZE[level + 1] * ((c >> 1) & 1));
        pos.push_back(z + DIM_SIZE[level + 1] * ((c) & 1));
        return pos;
    }

};
