#pragma once
#include "octree_node.h"
#include <vector>

class Octree {

public:
    OctreeNode* root = new OctreeNode(0, 0, 0, 0, 0);
    vector <OctreeNode*> fullBlocks;
    OctreeNode* selected = NULL;
    int selected_x, selected_y, selected_z;

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

    bool existSelected() {
        return selected != NULL;
    }

    void unsetSelected() {
        if (existSelected()) {
            selected->unsetSeleted();
            selected = NULL;
        }
    }

    OctreeNode* setSelected(int x, int y, int z) {
        unsetSelected();

        selected = getBlock(x, y, z);
        selected->setSeleted();
        selected_x = x;
        selected_y = y;
        selected_z = z;

        return selected;
    }

    OctreeNode* getSelected() {
        return selected;
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

private:
    void addToFullBlocks(OctreeNode* n) {
        fullBlocks.push_back(n);
        n->ind = fullBlocks.size() - 1;
    }

    void deleteFromFullBlocks(OctreeNode* n) {
        fullBlocks.back()->ind = n->ind;
        fullBlocks[n->ind] = fullBlocks.back();
        fullBlocks.pop_back();
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