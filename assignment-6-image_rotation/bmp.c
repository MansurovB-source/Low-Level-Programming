#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "./bmp_struct.h"

enum open_status open_file(FILE** file, const char* filename, const char* mod) {
	if((*file = fopen(filename, mod)) == NULL) return OPEN_ERR;
	return OPEN_OK;
}

enum read_status read_header(FILE* file, struct bmp_header* header) {
	fread(header, sizeof(struct bmp_header), 1, file);

	if(header -> bfType != BM) {
		return READ_INVALID_SIGNATURE;
	} else if(header -> biBitCount != 24) {
		return READ_INVALID_BITS;
	} else {
		return READ_OK;
	}
}

 struct image* read_image(FILE* file, uint32_t height, uint32_t width) {
	struct image* img = (struct image*) malloc(sizeof(struct image));
	img -> height = height;
	img -> width = width;
	uint8_t padding = (width * sizeof(struct pixel)) % 4;
	img -> data = (struct  pixel*) malloc(img -> height * (img -> width * sizeof(struct pixel) + padding));
	for(uint32_t i = 0; i < img -> height; i++) {
		fread(img -> data + i * img -> width, sizeof(struct pixel), img -> width, file);
		fseek(file, padding, SEEK_CUR);
	}
	return img;
}

struct bmp_header* rotate_header(const struct bmp_header* o_header) {
	struct bmp_header* new_header = (struct bmp_header*) malloc(sizeof(struct bmp_header));
	
	new_header -> bfType = 0x4D42;
	new_header -> biBitCount = 24;
	new_header -> biWidth = o_header -> biHeight;
	new_header -> biHeight = o_header -> biWidth;

	uint8_t padding = (new_header -> biWidth * sizeof(struct pixel)) % 4;

	new_header -> bfileSize = sizeof(struct bmp_header) + (new_header -> biWidth * sizeof(struct pixel) + padding) * new_header -> biHeight;
	new_header -> bfReserved = 0;
	new_header -> bOffBits = sizeof(struct bmp_header);
	new_header -> biSize = 40;
	new_header -> biPlanes = 1;
	new_header -> biCompression = 0;
	new_header -> biSizeImage = (new_header -> biWidth * sizeof(struct pixel) + padding) * new_header -> biHeight;
	new_header -> biXPelsPerMeter = 0;
	new_header -> biYPelsPerMeter = 0;
	new_header -> biClrUsed = 0;
	new_header -> biClrImportant = 0;

	return new_header;
}

struct image* rotate_left(const struct image* source) {
	struct image* new_image = (struct image*) malloc(sizeof(struct image));
  	new_image -> height = source -> width;
 	new_image -> width = source -> height;
 	new_image -> data = (struct pixel*) malloc(new_image -> height * new_image -> width * sizeof(struct pixel));
 	for(uint32_t i = 0; i < new_image -> height; i++) {
 		for(uint32_t j = 0; j < new_image -> width; j++) {
 			*(new_image -> data + (i * new_image -> width) + j) = *(source -> data + ((new_image -> width - j - 1) * new_image -> height) + i);
 		}
 	}

 	return new_image;
}

void save_bmp(FILE* file, const struct bmp_header* header, const struct image* img) {	
	uint8_t padding = (header -> biWidth * sizeof(struct pixel)) % 4;
	fwrite(header, sizeof(struct bmp_header), 1, file);
	for(uint32_t i = 0; i < img -> height; i++) {
		fwrite(img -> data + i * img -> width, sizeof(struct pixel), img -> width, file);
		fseek(file, padding, SEEK_CUR);
	}
}