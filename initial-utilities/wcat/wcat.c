//coded by onval
//wcat utility - it's like cat, but I wrote it
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

	FILE *fp;
	int i;
	char file_buf[256];

	if (argc == 1)
		return 0;
// foreach argv -> print file content
	for (i = 1; i < argc; i++) {
		fp = fopen(argv[i], "r");

		if (!fp) {
			printf("wcat: cannot open file\n");
			return 1;
		}

		while(fgets(file_buf, sizeof file_buf, fp) != NULL) {
			printf("%s", file_buf);
		}
	}

	return 0;	
}
