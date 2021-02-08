#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct commands {
    char * command;
    int commandnum;
}commands;


void addtohistory(char * buffer)
{
    return;
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

void prompt( char * user, char dir[], int dirnum)
{
    char* tmp = dir;
    char[3][]
    // get the first token
    tmp = strtok(tmp, "/");
    printf("world\n");
    while(tmp)
    {
         tmp);
        tmp = strtok(NULL, "/");
    }
    return;
}

int main(void)
{
    char cwd[1000];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s\n", cwd);
    };
    prompt("me", cwd, 3);

    return 0;
}

