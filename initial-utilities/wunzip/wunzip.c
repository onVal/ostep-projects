#include <stdio.h>

int main(int argc, char **argv) {
	if (argc == 1) {
		puts("wunzip: file1 [file2 ...]");
		return 1;
	}

	FILE *fp;
	
	int num; 
	char ch;

	for (int i=1; i < argc; i++) {
		fp = fopen(argv[1], "r");

		if (!fp) {
			printf("Cannot open file\n");
			return 1;
		}

		while (!feof(fp)) {
			fread(&num, 4, 1, fp);
			fread(&ch, 1, 1, fp);

			if (!feof(fp)) {
				for (int j=0; j < num; j++) {
					putchar(ch);
				}
			}
		}
	}

	return 0;
}
