#include "commands.h"

/* Register / declare commands
************************************/

char *builtin_str[] = {
	"cd",
	"echo",
	"help",
	"exit"
};

int (*builtin_func[]) (char **) = {
	&cli_cd,
	&cli_echo,
	&cli_help,
	&cli_exit
};

int cli_num_builtins() {
	return sizeof(builtin_str) / sizeof(char *);
}

/* Builtin function implementations.
************************************/

/************************************************************************ 
   @brief Bultin command: change directory.
   @param args List of args.  args[0] is "cd".  args[1] is the directory.
   @return Always returns 1, to continue executing.
*************************************************************************/
int cli_cd(char **args) {
	if (args[1] == NULL) {
		fprintf(stderr, "cli: expected argument to \"cd\"\n");
	} else {
		if (chdir(args[1]) != 0) {
			perror("cli");
		}
	}
	return 1;
}


/**************************************************
   @brief Builtin command: echo.
   @param args List of args.  Not examined.
   @return Always returns 1, to continue executing.
***************************************************/

int cli_echo(char **args) {
	if (args[1] == NULL) {
		fprintf(stderr, "cli: expected argument to \"echo\"\n");
	} else {
		printf("%s\n", args[1]);
	}
	return 1;
}


/**************************************************
   @brief Builtin command: print help.
   @param args List of args.  Not examined.
   @return Always returns 1, to continue executing.
***************************************************/

int cli_help(char **args) {
	int i;
	printf("Type program names and arguments, and hit enter.\n");

	for (i = 0; i < cli_num_builtins(); i++) {
		printf("  %s\n", builtin_str[i]);
	}

	printf("Use the man command for information on other programs.\n");
	return 1;
}


/***************************************************
   @brief Builtin command: exit.
   @param args List of args.  Not examined.
   @return Always returns 0, to terminate execution.
 ***************************************************/

int cli_exit(char **args) {
	return 0;
}

