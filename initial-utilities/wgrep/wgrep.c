#include <stdio.h>
#include <string.h>

int findWord(char *str, int len, char *buf);

int main(int argc, char **argv) {
	int i;
	int err = 0;
	char *buffer = NULL;
	size_t len = 0;

	if (argc == 1) {
		printf("wgrep: searchterm [file ...]\n");
		return 1;
	}

	if (argc == 2) {
		while(getline(&buffer, &len, stdin) != -1) {
			if (findWord(argv[1], strlen(argv[1]), buffer))
				printf("%s", buffer);
		}
	}

	if (argc > 2) {
		for (i = 2; i < argc; i++) {
			FILE *fp = fopen(argv[i], "r");
			if (fp == NULL) {
			   printf("wgrep: cannot open file\n");
		   	   return 1;
			}

			while (getline(&buffer, &len, fp) != -1) {
				if (findWord(argv[1], strlen(argv[1]), buffer))
					printf("%s", buffer);
			}
		}
	}		
}

// 1 if found, 0 otherwise
int findWord(char *str, int len, char *buf) {
	int i, j;
	
	for(i=0, j=0; buf[i] != '\0'; i++) {
		if (str[j] == buf[i]) {
			if (j == len-1) return 1;
			++j;
		} else {
			j = 0; 
			if (j > 0) --i;
		}
	}

	return 0;
}
