#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "./bmp_struct.h"
#define PI 3.14151592654

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
	struct image* img = create_image(height, width);
	uint8_t padding = set_padding(width);
	img -> data = (struct  pixel*) malloc(img -> height * (img -> width * sizeof(struct pixel)));
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

	uint8_t padding = set_padding(new_header -> biWidth);

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
	struct image* new_image = create_image(source -> height, source -> width);
 	for(uint32_t i = 0; i < new_image -> height; i++) {
 		for(uint32_t j = 0; j < new_image -> width; j++) {
 			*(new_image -> data + (i * new_image -> width) + j) = *(source -> data + ((new_image -> width - j - 1) * new_image -> height) + i);
 		}
 	}
 	return new_image;
}

struct image* mirror_image(const struct image* source) {
	struct image* new_image = create_image(source -> height, source -> width);
 	for(uint32_t i = 0; i < new_image -> height; i++) {
 		for(uint32_t j = 0; j < (new_image -> width / 2); j++) {
 			*(new_image -> data + (i * new_image -> width) + j) = *(source -> data + (i * new_image -> width) + (new_image -> width - 1 - j));
 			*(new_image -> data + (i * new_image -> width) + (new_image -> width - 1 - j)) = *(source -> data + (i * new_image -> width) + j);
 		}
 	}

 	if(new_image -> width / 2 != 0) {
 		for(uint32_t i = 0; i < new_image -> height; i++) {
 			*(new_image -> data + (i * new_image -> width) + (int) new_image -> width + 1) = 
 			*(source -> data + (i * new_image -> width) + (int) new_image -> width + 1);
 		}
 	}
 	return new_image;
}

struct image* blur(const struct image* source) {
	struct image* new_image = create_image(source -> height, source -> width);
 	uint16_t b = 0;
 	uint16_t g = 0;
 	uint16_t r = 0;
 	for(uint32_t i = 1; i < new_image -> height - 1; i++) {
 		for(uint32_t j = 1; j < new_image -> width - 1; j++) {
 			
 			for(int32_t s_i = -1; s_i < 2; s_i++) {
 				for (int32_t s_j = -1; s_j < 2; s_j++) {
 					b += (source -> data + ((i + s_i) * new_image -> width) + (j + s_j)) -> b;
 					g += (source -> data + ((i + s_i) * new_image -> width) + (j + s_j)) -> g;
 					r += (source -> data + ((i + s_i) * new_image -> width) + (j + s_j)) -> r;
 				}
 			}

 			(new_image -> data + (i * new_image -> width) + j) -> b = b / 9;
 			(new_image -> data + (i * new_image -> width) + j) -> g = g / 9;
 			(new_image -> data + (i * new_image -> width) + j) -> r = r / 9;

 			b = 0;
 			g = 0;
 			r = 0;

 		}
 	}

 	for(uint32_t i = 0 ; i < new_image -> width; i++) {
 		*(new_image -> data + i) = *(source -> data + i);
 		*(new_image -> data + ((new_image -> height - 1) * new_image -> width) + i) = *(source -> data + ((new_image -> height - 1) * new_image -> width) + i);
 	}

 	for(uint32_t i = 1; i < new_image -> height; i++) {
 		*(new_image -> data + (i * new_image -> width)) = *(source -> data + (i * new_image -> width));
 		*(new_image -> data + (i * new_image -> width) + (new_image -> width - 1)) = *(source -> data + (i * new_image -> width) + (new_image -> width - 1));

 	}
 	return new_image;
}

struct image* rotate(const struct image* source, double angle) {
	struct image* new_image = create_image(source -> height, source -> width);
	
	// Define the center of the image
	int64_t horizontal_center = (int) source -> width / 2;
	int64_t vertical_center = (int) source -> height / 2;
	//printf("%u %u\n", horizontal_center, vertical_center);
	uint64_t s = 0;
	float angle_on_radian = angle * (double) PI / 180;
	float cos_a = cos(angle_on_radian);
	float sin_a = sin(angle_on_radian);
	//printf("%f %f %d\n", cos_a, sin_a, horizontal_center);

	//printf("%f---------\n", (float) (cos_a * (63 - horizontal_center)));
	for(uint32_t i = 0; i < source -> height; i++) {
		for(uint32_t j = 0; j < source -> width; j++) {
			float horizontal_position = (float) (cos_a * (j - horizontal_center) + sin_a * (i - vertical_center) + horizontal_center);
			float vertical_position = (float)  (cos_a * (i - vertical_center) - sin_a * (j - horizontal_center) + vertical_center);

			int32_t x = floor (horizontal_position);
			int32_t y = floor (vertical_position);

			//printf("%u %u %u %u\n",y, i, x, j);

			if(x >= 0 && x < new_image -> width && y >= 0 && y < new_image -> height) {
				*(new_image -> data + (i * new_image -> width) + j) = *(source -> data + (y * source -> width) + x);
			}
		}
	}
	return new_image;
}

void save_bmp(FILE* file, const struct bmp_header* header, const struct image* img) {	
	uint8_t padding = set_padding(header -> biWidth);
	fwrite(header, sizeof(struct bmp_header), 1, file);
	for(uint32_t i = 0; i < img -> height; i++) {
		fwrite(img -> data + i * img -> width, sizeof(struct pixel), img -> width, file);
		fwrite(&padding, 1, padding, file);	
	}
}

uint8_t set_padding(uint32_t width) {
	uint8_t padding = (4 - (width * sizeof(struct pixel)) % 4) % 4;
	return padding;
}

struct image* create_image(uint32_t height, uint32_t width) {
	struct image* new_image = (struct image*) malloc(sizeof(struct image));
  	new_image -> height = height;
 	new_image -> width = width;
 	new_image -> data = (struct pixel*) malloc(new_image -> height * new_image -> width * sizeof(struct pixel));
 	return new_image;
}