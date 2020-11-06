#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "./bmp_struct.h"

int main(int argc, char** argv) {
	while(1) {
		puts("10");
	}

	FILE* file;
	char* o_filename = "./res/103_101.bmp";
	if(open_file(&file, o_filename, "rb") != OPEN_OK) {
		exit(OPEN_ERR);
	}

	struct bmp_header* header = (struct bmp_header*) malloc(sizeof(struct bmp_header));
	
	if(read_header(file, header) != READ_OK) {
		exit(READ_INVALID_HEADER);
	}

	fseek(file, header -> bOffBits, SEEK_SET);

	struct image* image = read_image(file, header -> biHeight, header -> biWidth);

	fclose(file);

	char* s_filename = "./res/1_1-11.bmp";

	if(open_file(&file, s_filename, "wb") != OPEN_OK) {
		exit(OPEN_ERR);
	}

	header = rotate_header(header);
	image = rotate_left(image);
	save_bmp(file, header, image);
	fclose(file); 
	return 0;
}