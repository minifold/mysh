#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

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

void prompt( char * user, char * dir)
{
    char* cwd[1000];
    getcwd(cwd, sizeof(cwd));
    

}

int main(void)
{
    while(1) 
    {
        print
    }
}

void addtohistory(char * buffer)
{
    return;
}