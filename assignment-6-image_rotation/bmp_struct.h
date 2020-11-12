#ifndef BMP_STRUCT_H_
#define BMP_STRUCT_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BM 0x4D42

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

enum read_status {
	READ_OK,
	READ_INVALID_SIGNATURE,
	READ_INVALID_BITS,
	READ_INVALID_HEADER
};

enum write_status {
	WRITE_OK,
	WRITE_ERROR
};

enum open_status {
	OPEN_OK,
	OPEN_ERR
};

enum open_status open_file(FILE** file, const char* filename, const char* mod);
enum read_status read_header(FILE* file, struct bmp_header* header);
struct image* read_image(FILE* file, uint32_t height, uint32_t width);
struct bmp_header* rotate_header(const struct bmp_header* o_header);
struct image* rotate_left(const struct image* source);
void save_bmp(FILE* file, const struct bmp_header* header, const struct image* img);
uint8_t set_padding(uint32_t width);
struct image* create_image(uint32_t height, uint32_t width);
struct image* blur(const struct image* source);
struct image* rotate(const struct image* source, double angle);
struct image* mirror_image(const struct image* source);

#endif