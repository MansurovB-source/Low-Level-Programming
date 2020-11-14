#ifndef BMP_READER_H_
#define BMP_READER_H_

#include <stdio.h>
#include <stdlib.h>
#include "./bmp_struct.h"

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

void read_bmp_file(const char* r_file, const char* w_file, char mode, double angle);
enum open_status open_file(FILE** file, const char* filename, const char* mod);
enum read_status read_header(FILE* file, struct bmp_header* header);
struct image read_image(FILE* file, uint32_t height, uint32_t width);
struct bmp_header rotate_header(const struct bmp_header* o_header, uint32_t height, uint32_t width);
void save_bmp(FILE* file, const struct bmp_header* header, const struct image img);

#endif