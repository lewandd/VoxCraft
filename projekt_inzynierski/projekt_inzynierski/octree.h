#pragma once
#include "octree_node.h"
#include <vector>

class Octree;
class CHUNK;
void remData(CHUNK*, Octree*, Block*);
void addData(CHUNK*, Octree*, Block*);
void addSingleData(CHUNK*, Octree*, Block*, int);
Block* getNeighbour(CHUNK* ch, Octree* o, int bx, int by, int bz, int blvl, CHUNK **neighch, Octree **neigho, int i, int &nchx, int &nchy);
void updateChunk(CHUNK* ch, int neigh);

class Octree {

public:
    Block* root;
    CHUNK* chunk;
    int x, y, o;

    Octree(CHUNK *chunk_, int x_, int y_, int o_) {
        this->x = x_;
        this->y = y_;
        this->o = o_;
        this->chunk = chunk_;
        root = NULL;
    }

    void addMinMap(float ***minMap, int lvl, int x, int y, int z, int type, int z0) {
        if (lvl >= 0) {
            if (minMap[lvl][x][y]*16 >= (1 << lvl)+z) {
                setFullBlock(x<<lvl, z0+z, y << lvl, type, MAX_LEVEL-lvl, false, false, false, false, false, false);
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
        if (root == NULL)
            root = new Block(0, 0, 0, 0, 0);
        
        if (getBlock(x, y, z)->isFull())    {
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

    void setFullBlock(int x, int y, int z, int type, int target_level, bool v0, bool v1, bool v2, bool v3, bool v4, bool v5) {
        if (root == NULL)
            root = new Block(0, 0, 0, 0, 0);
        
        Block* tm = root;

        int size = 1 << ((MAX_LEVEL - target_level) * 3);

        for (int i = 0; i < target_level; ++i) {
            if (tm->ch[INDEX[i][x][y][z]] == NULL)
                tm->ch[INDEX[i][x][y][z]] = new Block(i + 1, 0, POSITION[i + 1][x][y][z][0], POSITION[i + 1][x][y][z][1], POSITION[i + 1][x][y][z][2]);
            tm->size += size;
            tm = tm->ch[INDEX[i][x][y][z]];
        }
        tm->size = size;
        tm->type = type;
        tm->vis[0] = v0;
        tm->vis[1] = v1;
        tm->vis[2] = v2;
        tm->vis[3] = v3;
        tm->vis[4] = v4;
        tm->vis[5] = v5;
        addData(chunk, this, tm);
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
                deleteFromFullBlocks(blocks[lvl], true);

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
            if (lvl == MAX_LEVEL) {
                Block* to_del = blocks[lvl - 1]->getChild(x, y, z);
                addToFullBlocks(to_del);
                deleteFromFullBlocks(to_del, true);
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
        if (root == NULL)
            return root;

        Block* tm = root;
        int lvl = 0;
        while (tm->ch[INDEX[lvl][x][y][z]] != NULL)
            tm = tm->ch[INDEX[lvl++][x][y][z]];

        return tm;
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
        for (int j = 0; j < 6; ++j) {
            CHUNK *neighch;
            Octree *neigho;
            int nchx, nchy;
            Block* neigh = getNeighbour(chunk, this, n->x, n->y, n->z, n->level, &neighch, &neigho, j, nchx, nchy);

            int bsize = 1 << (MAX_LEVEL - n->level);
            if (neigh == NULL && j == 3 && (n->y + bsize + 16) % 16 == 0 && o < 7) {
                // up side into empty octree
                n->vis[j] = true;
            }
            else if (neigh == NULL) {
                n->vis[j] = true;
            }
            else if (neigh->isFull() == false && neigh->level <= n->level) {
                n->vis[j] = true;
            }
            else if (neigh->isFull() == true && neigh->level == n->level) {
                n->vis[j] = false;
                // delete visible neighbour
            }
            else if (neigh->isFull() == true && neigh->level > n->level) {
                n->vis[j] = true;
            }
            else {
                n->vis[j] = false;
            }
        }
        
        addData(chunk, this, n); 
    }

    void deleteFromFullBlocks(Block* n, bool checkNeigh) {
        int bsize = 1 << (MAX_LEVEL - n->level);

        bool needToUpdateChunk1 = false;
        bool needToUpdateChunk2 = false;
        bool needToUpdateChunk3 = false;
        bool needToUpdateChunk4 = false;

        if (checkNeigh) {

            for (int j = 0; j < 6; ++j) {

                for (int k = 0; k < bsize; ++k) {

                    for (int l = 0; l < bsize; ++l) {
                        Block* neigh;
                        CHUNK* newChunk;
                        Octree* newOctree;
                        int nchx, nchy;

                        if (j == 0) {
                            neigh = getNeighbour(chunk, this, n->x, n->y + k, n->z + l, n->level, &newChunk, &newOctree, 0, nchx, nchy);
                            if (neigh != NULL && neigh->isFull() && neigh->vis[1] == false) {
                                if (nchx != this->x || nchy != this->y)
                                    needToUpdateChunk1 = true;
                                addSingleData(newChunk, newOctree, neigh, 1);
                            }
                        }
                        if (j == 1) {
                            neigh = getNeighbour(chunk, this, n->x + bsize - 1, n->y + k, n->z + l, n->level, &newChunk, &newOctree, 1, nchx, nchy);
                            if (neigh != NULL && neigh->isFull() && neigh->vis[0] == false) {
                                if (nchx != this->x || nchy != this->y)
                                    needToUpdateChunk2 = true;
                                addSingleData(newChunk, newOctree, neigh, 0);
                            }
                        }
                        if (j == 2) {
                            neigh = getNeighbour(chunk, this, n->x + k, n->y, n->z + l, n->level, &newChunk, &newOctree, 2, nchx, nchy);
                            if (neigh != NULL && neigh->isFull() && neigh->vis[3] == false) {
                                addSingleData(newChunk, newOctree, neigh, 3);
                            }
                        }
                        if (j == 3) {
                            neigh = getNeighbour(chunk, this, n->x + k, n->y + bsize - 1, n->z + l, n->level, &newChunk, &newOctree, 3, nchx, nchy);
                            if (neigh != NULL && neigh->isFull() && neigh->vis[2] == false) {
                                addSingleData(newChunk, newOctree, neigh, 2);
                            }
                        }
                        if (j == 4) {
                            neigh = getNeighbour(chunk, this, n->x + k, n->y + l, n->z, n->level, &newChunk, &newOctree, 4, nchx, nchy);
                            if (neigh != NULL && neigh->isFull() && neigh->vis[5] == false) {
                                if (nchx != this->x || nchy != this->y)
                                    needToUpdateChunk3 = true;
                                addSingleData(newChunk, newOctree, neigh, 5);
                            }
                        }
                        if (j == 5) {
                            neigh = getNeighbour(chunk, this, n->x + k, n->y + l, n->z + bsize - 1, n->level, &newChunk, &newOctree, 5, nchx, nchy);
                            if (neigh != NULL && neigh->isFull() && neigh->vis[4] == false) {
                                if (nchx != this->x || nchy != this->y)
                                    needToUpdateChunk4 = true;
                                addSingleData(newChunk, newOctree, neigh, 4);
                            }
                        }
                    }
                }
            }
        }

        remData(chunk, this, n);
        if (needToUpdateChunk1) {
            updateChunk(chunk, 1);
        }
        if (needToUpdateChunk2) {
            updateChunk(chunk, 0);
        }
        if (needToUpdateChunk3) {
            updateChunk(chunk, 3);
        }
        if (needToUpdateChunk4) {
            updateChunk(chunk, 2);
        }
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
                        deleteFromFullBlocks(blocksList[i]->getChild(j), false);
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