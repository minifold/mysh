#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

int main (int argc, char** argv)
{
    char * args = argv[1];
    if (argc > 1 && fgets(args, strlen(args + 1), ) == "-d")
        rl_bind_key('\t', rl_insert);

    char * buffer;
    while ((buffer = readline("# ")) != NULL)
        if (strlen(buffer) > 0)
            add_history(buffer);
    
    printf("%s\n", buffer);
    free(buffer);

    return 0;
}