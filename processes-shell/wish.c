#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

#define BUF_SIZE 50

int getsize(char array[]);
void buildin(char *buffer, buildin);

typedef enum buildin {
	NOBUILDIN, EXIT, CD, PATH}
buildin;

int main(int argc, char **argv) {
	int i = 0, ch_read;
	unsigned long n = 0;
	int pid;
	char *buf = NULL;

	char *token;
	char *path[] = {"/bin/", NULL};
	FILE *fs;

	if (argc > 2) {
		perror("Usage: ./wish [batch_file]");
		exit(1);
	}

	if (argc == 2)
		fs = fopen(argv[1], "r");

	while (1) {
		//input
		if (argc == 1) {
			printf("wish> ");
			ch_read = getline(&buf, &n, stdin);
		} else {
			ch_read = getline(&buf, &n, fs);
		}

		if (ch_read == -1)
			if (argc != 1 && feof(fs))
				exit(0);

		//remove newline at the end
		buf[getsize(buf)] = '\0';

		char *cmd = strtok(buf, " ");

		//buildin
		if (strcmp(cmd, "exit") == 0) {
			exit(0);
		} else if (strcmp(cmd, "cd")) {
			builtin(buf, builtin.CD);
			
		}
		


		if ((pid = fork()) < 0) {
			perror("Cannot fork!");
			exit(1);
		} else if (pid == 0) { //child
			char *args[BUF_SIZE] = {0};

			//add path
			char *bufpath = calloc(getsize(path[0]) + getsize(buf) + 1, sizeof(char *));
			memcpy(bufpath, path[0], getsize(path[0]));
			strcat(bufpath, buf);
			
			for (i=0; (args[i] = strsep(&bufpath, " ")) != NULL ; i++)
				;

			//cd built-in
			if (strcmp(args[0], "cd") == 0) {
				if (args[1] != NULL && args[2] == NULL) {
					if (chdir(args[1])) {
						exit(0);
					} else {
						perror("chdir() failed");
						exit(1);
					}
				} else {
					fprintf(stderr, "Wrong built-in syntax: > cd path");
				}
			}

			execv(args[0], args);
			exit(0);
		} else if (pid > 0) { //father
			wait(NULL);

			if (argc == 1) {
				if (feof(stdin)) exit(0);
			} else {
				if (feof(fs)) exit(0); 
			}
		}
	}

	return 0;
}

int getsize(char *array) {
	int i = 0;

	while (array[i] != '\n' && array[i] != '\0')
		i++;

	return i;
}

void buildin(char *buffer, buildin bi) {
	switch (bi) {
		case builtin.CD:


	}
} 