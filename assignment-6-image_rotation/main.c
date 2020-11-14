#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "./bmp_reader.h"
#include "./bmp_struct.h"

int main(int argc, char** argv) {
	int rez = 0;
	char mode = 'a';
	int angle = 45;
	while((rez = getopt(argc, argv, "m:a:")) != -1) {
		switch(rez) {
			case 'm': 
				mode = *optarg;
				printf("Found argument \"m = %s\".\n", optarg);
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
	puts("-----start-----");
	read_bmp_file("./res/p.bmp", "./res/p1.bmp", mode, (double) angle);
	puts("-----end-----");
	return 0;
}