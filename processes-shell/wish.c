#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

#define BUF_SIZE 50

int getsize(char array[]);

int main(int argc, char **argv) {
	int i = 0, ch_read;
	unsigned long n = 0;
	int pid;
	char *buf = NULL;

	char *token;
	char error_msg[30] = "An error has occurred\n";

	char **path = malloc(2*sizeof(char *));
	path[0] = "/bin/";
	path[1] = NULL;

	FILE *fs;

	if (argc > 2) {
		fprintf(stderr, error_msg);
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
			if (argc != 1 && feof(fs)) {
				fprintf(stderr, error_msg);
				exit(1);
			}

		//remove newline at the end
		buf[getsize(buf)] = '\0';

		//put buf into tokens
		char *args[BUF_SIZE] = {0};

		char *file_out = NULL;

		for (i=0; (args[i] = strsep(&buf, " ")) != NULL; i++) {
			if (strcmp(args[i], ">") == 0) { //check for redirect symbol
				args[i] = NULL;
				file_out = strsep(&buf, " ");

				if (file_out != NULL) {
					break;
				} else {
					fprintf(stderr, error_msg);
					continue;
				}
			}
		}


		//buildin
		if (strcmp(args[0], "exit") == 0) { //exit builtin
			exit(0);
		} else if (strcmp(args[0], "cd") == 0) { //cd builtin
		 	if (args[1] != NULL && args[2] == NULL) {
		 		if (chdir(args[1]) == -1) {
	 				perror("chdir() failed");
				}
			} else {
		 		fprintf(stderr, error_msg);
			}

			continue;
		} else if (strcmp(args[0], "path") == 0) { //path builtin
			for(i=1; ((path[i-1] = args[i]) != NULL); i++)
				;
			continue;
		}

		if ((pid = fork()) < 0) {
			fprintf(stderr, error_msg);
			exit(1);
		} else if (pid == 0) { //child
			int path_size;
			int access_ret;
			char *cmd;

			if (file_out != NULL) {
				close(STDOUT_FILENO);
				close(STDERR_FILENO);
				open(file_out, O_WRONLY | O_CREAT, S_IRUSR);
			}

			//try path/binary existence
			for(i=0; path[i] != NULL; i++) {
				path_size = getsize(path[i]);
				cmd = calloc (path_size + getsize(args[0]) + 1, sizeof(char));
				memcpy(cmd, path[i], path_size);
				strcat(cmd, args[0]);

				if ((access_ret = access(cmd, X_OK)) == 0)
					execv(cmd, args);
			}

			if (access_ret == -1) {
				fprintf(stderr, error_msg);
				exit(1);
			}
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
