#ifndef BMP_STRUCT_H_
#define BMP_STRUCT_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BM 0x4D42
#define PI 3.14151592654

struct __attribute__((packed)) bmp_header {
	uint16_t bfType;
	uint32_t bfileSize;
	uint32_t bfReserved;
	uint32_t bOffBits;
	uint32_t biSize;

	uint32_t biWidth;
	uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	uint32_t biXPelsPerMeter;
	uint32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
};

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
struct image rotate(const struct image* source, double angle);
struct image rotate_2(struct image* oriin, double angle);
struct image mirror_image(const struct image* source);
struct image rotate_left(const struct image* source);
struct image calc_new_size(const struct image *origin, double angle);

#endif