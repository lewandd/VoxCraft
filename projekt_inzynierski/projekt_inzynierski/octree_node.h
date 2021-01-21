#pragma once
#include "constans.h"
#include <vector>
using namespace std;

class Block {
public:

    Block* ch[8];
    int x, y, z;
    int type;
    int level;
    int size;
    int max_size;
    int ind[6];
    bool vis[6];

    Block(int level, int type, int x, int y, int z);
    bool isFull();
    int isEmpty();
    int getLevel();
    int getSize();
    int getMaxSize();
    int getType();
    bool sameType();
    bool mergeUpdateType();
    void setFull(int _type);
    Block* setChild(int ind);
    void deleteChild(int x, int y, int z);
    void deleteChild(int ind);
    bool isSingleBlock();
    void show();
    Block* getChild(int x, int y, int z);
    Block* getChild(int ind);
    int getChildInd(int x, int y, int z);
    vector<int> getChildPosition(int c);
};
