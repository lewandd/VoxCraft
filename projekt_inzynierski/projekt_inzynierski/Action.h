#pragma once
class Action {
public:
    int mode;
    int octree;
    int x;
    int y;
    int z;
    int type;

    Action(int mode_, int octree_, int x_, int y_, int z_, int type_);
};

