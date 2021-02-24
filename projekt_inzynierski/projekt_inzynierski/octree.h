#pragma once
#include "octree_node.h"
#include <vector>

class Octree;
class CHUNK;

class Octree {
    public:
        Block* root;
        CHUNK* chunk;
        int x, y, o;

        Octree(CHUNK* chunk_, int x_, int y_, int o_);
        
        // basic operations

        void add(int x, int y, int z, int type);
        int remove(int x, int y, int z);
        Block* getBlock(int x, int y, int z);

        // generate

        void setFullBlock(int x, int y, int z, int type, int target_level, bool v0, bool v1, bool v2, bool v3, bool v4, bool v5);
        void addMinMap(float*** minMap, int lvl, int x, int y, int z, int type, int z0);

        // clear
        void deleteAll();
        void deleteRec(Block* tmp);

        // debug
        void show(Block* n);
        void show(vector<Block*> v);
        void show_all();

    private:
        void addToFullBlocks(Block* n);
        void deleteFromFullBlocks(Block* n, bool checkNeigh);
        void merge(int x, int y, int z);
        vector <Block*> getBlocks(int x, int y, int z);
};
