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
	puts("Hhhhhhhh");

	struct bmp_header* header = (struct bmp_header*) malloc(sizeof(struct bmp_header));
	
	if(read_header(file, header) != READ_OK) {
		exit(READ_INVALID_HEADER);
	}
	puts("Hhhhhhhh");

	fseek(file, header -> bOffBits, SEEK_SET);

	struct image* image = read_image(file, header -> biHeight, header -> biWidth);

	fclose(file);
	puts("Hhhhhhhh");

	FILE* s_file;
	char* s_filename = "./res/picture-1.bmp";

	file = fopen(s_filename, "wb");

	// if(open_file(&s_file, s_filename) != OPEN_OK) {
	// 	exit(OPEN_ERR);
	// }
	puts("Hhhhhhhheeeeeeeee");

	header = rotate_header(header);
	puts("Hhhhhhhheeeeeeeee1");
	image = rotate_left(image);
	puts("Hhhhhhhheeeeeeeee2");
	save_bmp(file, header, image);
	puts("Hhhhhhhheeeeeeeee3");
	fclose(file); 
	puts("Hhhhhhhheeeeeeeee4");
	return 0;
}