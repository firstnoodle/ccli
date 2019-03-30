#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* Function Declarations for builtin shell commands:
****************************************************/
int ccli_cd(char **args);
int ccli_echo(char **args);
int ccli_help(char **args);
int ccli_exit(char **args);

/* List of builtin commands, followed by their corresponding functions.
***********************************************************************/
char *builtin_str[];
int (*builtin_func[]) (char **);
int ccli_num_builtins();
