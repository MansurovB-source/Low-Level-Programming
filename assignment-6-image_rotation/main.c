#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void transform(const char* r_file, const char* w_file, char* mode, char transform, double angle);

int main(int argc, char** argv) {
	int rez = 0;
	char trans = 'a';
	char* mode;
	int angle = 45;
	puts("-----start-----");
	while((rez = getopt(argc, argv, "m:t:a:")) != -1) {
		switch(rez) {
			case 'm': 
				mode = optarg;
				printf("Found argument \"m = %s\".\n", optarg);
				break;
			case 't': 
				trans = *optarg;
				printf("Found argument \"t = %s\".\n", optarg);
				break;
			case 'a':
				angle = atoi(optarg);
				printf("Found argument \"a = %s\".\n", optarg);
				break;
			case '?':
				printf("Error found!\n");
				break;
		}
	}
	transform("./res/p.bmp", "./res/p1.bmp", mode, trans, (double) angle);
	puts("-----end-----");
	return 0;
}