#include <stdint.h>
#include <math.h>
#include "./image_struct.h"

static uint32_t calc_new_height(const struct image* source, double angle);
static uint32_t calc_new_width(const struct image* source, double angle);
static void pixel_blur(struct pixel* const new_pixel, const struct pixel* source, int32_t width)

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

 	if(new_image.width % 2 != 0) {
 		for(uint32_t i = 0; i < new_image.height; i++) {
 			*(new_image.data + (i * new_image.width) + new_image.width + 1) = 
 			*(source -> data + (i * new_image.width) + new_image.width + 1);
 		}
 	}
 	return new_image;
}

void pixel_blur(struct pixel* const new_pixel, const struct pixel* source, int32_t width) {
	uint16_t b = 0;
 	uint16_t g = 0;
 	uint16_t r = 0;

	for(int32_t i = -1; i < 2; i++) {
 		for (int32_t j = -1; j < 2; j++) {
 			b += (source + (i * width) + j) -> b;
 			g += (source + (i * width) + j) -> g;
 			r += (source + (i * width) + j) -> r;
		}
 	}

 			new_pixel -> b = b / 9;
 			new_pixel -> g = g / 9;
 			new_pixel -> r = r / 9;
}

struct image blur(const struct image* source) {
	struct image new_image = create_image(source -> height, source -> width);

 	// the inner part
 	for(uint32_t i = 1; i < new_image.height - 1; i++) {
 		for(uint32_t j = 1; j < new_image.width - 1; j++) {
 			pixel_blur((struct pixel*)(new_image.data + (i * new_image.width) + j), (struct pixel*)(source -> data + (i * new_image.width) + j), source -> width);
 		}
 	}

 	// the upper and lower part
 	for(uint32_t i = 0 ; i < new_image.width; i++) {
 		*(new_image.data + i) = *(source -> data + i);
 		*(new_image.data + ((new_image.height - 1) * new_image.width) + i) = *(source -> data + ((new_image.height - 1) * new_image.width) + i);
 	}
 	// the left and right part
 	for(uint32_t i = 1; i < new_image.height; i++) {
 		*(new_image.data + (i * new_image.width)) = *(source -> data + (i * new_image.width));
 		*(new_image.data + (i * new_image.width) + (new_image.width - 1)) = *(source -> data + (i * new_image.width) + (new_image.width - 1));

 	}
 	return new_image;
}


struct image rotate_with_cutting(const struct image* source, double angle) {

	struct image new_image = create_image(source -> height, source -> width);

	int64_t horizontal_center = round(source -> width / 2);
	int64_t vertical_center = round(source -> height / 2);

	float angle_on_radian = angle * (double) M_PI / 180;
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

struct image rotate(struct image* source, double angle) {
	float angle_on_radian = angle * (double) M_PI / 180;
	float cos_a = cos(angle_on_radian);
	float sin_a = sin(angle_on_radian);

	struct image new_image = create_image(calc_new_width(source, angle_on_radian), calc_new_height(source, angle_on_radian));

	float horizontal_center = (float)(new_image.width / 2);
	float vertical_center = (float)(new_image.height / 2);

	float x_center = (float)(source -> width / 2);
	float y_center = (float)(source -> height / 2);
	
	for(int32_t x = 0 - horizontal_center; x < new_image.width - horizontal_center; x++) {
		for(int32_t y = 0 - vertical_center; y < new_image.height - vertical_center; y++) {
			int32_t n_x = round(x * cos_a + y * sin_a + x_center);
			int32_t n_y = round(y * cos_a - x * sin_a + y_center);

			if(n_x <= source -> width && n_x >= 0 && n_y < source -> width && n_y >= 0) {
				*(new_image.data + ((y + (int32_t)vertical_center)* new_image.width) + (x + (int32_t)horizontal_center)) = *(source -> data + (n_y * source -> width) + n_x); 
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
	struct image new_image = {height, width, (struct pixel*) malloc(height * width * sizeof(struct pixel))};
	return new_image;
}

static uint32_t calc_new_width(const struct image* source, double angle) {
	return round(fabs(sin(angle)) * source -> width + fabs(cos(angle)) * source -> height);
}

static uint32_t calc_new_height(const struct image* source, double angle) {
	return round(fabs(cos(angle)) * source -> width + fabs(sin(angle)) * source -> height);
	 
}