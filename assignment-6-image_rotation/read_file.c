#include <stdio.h>
#include <string.h>
#include "bmp_struct.h"

void transform(const char* r_file, const char* w_file, char* mode, char transform, double angle) {
	if(strcmp(mode, "bmp") == 0) {
		start(r_file, w_file, transform, angle);
	} else {
		puts("We don't have any other type");
	}
}