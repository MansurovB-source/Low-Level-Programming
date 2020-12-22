#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "./bmp_struct.h"
#include "./image_struct.h"

static enum read_status read_header(FILE* file, struct bmp_header* header);
static struct image read_image(FILE* file, uint32_t height, uint32_t width);
static struct bmp_header new_header(uint32_t height, uint32_t width);
static void save_bmp(FILE* file, const struct image img);
static struct image select_mode(struct image image, char mode, double angle);
static struct image read(const char* r_file);
static void write(const char* w_file, const struct image img, char transform, double angle);

enum open_status open_file(FILE** file, const char* filename, const char* mod);

void start(const char* r_file, const char* w_file, char transform, double angle) {
	struct image img =  read(r_file);
	write(w_file, img, transform, angle);
}

struct image read(const char* r_file) {
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
	return image;
}

void write(const char* w_file, const struct image img, char transform, double angle) {
	FILE* file;

	if(open_file(&file, w_file, "wb") != OPEN_OK) {
		puts("Somethign wrong with write file");
		exit(OPEN_ERR);
	}
	struct image image = select_mode(img, transform, angle);
	save_bmp(file, image);
	fclose(file);
}

struct image select_mode(struct image img, char transform, double angle) {
	if(transform == 'r') {
		return rotate(&img, angle);
	} else if(transform == 'm') {
		return mirror_image(&img);
	} else {
		return blur(&img);
	}
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

struct bmp_header new_header(uint32_t height, uint32_t width) {
	uint8_t padding = set_padding(width);
	struct bmp_header new_header = {
		BM, 
		sizeof(struct bmp_header) + (width * sizeof(struct pixel) + padding) * height, 
		0, 
		sizeof(struct bmp_header), 
		40, 
		width, 
		height, 
		1, 
		24, 
		0, 
		-1, 
		0, 
		0, 
		0, 
		0
	};
	return new_header;
}

void save_bmp(FILE* file, const struct image img) {	
	uint8_t padding = set_padding(img.width);
	struct bmp_header header = new_header(img.height, img.width);
	fwrite(&header, sizeof(struct bmp_header), 1, file);
	for(uint32_t i = 0; i < img.height; i++) {
		fwrite(img.data + i * img.width, sizeof(struct pixel), img.width, file);
		fwrite(&padding, 1, padding, file);	
	}
}