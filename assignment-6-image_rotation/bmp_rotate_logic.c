#include <stdint.h>
#include <math.h>
#include "./bmp_struct.h"

struct image rotate_left(const struct image* source) {
	struct image new_image = create_image(source -> height, source -> width);
 	for(uint32_t i = 0; i < new_image.height; i++) {
 		for(uint32_t j = 0; j < new_image.width; j++) {
 			*(new_image.data + (i * new_image.width) + j) = *(source -> data + ((new_image.width - j - 1) * new_image.height) + i);
 		}
 	}
 	return new_image;
}


struct image mirror_image(const struct image* source) {
	struct image new_image = create_image(source -> height, source -> width);
 	for(uint32_t i = 0; i < new_image.height; i++) {
 		for(uint32_t j = 0; j < (new_image.width / 2); j++) {
 			*(new_image.data + (i * new_image.width) + j) = *(source -> data + (i * new_image.width) + (new_image.width - 1 - j));
 			*(new_image.data + (i * new_image.width) + (new_image.width - 1 - j)) = *(source -> data + (i * new_image.width) + j);
 		}
 	}

 	if(new_image.width / 2 != 0) {
 		for(uint32_t i = 0; i < new_image.height; i++) {
 			*(new_image.data + (i * new_image.width) + (int) new_image.width + 1) = 
 			*(source -> data + (i * new_image.width) + (int) new_image.width + 1);
 		}
 	}
 	return new_image;
}


struct image blur(const struct image* source) {
	struct image new_image = create_image(source -> height, source -> width);
 	uint16_t b = 0;
 	uint16_t g = 0;
 	uint16_t r = 0;
 	for(uint32_t i = 1; i < new_image.height - 1; i++) {
 		for(uint32_t j = 1; j < new_image.width - 1; j++) {
 			
 			for(int32_t s_i = -1; s_i < 2; s_i++) {
 				for (int32_t s_j = -1; s_j < 2; s_j++) {
 					b += (source -> data + ((i + s_i) * new_image.width) + (j + s_j)) -> b;
 					g += (source -> data + ((i + s_i) * new_image.width) + (j + s_j)) -> g;
 					r += (source -> data + ((i + s_i) * new_image.width) + (j + s_j)) -> r;
 				}
 			}

 			(new_image.data + (i * new_image.width) + j) -> b = b / 9;
 			(new_image.data + (i * new_image.width) + j) -> g = g / 9;
 			(new_image.data + (i * new_image.width) + j) -> r = r / 9;

 			b = 0;
 			g = 0;
 			r = 0;
 		}
 	}

 	for(uint32_t i = 0 ; i < new_image.width; i++) {
 		*(new_image.data + i) = *(source -> data + i);
 		*(new_image.data + ((new_image.height - 1) * new_image.width) + i) = *(source -> data + ((new_image.height - 1) * new_image.width) + i);
 	}

 	for(uint32_t i = 1; i < new_image.height; i++) {
 		*(new_image.data + (i * new_image.width)) = *(source -> data + (i * new_image.width));
 		*(new_image.data + (i * new_image.width) + (new_image.width - 1)) = *(source -> data + (i * new_image.width) + (new_image.width - 1));

 	}
 	return new_image;
}


struct image rotate(const struct image* source, double angle) {

	struct image new_image = create_image(source -> height, source -> width);

	int64_t horizontal_center = round(source -> width / 2);
	int64_t vertical_center = round(source -> height / 2);

	float angle_on_radian = angle * (double) PI / 180;
	float cos_a = cos(angle_on_radian);
	float sin_a = sin(angle_on_radian);
	
	for(uint32_t i = 0; i < source -> height; i++) {
		for(uint32_t j = 0; j < source -> width; j++) {
			float horizontal_position = (float) (cos_a * (j - horizontal_center) + sin_a * (i - vertical_center) + horizontal_center);
			float vertical_position = (float)  (cos_a * (i - vertical_center) - sin_a * (j - horizontal_center) + vertical_center);

			int32_t x = round(horizontal_position);
			int32_t y = round(vertical_position);

			if(x >= 0 && x < new_image.width && y >= 0 && y < new_image.height) {
				*(new_image.data + (i * new_image.width) + j) = *(source -> data + (y * source -> width) + x);
			}
		}
	}
	return new_image;
}
 
struct image rotate_2(struct image* source, double angle) {
	float angle_on_radian = angle * (double) PI / 180;
	float cos_a = cos(angle_on_radian);
	float sin_a = sin(angle_on_radian);

	struct image tmp = calc_new_size(source, angle_on_radian);
	struct image new_image = create_image(tmp.height, tmp.width);

	float horizontal_center = (float)(source -> width / 2);
	float vertical_center = (float)(source -> height / 2);

	float x_center = (float)(new_image.width / 2);
	float y_center = (float)(new_image.height / 2);
	float r = sqrt(x_center * x_center + y_center * y_center);
	float b = atan2(1. * y_center, x_center);
	float cos_b = cos(b);
	float sina_b = sin(b);

	for(int32_t i = 0 - horizontal_center; i < new_image.width - horizontal_center; i++) {
		for(int32_t j = 0 - vertical_center; j < new_image.height - vertical_center; j++) {
			int32_t n_i = round(i * cos_a - j * sin_a + r * cos_b);
			int32_t n_j = round(i * sina_b + j * cos_a + r * sina_b);

			if(n_i < 2 * x_center && n_i >= 0 && n_j < 2 * y_center && n_j >= 0) {
				*(new_image.data + ((i + (int32_t)horizontal_center)* new_image.width) + j + (int32_t)vertical_center) = *(source -> data + (n_i * source -> width) + n_j); 
			}
		}
	}
	return new_image;
}
uint8_t set_padding(uint32_t width) {
	uint8_t padding = (4 - (width * sizeof(struct pixel)) % 4) % 4;
	return padding;
}

struct image create_image(uint32_t height, uint32_t width) {
	struct image new_image;
  	new_image.height = height;
 	new_image.width = width;
 	new_image.data = (struct pixel*) malloc(new_image.height * new_image.width * sizeof(struct pixel));
 	return new_image;
}

struct image calc_new_size(const struct image *source, double angle) {
	struct image new_image = *source;
	new_image.height = round(cos(angle) * source -> width + sin(angle) * source -> height);
	new_image.width = round(sin(angle) * source -> width + cos(angle) * source -> height);
}