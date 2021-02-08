#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

int main (int argc, char** argv)
{
    if (argc > 1 && fgets() == "-d")
        rl_bind_key('\t', rl_insert);
}