#include <sys/time.h>
#include <sys/resource.h>
#include "./sepia_func.h"

struct image create_image(uint32_t height, uint32_t width);

static uint8_t sat(uint64_t x) {
	if(x < 256) return x;
	return 255;
}

static void sepia_one(const struct pixel* old, struct pixel* new_pixel) {
	static const float c[3][3] = {
		{ .393f, .769f, .189f },
		{ .349f, .686f, .168f },
		{ .272f, .543f, .131f }
	};

	new_pixel -> r = sat((old -> r * c[0][0] + old -> g * c[0][1] + old -> b * c[0][2]));
	new_pixel -> g = sat((old -> r * c[1][0] + old -> g * c[1][1] + old -> b * c[1][2]));
	new_pixel -> b = sat((old -> r * c[2][0] + old -> g * c[2][1] + old -> b * c[2][2]));
}

struct image sepia_c_inplace(struct image * img) {
	struct image new_image = create_image(img -> height, img -> width);
	for(uint32_t i = 0; i < img -> height; i++) {
		for(uint32_t j = 0; j < img -> width; j++) {
			sepia_one(img -> data + (i * img -> width) + j, new_image.data + (i * new_image.width) + j);
		}
	}
	return new_image;
}

struct image sepia_sse(struct image* img) {
	struct image new_image = create_image(img -> height, img -> width);
	float rgb_b[4];
	float rgb_g[4];
	float rgb_r[4];
	uint8_t result[12];
	struct pixel* pixel;

	for(uint64_t i = 0; i < img -> height * img -> width; i += 4) {
		for(uint8_t j = 0; j < 4; j++) {
			pixel = (img -> data + i + j);
			rgb_b[j] = pixel -> b;
			rgb_g[j] = pixel -> g;
			rgb_r[j] = pixel -> r;			
		}

		sepia_asm(rgb_b, rgb_g, rgb_r, result);

		for(uint8_t k = 0; k < 4; k++) {
			pixel = (new_image.data + i + k);
			pixel -> b = *(result + 3 * k);
			pixel -> g = *(result + 3 * k + 1);
			pixel -> r = *(result + 3 * k + 2);
		}
	}
	return new_image;
}

void test_func(struct image* img) {
	puts("test ...\n");
	struct rusage r;
	struct timeval start;
	struct timeval end;
	int64_t res_1;
	int64_t res_2;

	getrusage(RUSAGE_SELF, &r);
	start = r.ru_utime;

	for(uint64_t i = 0; i < 100; i++) {
		sepia_c_inplace(img);
	}
	getrusage(RUSAGE_SELF, &r);
	end = r.ru_utime;

	res_1 = ((end.tv_sec - start.tv_sec) * 1000000L) + end.tv_usec - start.tv_usec;
	printf("Time elapsed in microseconds (on C): %ld\n", res_1);



	getrusage(RUSAGE_SELF, &r);
	start = r.ru_utime;

	for(uint64_t i = 0; i < 100; i++) {
		sepia_sse(img);
	}
	getrusage(RUSAGE_SELF, &r);
	end = r.ru_utime;

	res_2 = ((end.tv_sec - start.tv_sec) * 1000000L) + end.tv_usec - start.tv_usec;
	printf("Time elapsed in microseconds (on ASM with SSE): %ld\n", res_2);

	printf("Diff between C and ASM: %ld\n", res_1 - res_2);
}

// Time elapsed in microseconds (on C): 2967980
// Time elapsed in microseconds (on ASM with SSE): 1768602
// Diff between C and ASM: 1199378
