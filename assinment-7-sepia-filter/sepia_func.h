#ifndef SEPIA_FUNC_H
#define SEPIA_FUNC_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "./image_struct.h"

struct image sepia_c_inplace(struct image * img);
struct image sepia_sse(struct image* img);
void sepia_asm(float* rgb_b, float* rgb_g, float* rgb_r, uint8_t* result);
#endif