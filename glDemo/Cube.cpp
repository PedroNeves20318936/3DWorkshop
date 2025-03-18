#include "Cube.h"

using namespace std;
using namespace glm;

static float positionArray[] = {
    -1.0f,  1.0f, -1.0f, 1.0f,
     1.0f,  1.0f, -1.0f, 1.0f,
     1.0f,  1.0f,  1.0f, 1.0f,
    -1.0f,  1.0f,  1.0f, 1.0f,

    -1.0f, -1.0f, -1.0f, 1.0f,
     1.0f, -1.0f, -1.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 1.0f,
    -1.0f, -1.0f,  1.0f, 1.0f,

    -1.0f, -1.0f,  1.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 1.0f,
     1.0f,  1.0f,  1.0f, 1.0f,
    -1.0f,  1.0f,  1.0f, 1.0f,

    -1.0f, -1.0f, -1.0f, 1.0f,
     1.0f, -1.0f, -1.0f, 1.0f,
     1.0f,  1.0f, -1.0f, 1.0f,
    -1.0f,  1.0f, -1.0f, 1.0f,

    -1.0f, -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f,  1.0f, 1.0f,
    -1.0f,  1.0f,  1.0f, 1.0f,
    -1.0f,  1.0f, -1.0f, 1.0f,

     1.0f, -1.0f, -1.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 1.0f,
     1.0f,  1.0f,  1.0f, 1.0f,
     1.0f,  1.0f, -1.0f, 1.0f
};

static float colourArray[] = {
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,

    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,

    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,

    1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f,

    1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 1.0f,

    0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f
};

static unsigned int indexArray[] = {
    0, 3, 2,  2, 1, 0,
    4, 5, 6,  6, 7, 4,
    8, 9,10, 10,11, 8,
   12,15,14, 14,13,12,
   16,17,18, 18,19,16,
   20,23,22, 22,21,20
};



Cube::Cube()
    : InternalGameObj(positionArray, sizeof(positionArray),
        colourArray, sizeof(colourArray),
        indexArray, sizeof(indexArray),
        6 * 2) {
}

Cube::~Cube() {}
