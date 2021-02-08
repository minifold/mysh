#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct commands {
    char * command;
    int commandnum;
}commands;


void addtohistory(char * buffer)
{
    return;
}

int read(char * args)
{
    char* buffer;
    // buffer = readline("\n# ");
    if (strlen(buffer) != 0)
    {
        addtohistory(buffer);
        // strcpy(args);
        return 0;
    }
    else {
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

void foo( char * token)
{
    printf("%s", token);
    return;
}

void prompt( char * user, char * dir, int dirnum)
{
    char * token = malloc(strlen(dir) + 1);
    strcpy(token, dir);

    // get the first token
    printf("hello\n");
    token = strtok(dir, "/");
    printf("world\n");
    while(token)
    {
        printf("[%s]\n", token);
        token = strtok(0, "/");
    }
    return;
}

int main(void)
{
    prompt("me", "/hello/world/its/me", 3);

    return 0;
}

