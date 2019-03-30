#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* Function Declarations for builtin shell commands:
****************************************************/
int cli_cd(char **args);
int cli_echo(char **args);
int cli_help(char **args);
int cli_exit(char **args);

/* List of builtin commands, followed by their corresponding functions.
***********************************************************************/
char *builtin_str[];
int (*builtin_func[]) (char **);
int cli_num_builtins();
