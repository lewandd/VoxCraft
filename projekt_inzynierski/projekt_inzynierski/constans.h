#pragma once

// Octree

const int MAX_LEVEL = 4;
const int MAX_DIM_SIZE = 1 << MAX_LEVEL;
const int MAX_SIZE = MAX_DIM_SIZE * MAX_DIM_SIZE * MAX_DIM_SIZE;
const int MAX_CHILD = 8;

const int DIM_SIZE[] = {
    //256, // MAX_LEVEL - 8
    //128, // MAX_LEVEL - 7 
    //64,  // MAX_LEVEL - 6
    //32,  // MAX_LEVEL - 5
    16,  // MAX_LEVEL - 4
    8,   // MAX_LEVEL - 3
    4,   // MAX_LEVEL - 2
    2,   // MAX_LEVEL - 1
    1    // MAX_LEVEL
};

// Chunks

const int CHUNKS_COUNT = 256;
const int CHUNK_SIZE = MAX_DIM_SIZE;

const int TOTAL_SIZE = CHUNKS_COUNT * CHUNK_SIZE;

// Noise

const int NOISE_MAP_SIZE = 128;
const int FREQ1 = 32;

const int NOISE_MAP_COUNT = TOTAL_SIZE / NOISE_MAP_SIZE;

// Textures

const int NUM_BLOCK_TEXTURES = 10;

// Target

const float TARGET_SIZE = 1.0f;
const float TARGET_THICKNESS = 2.0f;
const float TARGET_COLOR_R = 1.0f;
const float TARGET_COLOR_G = 1.0f;
const float TARGET_COLOR_B = 1.0f;
