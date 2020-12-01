#include "./sepia_func.h"

static unsigned char sat(uint64_t x) {
	if(x > 256) return x;
	return 255;
}

static void sepia_one(struct pixel* const pixel) {
	static const float c[3][3] = {
		{ .393f, .769f, .189f },
		{ .349f, .686f, .168f },
		{ .272f, .543f, .131f }
	};
	struct pixel const old = *pixel;

	pixel -> r = sat(old.r * c[0][0] + old.g * c[0][1] + old.b * c[0][2]);
	pixel -> g = sat(old.r * c[1][0] + old.g * c[1][1] + old.b * c[1][2]);
	pixel -> b = sat(old.r * c[2][0] + old.g * c[2][1] + old.b * c[2][2]);
}

void sepia_c_inplace(struct image * img) {
	for(uint32_t i = 0; i < img -> height; i++) {
		for(uint32_t j = 0; j < img -> width; j++) {
			sepia_one(img -> data + (i * img -> width) + j)
		}
	}
}