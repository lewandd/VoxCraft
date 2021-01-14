#pragma once
#include "octree_node.h"
#include <vector>

class Octree {

public:
    OctreeNode* root = new OctreeNode(0, 0, 0, 0, 0);
    float* data;
    int* data_size;
    int x, y, o;

    Octree(float* data_, int* data_size_, int x_, int y_, int o_) {
        this->data = data_;
        this->data_size = data_size_;
        this->x = x_;
        this->y = y_;
        this->o = o_;
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

        OctreeNode* target = root;
        OctreeNode* tmp = root;
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

        OctreeNode* target = root;
        OctreeNode* tmp = root;
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

    void remove(int x, int y, int z) {
        vector <OctreeNode*> blocks = getBlocks(x, y, z);
        if (!blocks.back()->isFull()) {
            printf("WARNING (remove): block does not exist (%d, %d, %d)\n", x, y, z);
            return;
        }

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

    }

    OctreeNode* getBlock(int x, int y, int z) {
        OctreeNode* target = root;
        OctreeNode* tmp = root;
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

    void show(OctreeNode* n) {
        if (n != NULL) {
            n->show();

            for (int i = 0; i < 8; i++)
                show(n->getChild(i));
        }
    }

    void show(vector<OctreeNode*> v) {
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

    void deleteRec(OctreeNode* tmp) {
        if (tmp != NULL) {
            for (int i = 0; i < 8; ++i) {
                deleteRec(tmp->ch[i]);
            }
            delete tmp;
        }
    }

private:
    void addToFullBlocks(OctreeNode* n) {
        
        n->ind = *data_size;

        data[*data_size] = n->x + 16*x;
        data[*data_size+1] = n->y + 16*o;
        data[*data_size+2] = n->z + 16*y;
        data[*data_size+3] = 1 << (MAX_LEVEL - n->level);
        data[*data_size+4] = n->type;
        *data_size = *data_size + 5;
    }

    void deleteFromFullBlocks(OctreeNode* n) {
        data[n->ind] = data[*data_size-5];
        data[n->ind + 1] = data[*data_size - 4];
        data[n->ind + 2] = data[*data_size - 3];
        data[n->ind + 3] = data[*data_size - 2];
        data[n->ind + 4] = data[*data_size - 1];
        
        *data_size = *data_size - 5;
        n->ind = -1;
    }

    void merge(int x, int y, int z) {
        OctreeNode* target = root;
        OctreeNode* tmp = root;
        int level = target->level;
        int offset = 1 << (MAX_LEVEL - 1);

        vector <OctreeNode*> blocksList = getBlocks(x, y, z);
        for (int i = blocksList.size() - 1; i >= 0; --i) {
            if (blocksList[i]->mergeUpdateType()) {
                addToFullBlocks(blocksList[i]);

                for (int j = 0; j < MAX_CHILD; ++j) {
                    if (blocksList[i]->getChild(j) != NULL) {
                        // delete
                        deleteFromFullBlocks(blocksList[i]->getChild(j));
                        blocksList[i]->deleteChild(j);
                    }
                }
            }
        }
    }

    vector <OctreeNode*> getBlocks(int x, int y, int z) {
        vector <OctreeNode*> blocksList;
        OctreeNode* target = root;
        OctreeNode* tmp = root;
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