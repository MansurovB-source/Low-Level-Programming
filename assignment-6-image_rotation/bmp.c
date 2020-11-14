#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "./bmp_struct.h"
#include "./bmp_reader.h"

void read_bmp_file(const char* r_file, const char* w_file, char mode, double angle) {
	FILE* file;
	if(open_file(&file, r_file, "rb") != OPEN_OK) {
		puts("Something wrong with read file");
		exit(OPEN_ERR);
	}

	struct bmp_header header;
	if(read_header(file, &header) != READ_OK) {
		puts("Invalid header");
		exit(READ_INVALID_HEADER);
	}

	fseek(file, header.bOffBits, SEEK_SET);
	
	struct image image = read_image(file, header.biHeight, header.biWidth);
	fclose(file);

	if(open_file(&file, w_file, "wb") != OPEN_OK) {
		puts("Somethign wrong with write file");
		exit(OPEN_ERR);
	}
	if(mode == 'a') {
		image = rotate(&image, angle);
	} else if(mode == 'm') {
		image = mirror_image(&image);
	} else {
		image = blur(&image);
	}
	header = rotate_header(&header, image.height, image.width);
	save_bmp(file, &header, image);
	
	fclose(file);
}
 
enum open_status open_file(FILE** file, const char* filename, const char* mod) {
	if((*file = fopen(filename, mod)) == NULL) return OPEN_ERR;
	return OPEN_OK;
}

enum read_status read_header(FILE* file, struct bmp_header* header) {
	fread(header, sizeof(struct bmp_header), 1, file);

	if(header ->bfType != BM) {
		return READ_INVALID_SIGNATURE;
	} else if(header -> biBitCount != 24) {
		return READ_INVALID_BITS;
	} else {
		return READ_OK;
	}
}

 struct image read_image(FILE* file, uint32_t height, uint32_t width) {
	struct image img = create_image(height, width);
	uint8_t padding = set_padding(width);
	for(uint32_t i = 0; i < img.height; i++) {
		fread(img.data + i * img.width, sizeof(struct pixel), img.width, file);
		fseek(file, padding, SEEK_CUR);
	}
	return img;
}

struct bmp_header rotate_header(const struct bmp_header* o_header, uint32_t height, uint32_t width) {
	//struct bmp_header* new_header = (struct bmp_header*) malloc(sizeof(struct bmp_header));
	struct bmp_header new_header;
	new_header.bfType = BM;
	new_header.biBitCount = 24;
	new_header.biWidth = width;
	new_header.biHeight = height;
	uint8_t padding = set_padding(new_header.biWidth);
	new_header.bfileSize = sizeof(struct bmp_header) + (new_header.biWidth * sizeof(struct pixel) + padding) * new_header.biHeight;
	new_header.bfReserved = 0;
	new_header.bOffBits = sizeof(struct bmp_header);
	new_header.biSize = 40;
	new_header.biPlanes = 1;
	new_header.biCompression = 0;
	new_header.biSizeImage = (new_header.biWidth * sizeof(struct pixel) + padding) * new_header.biHeight;
	new_header.biXPelsPerMeter = 0;
	new_header.biYPelsPerMeter = 0;
	new_header.biClrUsed = 0;
	new_header.biClrImportant = 0;

	return new_header;
}

void save_bmp(FILE* file, const struct bmp_header* header, const struct image img) {	
	uint8_t padding = set_padding(header -> biWidth);
	fwrite(header, sizeof(struct bmp_header), 1, file);
	for(uint32_t i = 0; i < img.height; i++) {
		fwrite(img.data + i * img.width, sizeof(struct pixel), img.width, file);
		fwrite(&padding, 1, padding, file);	
	}
}