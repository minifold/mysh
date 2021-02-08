#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct commands {
    char * command;
    int commandnum;
}commands;

char * initshell()
{
    printf("Please create a username:\n");
    scanf()
}


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
    char cwd[1000];
    getcwd(cwd, sizeof(cwd));
    char* tmp = dir;
    char* dirs[dirnum];
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
    user = 
    prompt(user, cwd, 3);

    return 0;
}

