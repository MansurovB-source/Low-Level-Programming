#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "./bmp_struct.h"

int main(int argc, char** argv) {
	FILE* file;
	char* o_filename = "./res/p.bmp";
	if(open_file(&file, o_filename) != OPEN_OK) {
		exit(OPEN_ERR);
	}

	struct bmp_header* header = (struct bmp_header*) malloc(sizeof(struct bmp_header));
	
	if(read_header(file, header) != READ_OK) {
		exit(READ_INVALID_HEADER);
	}

	fseek(file, header -> bOffBits, SEEK_SET);

	struct image* image = read_image(file, header -> biHeight, header -> biWidth);

	fclose(file);

	FILE* s_file;
	char* s_filename = "./res/picture-1.bmp";

	file = fopen(s_filename, "wb");

	// if(open_file(&s_file, s_filename) != OPEN_OK) {
	// 	exit(OPEN_ERR);
	// }

	header = rotate_header(header);
	image = rotate_left(image);
	save_bmp(file, header, image);
	fclose(file); 
	return 0;
}