#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

std::vector <glm::vec3> c[9];

void set_data() {
    c[2].push_back(glm::vec3(8, 0, 0));
    c[2].push_back(glm::vec3(4, 0, 8));
    c[2].push_back(glm::vec3(8, 0, 8));
    c[2].push_back(glm::vec3(0, 0, 8));

    c[1].push_back(glm::vec3(0, 0, 0));
    c[1].push_back(glm::vec3(0, 2, 0));
    c[1].push_back(glm::vec3(0, 0, 4));
    c[1].push_back(glm::vec3(0, 2, 4));
    c[1].push_back(glm::vec3(0, 0, 6));
    c[1].push_back(glm::vec3(0, 2, 6));

    c[1].push_back(glm::vec3(4, 2, 0));
    c[1].push_back(glm::vec3(4, 6, 2));

    c[1].push_back(glm::vec3(6, 0, 0));
    c[1].push_back(glm::vec3(6, 0, 2));
    c[1].push_back(glm::vec3(6, 0, 4));
    c[1].push_back(glm::vec3(6, 0, 6));
    c[1].push_back(glm::vec3(6, 2, 0));
    c[1].push_back(glm::vec3(6, 2, 2));
    c[1].push_back(glm::vec3(6, 2, 4));
    c[1].push_back(glm::vec3(6, 2, 6));
    c[1].push_back(glm::vec3(6, 4, 10));

    c[1].push_back(glm::vec3(8, 4, 10));
    c[1].push_back(glm::vec3(8, 8, 2));
    c[1].push_back(glm::vec3(8, 10, 2));
    c[1].push_back(glm::vec3(8, 0, 4));
    c[1].push_back(glm::vec3(8, 0, 6));

    c[1].push_back(glm::vec3(10, 4, 10));
    c[1].push_back(glm::vec3(10, 0, 4));
    c[1].push_back(glm::vec3(10, 0, 6));

    c[0].push_back(glm::vec3(0, 0, 2));
    c[0].push_back(glm::vec3(0, 0, 3));
    c[0].push_back(glm::vec3(1, 0, 2));
    c[0].push_back(glm::vec3(1, 0, 3));
    c[0].push_back(glm::vec3(2, 0, 2));
    c[0].push_back(glm::vec3(2, 0, 3));
    c[0].push_back(glm::vec3(3, 0, 2));
    c[0].push_back(glm::vec3(3, 0, 3));
    c[0].push_back(glm::vec3(4, 0, 2));
    c[0].push_back(glm::vec3(4, 0, 3));
    c[0].push_back(glm::vec3(5, 0, 2));
    c[0].push_back(glm::vec3(5, 0, 3));

    c[0].push_back(glm::vec3(2, 0, 1));
    c[0].push_back(glm::vec3(3, 0, 1));
    c[0].push_back(glm::vec3(4, 0, 1));
    c[0].push_back(glm::vec3(5, 0, 1));

    c[0].push_back(glm::vec3(2, 0, 0));
    c[0].push_back(glm::vec3(3, 0, 0));
    c[0].push_back(glm::vec3(4, 0, 0));
    c[0].push_back(glm::vec3(5, 0, 0));

    c[0].push_back(glm::vec3(2, 0, 4));
    c[0].push_back(glm::vec3(3, 0, 4));
    c[0].push_back(glm::vec3(4, 0, 4));
    c[0].push_back(glm::vec3(5, 0, 4));

    c[0].push_back(glm::vec3(2, 0, 5));
    c[0].push_back(glm::vec3(3, 0, 5));
    c[0].push_back(glm::vec3(4, 0, 5));
    c[0].push_back(glm::vec3(5, 0, 5));

    c[0].push_back(glm::vec3(2, 0, 6));
    c[0].push_back(glm::vec3(3, 0, 6));
    c[0].push_back(glm::vec3(4, 0, 6));
    c[0].push_back(glm::vec3(5, 0, 6));

    c[0].push_back(glm::vec3(2, 0, 7));
    c[0].push_back(glm::vec3(3, 0, 7));
    c[0].push_back(glm::vec3(4, 0, 7));
    c[0].push_back(glm::vec3(5, 0, 7));

    c[0].push_back(glm::vec3(2, 1, 0));
    c[0].push_back(glm::vec3(3, 1, 0));
    c[0].push_back(glm::vec3(4, 1, 0));
    c[0].push_back(glm::vec3(5, 1, 0));

    c[0].push_back(glm::vec3(2, 2, 0));
    c[0].push_back(glm::vec3(3, 2, 0));

    c[0].push_back(glm::vec3(5, 1, 1));

    c[0].push_back(glm::vec3(0, 2, 3));

    c[0].push_back(glm::vec3(2, 1, 5));
    c[0].push_back(glm::vec3(2, 2, 5));

    c[0].push_back(glm::vec3(5, 1, 5));
    c[0].push_back(glm::vec3(5, 2, 5));

    // sufit
    c[0].push_back(glm::vec3(2, 3, 0));
    c[0].push_back(glm::vec3(3, 3, 0));

    c[0].push_back(glm::vec3(2, 3, 1));
    c[0].push_back(glm::vec3(3, 3, 1));

    c[0].push_back(glm::vec3(0, 3, 2));
    c[0].push_back(glm::vec3(0, 3, 3));
    c[0].push_back(glm::vec3(1, 3, 2));
    c[0].push_back(glm::vec3(1, 3, 3));

    c[0].push_back(glm::vec3(2, 3, 2));
    c[0].push_back(glm::vec3(3, 3, 2));
    c[0].push_back(glm::vec3(4, 3, 2));
    c[0].push_back(glm::vec3(5, 3, 2));

    c[0].push_back(glm::vec3(2, 3, 3));
    c[0].push_back(glm::vec3(3, 3, 3));
    c[0].push_back(glm::vec3(4, 3, 3));
    c[0].push_back(glm::vec3(5, 3, 3));

    c[0].push_back(glm::vec3(2, 3, 4));
    c[0].push_back(glm::vec3(3, 3, 4));
    c[0].push_back(glm::vec3(4, 3, 4));
    c[0].push_back(glm::vec3(5, 3, 4));

    c[0].push_back(glm::vec3(2, 3, 5));
    c[0].push_back(glm::vec3(3, 3, 5));
    c[0].push_back(glm::vec3(4, 3, 5));
    c[0].push_back(glm::vec3(5, 3, 5));

    c[0].push_back(glm::vec3(2, 3, 6));
    c[0].push_back(glm::vec3(3, 3, 6));
    c[0].push_back(glm::vec3(4, 3, 6));
    c[0].push_back(glm::vec3(5, 3, 6));

    c[0].push_back(glm::vec3(2, 3, 7));
    c[0].push_back(glm::vec3(3, 3, 7));
    c[0].push_back(glm::vec3(4, 3, 7));
    c[0].push_back(glm::vec3(5, 3, 7));

    // dom

    c[0].push_back(glm::vec3(2, 4, 1));
    c[0].push_back(glm::vec3(2, 5, 1));

    c[0].push_back(glm::vec3(3, 4, 1));
    c[0].push_back(glm::vec3(3, 5, 1));

    c[0].push_back(glm::vec3(4, 4, 1));
    c[0].push_back(glm::vec3(4, 5, 1));
    c[0].push_back(glm::vec3(4, 6, 1));

    c[0].push_back(glm::vec3(5, 4, 1));
    c[0].push_back(glm::vec3(5, 5, 1));

    c[0].push_back(glm::vec3(6, 4, 1));
    c[0].push_back(glm::vec3(6, 5, 1));

    c[0].push_back(glm::vec3(6, 4, 2));
    c[0].push_back(glm::vec3(6, 5, 2));

    c[0].push_back(glm::vec3(6, 4, 3));
    c[0].push_back(glm::vec3(6, 5, 3));

    c[0].push_back(glm::vec3(6, 4, 4));
    c[0].push_back(glm::vec3(6, 5, 4));

    c[0].push_back(glm::vec3(6, 4, 5));
    c[0].push_back(glm::vec3(6, 5, 5));

    c[0].push_back(glm::vec3(5, 4, 5));
    c[0].push_back(glm::vec3(5, 5, 5));

    c[0].push_back(glm::vec3(4, 4, 5));
    c[0].push_back(glm::vec3(4, 5, 5));

    c[0].push_back(glm::vec3(3, 4, 5));
    c[0].push_back(glm::vec3(3, 5, 5));

    c[0].push_back(glm::vec3(2, 4, 5));
    c[0].push_back(glm::vec3(2, 5, 5));

    //dach

    c[0].push_back(glm::vec3(2, 6, 5));
    c[0].push_back(glm::vec3(2, 6, 4));
    c[0].push_back(glm::vec3(2, 6, 3));
    c[0].push_back(glm::vec3(2, 6, 2));
    c[0].push_back(glm::vec3(2, 6, 1));

    c[0].push_back(glm::vec3(3, 6, 5));
    c[0].push_back(glm::vec3(3, 6, 4));
    c[0].push_back(glm::vec3(3, 6, 3));
    c[0].push_back(glm::vec3(3, 6, 2));
    c[0].push_back(glm::vec3(3, 6, 1));

    c[0].push_back(glm::vec3(4, 6, 5));
    c[0].push_back(glm::vec3(4, 6, 4));

    c[0].push_back(glm::vec3(5, 6, 5));
    c[0].push_back(glm::vec3(5, 6, 4));
    c[0].push_back(glm::vec3(5, 6, 1));

    c[0].push_back(glm::vec3(6, 6, 5));
    c[0].push_back(glm::vec3(6, 6, 4));
    c[0].push_back(glm::vec3(6, 6, 3));
    c[0].push_back(glm::vec3(6, 6, 2));
    c[0].push_back(glm::vec3(6, 6, 1));

    //czubek

    c[0].push_back(glm::vec3(3, 7, 4));
    c[0].push_back(glm::vec3(3, 7, 3));
    c[0].push_back(glm::vec3(3, 7, 2));

    c[0].push_back(glm::vec3(4, 7, 4));

    c[0].push_back(glm::vec3(5, 7, 4));

    // drzewo

    c[0].push_back(glm::vec3(9, 4, 2));
    c[0].push_back(glm::vec3(9, 5, 2));
    c[0].push_back(glm::vec3(9, 6, 2));

    c[0].push_back(glm::vec3(8, 7, 1));
    c[0].push_back(glm::vec3(8, 7, 2));
    c[0].push_back(glm::vec3(8, 7, 3));
    c[0].push_back(glm::vec3(9, 7, 1));
    c[0].push_back(glm::vec3(9, 7, 2));
    c[0].push_back(glm::vec3(9, 7, 3));
    c[0].push_back(glm::vec3(10, 7, 1));
    c[0].push_back(glm::vec3(10, 7, 2));
    c[0].push_back(glm::vec3(10, 7, 3));

    c[0].push_back(glm::vec3(8, 8, 1));
    c[0].push_back(glm::vec3(9, 8, 1));
    c[0].push_back(glm::vec3(10, 8, 1));
    c[0].push_back(glm::vec3(10, 8, 2));
    c[0].push_back(glm::vec3(10, 8, 3));

    c[0].push_back(glm::vec3(8, 9, 1));
    c[0].push_back(glm::vec3(9, 9, 1));
    c[0].push_back(glm::vec3(10, 9, 1));
    c[0].push_back(glm::vec3(10, 9, 2));
    c[0].push_back(glm::vec3(10, 9, 3));

    c[0].push_back(glm::vec3(8, 10, 1));
    c[0].push_back(glm::vec3(9, 10, 1));
    c[0].push_back(glm::vec3(10, 10, 1));
    c[0].push_back(glm::vec3(10, 10, 2));
    c[0].push_back(glm::vec3(10, 10, 3));

    c[0].push_back(glm::vec3(8, 11, 1));
    c[0].push_back(glm::vec3(9, 11, 1));
    c[0].push_back(glm::vec3(10, 11, 1));
    c[0].push_back(glm::vec3(10, 11, 2));
    c[0].push_back(glm::vec3(10, 11, 3));

    //dziora

    c[0].push_back(glm::vec3(8, 2, 4));
    c[0].push_back(glm::vec3(9, 2, 4));
    c[0].push_back(glm::vec3(10, 2, 4));
    c[0].push_back(glm::vec3(11, 2, 4));

    c[0].push_back(glm::vec3(8, 2, 5));
    c[0].push_back(glm::vec3(9, 2, 5));
    c[0].push_back(glm::vec3(10, 2, 5));
    c[0].push_back(glm::vec3(11, 2, 5));

    c[0].push_back(glm::vec3(8, 2, 6));
    c[0].push_back(glm::vec3(9, 2, 6));
    c[0].push_back(glm::vec3(10, 2, 6));
    c[0].push_back(glm::vec3(11, 2, 6));

    c[0].push_back(glm::vec3(8, 2, 7));
    c[0].push_back(glm::vec3(9, 2, 7));
    c[0].push_back(glm::vec3(10, 2, 7));
    c[0].push_back(glm::vec3(11, 2, 7));

    c[0].push_back(glm::vec3(8, 3, 4));
    c[0].push_back(glm::vec3(9, 3, 4));
    c[0].push_back(glm::vec3(10, 3, 4));
    c[0].push_back(glm::vec3(11, 3, 4));

    c[0].push_back(glm::vec3(8, 3, 5));

    c[0].push_back(glm::vec3(11, 3, 5));

    c[0].push_back(glm::vec3(8, 3, 6));

    c[0].push_back(glm::vec3(11, 3, 6));

    c[0].push_back(glm::vec3(8, 3, 7));
    c[0].push_back(glm::vec3(9, 3, 7));
    c[0].push_back(glm::vec3(10, 3, 7));
    c[0].push_back(glm::vec3(11, 3, 7));

    // wzgorze
    c[0].push_back(glm::vec3(1, 4, 11));
    c[0].push_back(glm::vec3(2, 4, 11));
    c[0].push_back(glm::vec3(3, 4, 11));
    c[0].push_back(glm::vec3(4, 4, 11));
    c[0].push_back(glm::vec3(5, 4, 11));

    c[0].push_back(glm::vec3(2, 4, 10));
    c[0].push_back(glm::vec3(3, 4, 10));
    c[0].push_back(glm::vec3(4, 4, 10));
    c[0].push_back(glm::vec3(5, 4, 10));

    c[0].push_back(glm::vec3(2, 4, 9));
    c[0].push_back(glm::vec3(3, 4, 9));
    c[0].push_back(glm::vec3(4, 4, 9));
    c[0].push_back(glm::vec3(5, 4, 9));
    c[0].push_back(glm::vec3(6, 4, 9));
    c[0].push_back(glm::vec3(7, 4, 9));
    c[0].push_back(glm::vec3(8, 4, 9));
    c[0].push_back(glm::vec3(9, 4, 9));
    c[0].push_back(glm::vec3(10, 4, 9));
    c[0].push_back(glm::vec3(11, 4, 9));

    c[0].push_back(glm::vec3(4, 4, 8));
    c[0].push_back(glm::vec3(5, 4, 8));
    c[0].push_back(glm::vec3(6, 4, 8));
    c[0].push_back(glm::vec3(7, 4, 8));
    c[0].push_back(glm::vec3(8, 4, 8));
    c[0].push_back(glm::vec3(9, 4, 8));
    c[0].push_back(glm::vec3(10, 4, 8));
    c[0].push_back(glm::vec3(11, 4, 8));

    c[0].push_back(glm::vec3(11, 4, 7));

    // wzgorze 2

    c[0].push_back(glm::vec3(5, 5, 11));
    c[0].push_back(glm::vec3(5, 5, 10));

    c[0].push_back(glm::vec3(6, 5, 9));
    c[0].push_back(glm::vec3(7, 5, 9));
    c[0].push_back(glm::vec3(8, 5, 9));
    c[0].push_back(glm::vec3(9, 5, 9));
    c[0].push_back(glm::vec3(10, 5, 9));
    c[0].push_back(glm::vec3(11, 5, 9));

    c[0].push_back(glm::vec3(11, 5, 8));

    // wzgorze 3

    c[0].push_back(glm::vec3(7, 6, 11));
    c[0].push_back(glm::vec3(8, 6, 11));
    c[0].push_back(glm::vec3(9, 6, 11));
    c[0].push_back(glm::vec3(10, 6, 11));
    c[0].push_back(glm::vec3(11, 6, 11));

    c[0].push_back(glm::vec3(10, 6, 10));
    c[0].push_back(glm::vec3(11, 6, 10));
}