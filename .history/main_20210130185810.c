#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct commands {
    char * command;
    int commandnum;
}commands;

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
        puts();
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

void addtohistory(char * buffer)
{
    return;
}

void prompt( char * user, char * dir, int dirnum)
{
    char * token;
    char * tmp = malloc(strlen(dir) + 1); 
    strcpy(tmp, dir);
    while ((token = strtok(&tmp, "/"))) foo(token);

    return;
}

foo( char * token)
{
    printf(token);
}

int main(void)
{
    prompt("me", "/hello/", 3);

    return 0;
}

