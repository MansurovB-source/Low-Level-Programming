#ifndef IMAGE_STRUCT_H
#define IMAGE_STRUCT_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
struct __attribute__(( packed )) pixel {
	uint8_t b;
	uint8_t g;
	uint8_t r;

};

struct image {
	uint32_t height;
	uint32_t width;
	struct pixel* data;
};

uint8_t set_padding(uint32_t width);
struct image create_image(uint32_t height, uint32_t width);
struct image blur(const struct image* source);
struct image rotate_with_cutting(const struct image* source, double angle);
struct image rotate(struct image* oriin, double angle);
struct image mirror_image(const struct image* source);
struct image rotate_left(const struct image* source);

#endif