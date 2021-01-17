#pragma once
#include "octree_node.h"
#include <vector>

class Octree;
class CHUNK;
void remData(CHUNK*, Octree*, Block*);
void addData(CHUNK*, Octree*, Block*);

class Octree {

public:
    Block* root = new Block(0, 0, 0, 0, 0);
    CHUNK* chunk;
    int x, y, o;

    Octree(CHUNK *chunk_, int x_, int y_, int o_) {
        this->x = x_;
        this->y = y_;
        this->o = o_;
        this->chunk = chunk_;
    }

    void addMinMap(float ***minMap, int lvl, int x, int y, int z, int type, int z0) {
        if (lvl >= 0) {
            if (minMap[lvl][x][y]*16 >= (1 << lvl)+z) {
                setFullBlock(x<<lvl, z0+z, y << lvl, type, MAX_LEVEL-lvl);
                addMinMap(minMap, lvl, x, y, z + (1<<lvl), type, z0);
            }
            else {
                addMinMap(minMap, lvl - 1, (x << 1), (y << 1), z, type, z0);
                addMinMap(minMap, lvl - 1, (x << 1), (y << 1)+1, z, type, z0);
                addMinMap(minMap, lvl - 1, (x << 1)+1, (y << 1), z, type, z0);
                addMinMap(minMap, lvl - 1, (x << 1)+1, (y << 1)+1, z, type, z0);
            }
        }
    }

    void add(int x, int y, int z, int type) {
        if (getBlock(x, y, z)->isFull()) {
            printf("WARNING (add): block already exist (%d, %d, %d)\n", x, y, z);
            return;
        }
        if (type == 0) {
            printf("WARNING (add): block with type = 0 (%d, %d, %d)\n", x, y, z);
            return;
        }

        Block* target = root;
        Block* tmp = root;
        int level = 0;
        int ind;

        while (tmp != NULL) {
            target = tmp;
            target->size++;
            ind = target->getChildInd(x, y, z);
            tmp = target->getChild(ind);
            level++;
        }

        while (level <= MAX_LEVEL) {
            target = target->setChild(ind);
            target->size++;

            ind = target->getChildInd(x, y, z);
            level++;
        }

        target->type = type;

        merge(x, y, z);
    }

    void setFullBlock(int x, int y, int z, int type, int target_level) {
        if (target_level == 0) {
            root->setFull(type);
            addToFullBlocks(root);
            return;
        }
        
        if (getBlock(x, y, z)->isFull()) {
            printf("WARNING (add): block already exist (%d, %d, %d)\n", x, y, z);
            return;
        }
        if (type == 0) {
            printf("WARNING (add): block with type = 0 (%d, %d, %d)\n", x, y, z);
            return;
        }

        Block* target = root;
        Block* tmp = root;
        int level = 0;
        int ind;

        while (tmp != NULL) {
            target = tmp;
            target->size += 1 << ((MAX_LEVEL - target_level) * 3);
            ind = target->getChildInd(x, y, z);
            tmp = target->getChild(ind);
            level++;
        }

        if (level > target_level) {
            printf("WARNING (add): full block exist (%d, %d, %d)\n", x, y, z);
            return;
        }

        while (level <= target_level) {
            target = target->setChild(ind);
            target->size += 1 << ((MAX_LEVEL - target_level) * 3);

            ind = target->getChildInd(x, y, z);
            level++;
        }

        target->setFull(type);
        addToFullBlocks(target);

    }

    int remove(int x, int y, int z) {
        int last_type = -1;
        vector <Block*> blocks = getBlocks(x, y, z);
        if (!blocks.back()->isFull()) {
            printf("WARNING (remove): block does not exist (%d, %d, %d)\n", x, y, z);
            return -1;
        }
        last_type = blocks.back()->type;

        // update existed
        for (int lvl = 0; lvl < (int)blocks.size(); ++lvl) {
            // delete from full blocks
            if (blocks[lvl]->sameType())
                deleteFromFullBlocks(blocks[lvl]);

            blocks[lvl]->size--;
        }

        // split full blocks (not single)
        int type = blocks.back()->getType();
        for (int lvl = blocks.size(); lvl <= MAX_LEVEL; ++lvl) {
            for (int i = 0; i < MAX_CHILD; ++i) {
                // add
                blocks[lvl - 1]->setChild(i)->setFull(type);

                if (i != blocks[lvl - 1]->getChildInd(x, y, z))
                    addToFullBlocks(blocks[lvl - 1]->getChild(i));
            }
            blocks[lvl - 1]->getChild(x, y, z)->size--;
            blocks.push_back(blocks[lvl - 1]->getChild(x, y, z));
        }

        // delete empty blocks
        for (int lvl = MAX_LEVEL - 1; lvl >= 0; --lvl) {
            if (blocks[lvl]->getChild(x, y, z)->isEmpty()) {
                blocks[lvl]->deleteChild(x, y, z);
            }
            blocks[lvl]->type = 0;
        }

        return last_type;
    }

    Block* getBlock(int x, int y, int z) {
        Block* target = root;
        Block* tmp = root;
        int level = 0;
        int ind;
        unsigned int offset = 1 << (MAX_LEVEL - 1);

        while (tmp != NULL) {
            target = tmp;
            level++;
            ind = ((x & offset) << 2 | ((y & offset) << 1) | ((z & offset))) >> (MAX_LEVEL - level);
            tmp = target->getChild(ind);
            offset >>= 1;
        }

        return target;
    }

    void show(Block* n) {
        if (n != NULL) {
            n->show();

            for (int i = 0; i < 8; i++)
                show(n->getChild(i));
        }
    }

    void show(vector<Block*> v) {
        for (int i = 0; i < (int)v.size(); i++)
            show(v[i]);
    }

    void show_all() {
        cout << "----- show_all -----\n\n";
        printf("                lvl |    position     | dim size |               range               |   size    |  max_size  |  type |\n");
        show(root);
    }

    void deleteAll() {
        deleteRec(root);     
    }

    void deleteRec(Block* tmp) {
        if (tmp != NULL) {
            for (int i = 0; i < 8; ++i) {
                deleteRec(tmp->ch[i]);
            }
            delete tmp;
        }
    }

private:
    void addToFullBlocks(Block* n) {
        addData(chunk, this, n); 
    }

    void deleteFromFullBlocks(Block* n) {
        remData(chunk, this, n);
    }

    void merge(int x, int y, int z) {
        Block* target = root;
        Block* tmp = root;
        int level = target->level;
        int offset = 1 << (MAX_LEVEL - 1);

        vector <Block*> blocksList = getBlocks(x, y, z);
        for (int i = blocksList.size() - 1; i >= 0; --i) {
            if (blocksList[i]->mergeUpdateType()) {

                for (int j = 0; j < MAX_CHILD; ++j) {
                    if (blocksList[i]->getChild(j) != NULL) {
                        // delete
                        deleteFromFullBlocks(blocksList[i]->getChild(j));
                        blocksList[i]->deleteChild(j);
                    }
                }
                addToFullBlocks(blocksList[i]);
            }
        }
    }

    vector <Block*> getBlocks(int x, int y, int z) {
        vector <Block*> blocksList;
        Block* target = root;
        Block* tmp = root;
        int level = 0;
        int ind;
        unsigned int offset = 1 << (MAX_LEVEL - 1);

        while (tmp != NULL) {
            target = tmp;
            blocksList.push_back(target);
            level++;
            ind = ((x & offset) << 2 | ((y & offset) << 1) | ((z & offset))) >> (MAX_LEVEL - level);
            tmp = target->ch[ind];
            offset >>= 1;
        }

        return blocksList;
    }
};