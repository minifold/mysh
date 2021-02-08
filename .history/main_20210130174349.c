#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h

int read(char * args)
{
    char* buffer;
    buffer = readline("\n# ");
    if (strlen(buffer) != 0)
    {
        addtohistory(buffer);
        strcpy(args);
        return 0;
    }
    else {
        puts("")
        return 1;
    }
}

void help()
{
    puts("*** MySH ***"
    "\n============"
    "\nCommands supported:"
    "\nmovetodir"
    "\nwhereami"
    "\nhistory [-c]"
    "\nbyebye"
    "\nreplay <number>"
    "\nstart <program> [parameters]"
    "\nbackground <program> [parameters]"
    "\ndalek <PID>"
    "\nrepeat n command ...");
    
    return;
}