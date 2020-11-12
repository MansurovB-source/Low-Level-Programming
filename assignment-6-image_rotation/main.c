#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./bmp_struct.h"

int main(int argc, char** argv) {
	// while(1) {
	// 	puts("10");
	// }
	puts("--start--");
	
	FILE* file;
	char* o_filename = "./res/p.bmp";
	if(open_file(&file, o_filename, "rb") != OPEN_OK) {
		puts("f");
		exit(OPEN_ERR);
	}

	struct bmp_header* header = (struct bmp_header*) malloc(sizeof(struct bmp_header));
	
	if(read_header(file, header) != READ_OK) {
		puts("fs");
		exit(READ_INVALID_HEADER);
	}

	fseek(file, header -> bOffBits, SEEK_SET);
	puts("fss");

	struct image* image = read_image(file, header -> biHeight, header -> biWidth);
	puts("fsss");

	fclose(file);
	puts("fssss");
	
	char* s_filename = "./res/p1.bmp";

	if(open_file(&file, s_filename, "wb") != OPEN_OK) {
		puts("fsssss");
		exit(OPEN_ERR);
	}
	puts("fssssss");
	header = rotate_header(header);
	puts("fssssssa");
	// image = blur(image);
	image = rotate(image, 77);
	puts("fssssssaa");
	save_bmp(file, header, image);
	puts("fssssssaaa");
	fclose(file); 
	return 0;
}