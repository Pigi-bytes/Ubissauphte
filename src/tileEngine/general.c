#include "general.h"

int inMat(int x, int y) {
    return x >= 0 && x < HEIGHT && y >= 0 && y < WIDTH;
}

int inMat2(int x, int y, int maxx, int maxy) {
    return x >= 0 && x < maxx && y >= 0 && y < maxy;
}