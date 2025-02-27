#ifndef GENERAL_H
#define GENERAL_H

#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "liste_block.h"

#define HEIGHT 100
#define WIDTH 100

#define window_width 1280
#define window_height 960

int inMat(int x, int y);
int inMat2(int x, int y, int maxx, int maxy);
#endif