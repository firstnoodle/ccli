#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "commands.h"

/*******************************************************************
  @brief Launch a program and wait for it to terminate.
  @param args Null terminated list of arguments (including program).
  @return Always returns 1, to continue execution.
 *******************************************************************/
int ccli_launch(char **args) {
	pid_t pid;
	int status;

	pid = fork();
	if (pid == 0) {
		// Child process
		if (execvp(args[0], args) == -1) {
			perror("ccli");
		}
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		// Error forking
		perror("ccli");
	} else {
		// Parent process
		do {
			waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return 1;
}

/***************************************************************************
   @brief Execute shell built-in or launch program.
   @param args Null terminated list of arguments.
   @return 1 if the shell should continue running, 0 if it should terminate
 ***************************************************************************/
int ccli_execute(char **args) {
	int i;

	if (args[0] == NULL) {
		// An empty command was entered.
		return 1;
	}

	for (i = 0; i < ccli_num_builtins(); i++) {
		if (strcmp(args[0], builtin_str[i]) == 0) {
			return (*builtin_func[i])(args);
		}
	}

	return ccli_launch(args);
}


/******************************************
   @brief Read a line of input from stdin.
   @return The line from stdin.
 ******************************************/

#define LSH_RL_BUFSIZE 1024

char *ccli_read_line(void) {
	int bufsize = LSH_RL_BUFSIZE;
	int position = 0;
	char *buffer = malloc(sizeof(char) * bufsize);
	int c;

	if (!buffer) {
		fprintf(stderr, "ccli: allocation error\n");
		exit(EXIT_FAILURE);
	}

	while (1) {
		// Read a character
		c = getchar();

		if (c == EOF) {
			exit(EXIT_SUCCESS);
		} else if (c == '\n') {
			buffer[position] = '\0';
			return buffer;
		} else {
			buffer[position] = c;
		}
		position++;

		// If we have exceeded the buffer, reallocate.
		if (position >= bufsize) {
			bufsize += LSH_RL_BUFSIZE;
			buffer = realloc(buffer, bufsize);
			if (!buffer) {
				fprintf(stderr, "ccli: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}


/*************************************************
   @brief Split a line into tokens (very naively).
   @param line The line.
   @return Null-terminated array of tokens.
 *************************************************/

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

char **ccli_split_line(char *line) {
	int bufsize = LSH_TOK_BUFSIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token, **tokens_backup;

	if (!tokens) {
		fprintf(stderr, "ccli: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, LSH_TOK_DELIM);
	while (token != NULL) {
		tokens[position] = token;
		position++;

		if (position >= bufsize) {
			bufsize += LSH_TOK_BUFSIZE;
			tokens_backup = tokens;
			tokens = realloc(tokens, bufsize * sizeof(char*));
			if (!tokens) {
				free(tokens_backup);
				fprintf(stderr, "ccli: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, LSH_TOK_DELIM);
	}
	tokens[position] = NULL;
	return tokens;
}


/*********************************************
   @brief Loop getting input and executing it.
 *********************************************/

void ccli_loop(void) {
	char *line;
	char **args;
	int status;

	do {
		printf("> ");
		line = ccli_read_line();
		args = ccli_split_line(line);
		status = ccli_execute(args);

		free(line);
		free(args);
	} while (status);
}


/*******************************
   @brief Main entry point.
   @param argc Argument count.
   @param argv Argument vector.
   @return status code
 *******************************/

int main(int argc, char **argv) {
	// 1. Load config files, if any.

	// 2. Run command loop.
	ccli_loop();

	// 3. Perform any shutdown/cleanup.

	return EXIT_SUCCESS;
}

