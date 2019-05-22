#include <stdio.h>

int main(int argc, char **argv) {
	if (argc == 1) {
		puts("wzip: file1 [file2 ...]");
		return 1;
	}

	int i;
	FILE *fp;

	int n = 0;
	char c = '\0';
	char prev = '\0';

	for (i=1; i < argc; i++) {
		fp = fopen(argv[i], "r");

		if (!fp) {
			printf("Cannot open file");
			return 1;
		}

		if (!fread(&prev, 1, 1, fp))
			break;
		
		++n;

		while(fread(&c, 1, 1, fp)) {

			if (c != prev) {
				fwrite(&n, 4, 1, stdout);
				fwrite(&prev, 1, 1, stdout);
				n = 1;
				prev = c;
			} else ++n;
		}
	}

	fclose(fp);

	if (n > 0) {
		fwrite(&n, 4, 1, stdout);
		fwrite(&c, 1, 1, stdout);
	}

	return 0;
}
