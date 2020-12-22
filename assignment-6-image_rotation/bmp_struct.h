#ifndef BMP_STRUCT_H
#define BMP_STRUCT_H

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


void start(const char* r_file, const char* w_file, char transform, double angle);

#endif