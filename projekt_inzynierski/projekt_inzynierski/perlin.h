#pragma once
#include <algorithm>

const float M_PI = 3.14159265358979323846f;

using namespace std;

class vec {
public:
    float x, y;

public:
    vec() { x = y = 0; }

    vec(float xn, float yn) { x = xn; y = yn; }
};

class Perlin {

private:
    int seed;
    int vec_type; // typ losowanych wektorow
    int size; // rozmiar pola
    int grid_size; // rozmiar kratki z siatki
    int grid_count; // liczba kratek w siatce
    float** all;
    vec** grid = NULL; // siatka wektorów
    float*** minMap = NULL;
    float*** maxMap = NULL;

    vec randVec() {
        float x = 0, y = 0;

        if (vec_type == 0) {
            // wektory równoleg³e do osi
            int ran = rand() % 4;
            switch (ran) {
            case 0: x = -1; y = 0; break;
            case 1: x = 1; y = 0; break;
            case 2: x = 0; y = -1; break;
            case 3: x = 0; y = 1; break;
            }
        }
        else if (vec_type == 1) {
            // wektory dowolne
            float rad = (rand() % 10000) / 10000.0f * 2.0f * M_PI;
            x = cos(rad);
            y = sin(rad);
        }

        return vec(x, y);
    }

    float fade(float x) {
        return x * x * x * (x * (x * 6.0f - 15.0f) + 10.0f);
    }

public:
    float*** getMinMap() {
        if (minMap == NULL)
            setMinMaxMap();
        return minMap;
    }

    float*** getMaxMap() {
        if (maxMap == NULL)
            setMinMaxMap();
        return maxMap;
    }

    void setMinMaxMap() {
        for (int i = 0; i < MAX_DIM_SIZE/16; i += 1) {
            for (int j = 0; j < MAX_DIM_SIZE/16; j += 1) {
                recSetMinMaxMap(i, j, 0, 0, 4);
            }
        }
    }

    pair<float, float> recSetMinMaxMap(int x0, int y0, int x, int y, int lvl) {
        int map_x = (x0 << (4 - lvl)) + x;
        int map_y = (y0 << (4 - lvl)) + y;
        if (lvl > 0) {
            minMap[lvl][map_x][map_y] = 1.0f;
            maxMap[lvl][map_x][map_y] = 0.0f;

            for (int i = 0; i < 2; ++i) {
                for (int j = 0; j < 2; ++j) {
                    pair<float, float> new_pair = recSetMinMaxMap(x0, y0, x*2 + i, y*2 + j, lvl - 1);
                    minMap[lvl][map_x][map_y] = min(minMap[lvl][map_x][map_y], new_pair.first);
                    maxMap[lvl][map_x][map_y] = max(maxMap[lvl][map_x][map_y], new_pair.second);
                }
            }
            return pair<float, float> (minMap[lvl][map_x][map_y], maxMap[lvl][map_x][map_y]);
        }
        else {
            minMap[lvl][map_x][map_y] = all[map_x][map_y];
            maxMap[lvl][map_x][map_y] = all[map_x][map_y];

            return pair<float, float>(minMap[lvl][map_x][map_y], maxMap[lvl][map_x][map_y]);
        }
    }

    Perlin(int _field_size, int _grid_size, int _vec_type) {
        seed = 0;

        size = _field_size;
        grid_size = _grid_size;
        vec_type = _vec_type;
        grid_count = size / grid_size + 2;

        grid = new vec * [grid_count];
        for (int i = 0; i < grid_count; ++i)
            grid[i] = new vec[grid_count];

        // minMap, maxMap allocation
        minMap = new float** [5];
        maxMap = new float** [5];

        for (int i = 0; i < 5; ++i) {
            minMap[i] = new float* [MAX_DIM_SIZE >> i];
            maxMap[i] = new float* [MAX_DIM_SIZE >> i];

            for (int j = 0; j < MAX_DIM_SIZE >> i; ++j) {
                minMap[i][j] = new float[MAX_DIM_SIZE >> i];
                maxMap[i][j] = new float[MAX_DIM_SIZE >> i];
            }
        }
    }


    void setSeed(int nseed) {
        seed = nseed;
        setGrid();
    }


    void setGrid() {
        srand(seed);
        for (int i = 0; i < grid_count; ++i) {
            for (int j = 0; j < grid_count; ++j) {
                grid[i][j] = randVec();
            }
        }
    }

    void setGridSize(int _grid_size) {
        grid_size = _grid_size;
    };

    float** getAll() {
        all = new float* [size];
        for (int i = 0; i < size; ++i)
            all[i] = new float[size];

        float delta = 1.0f / grid_size;

        for (int grid_i = 0; grid_i < size; grid_i += grid_size) {
            for (int grid_j = 0; grid_j < size; grid_j += grid_size) {
                float u = 0.0f, v = 0.0f;

                int gx = grid_i / grid_size;
                int gy = grid_j / grid_size;

                float dot1u = 0.0f;
                float dot2u = -grid[gy][gx + 1].x;
                float dot3u = 0.0f;
                float dot4u = -grid[gy + 1][gx + 1].x;

                float dot1u_delta = grid[gy][gx].x * delta;
                float dot1v_delta = grid[gy][gx].y * delta;
                float dot2u_delta = grid[gy][gx + 1].x * delta;
                float dot2v_delta = grid[gy][gx + 1].y * delta;
                float dot3u_delta = grid[gy + 1][gx].x * delta;
                float dot3v_delta = grid[gy + 1][gx].y * delta;
                float dot4u_delta = grid[gy + 1][gx + 1].x * delta;
                float dot4v_delta = grid[gy + 1][gx + 1].y * delta;

                for (int i = 0; (i < grid_size) && (i + grid_i < size); ++i) {
                    v = 0.0f;
                    float dot1 = dot1u + 0.0f;
                    float dot2 = dot2u + 0.0f;
                    float dot3 = dot3u - grid[gy + 1][gx].y;
                    float dot4 = dot4u - grid[gy + 1][gx + 1].y;

                    float fu = fade(u);

                    for (int j = 0; (j < grid_size) && (j + grid_j < size); ++j) {
                        float fv = fade(v);

                        float lin1 = dot1 + (dot2 - dot1) * fu;
                        float lin2 = dot3 + (dot4 - dot3) * fu;
                        float lin = lin1 + (lin2 - lin1) * fv;
                        lin = (lin + 1) / 2;

                        all[grid_i + i][grid_j + j] = lin;

                        dot1 += dot1v_delta;
                        dot2 += dot2v_delta;
                        dot3 += dot3v_delta;
                        dot4 += dot4v_delta;
                        v += delta;
                    }
                    dot1u += dot1u_delta;
                    dot2u += dot2u_delta;
                    dot3u += dot3u_delta;
                    dot4u += dot4u_delta;
                    u += delta;
                }
            }
        }
        return all;
    }

};