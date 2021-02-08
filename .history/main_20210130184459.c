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

void prompt( char * user, char * dir, int dirnum)
{
    char ** dirs;
    char * tmp;
    char * token;
    int count = 0;
    while (*tmp)
    {
        if (dir == *tmp)
        {
            count++;
            token = tmp;
        }
        tmp++;
    }

    while (dir) 
    {
        strtok(dir, "/");
    
    

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