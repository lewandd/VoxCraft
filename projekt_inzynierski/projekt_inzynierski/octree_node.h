#pragma once
using namespace std;

const int MAX_LEVEL = 8;
const int MAX_DIM_SIZE = 1 << MAX_LEVEL;
const int MAX_SIZE = MAX_DIM_SIZE * MAX_DIM_SIZE * MAX_DIM_SIZE;
const int MAX_CHILD = 8;

const int DIM_SIZE[] = {
    256, // level 0
    128, // level 1
    64,  // level 2
    32,  // level 3
    16,  // level 4
    8,   // level 5
    4,   // level 6
    2,   // level 7
    1    // level 8
};

struct TRI_ARGS {
    float x, y, z;
    float scale;
    float type;

    TRI_ARGS(float x, float y, float z, float scale, float type) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->scale = scale;
        this->type = type;
    }

    void show() {
        printf("(%.0f, %.0f, %.0f), scale: %.0f, type: %.0f\n", x, y, z, scale, type);
    }
};

class OctreeNode {
public:

    OctreeNode* ch[8];
    int x, y, z;
    int type;
    int level;
    int size;
    int max_size;
    int ind;
    bool selected = false;

    OctreeNode(int level, int type, int x, int y, int z) {
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

    bool isSelected() {
        return selected;
    }

    void setSeleted() {
        selected = true;
    }

    void unsetSeleted() {
        selected = false;
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
        return TRI_ARGS(x, y, z, 1 << (8 - level), type);
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

    OctreeNode* setChild(int ind) {
        vector <int> pos = getChildPosition(ind);
        ch[ind] = new OctreeNode(level + 1, 0, pos[0], pos[1], pos[2]);
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

    OctreeNode* getChild(int x, int y, int z) {
        return ch[getChildInd(x, y, z)];
    }

    OctreeNode* getChild(int ind) {
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
