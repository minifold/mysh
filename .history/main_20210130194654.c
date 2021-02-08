#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct commands {
    char * command;
    int commandnum;
}commands;

const char * initshell()
{
    char user[1024];
    printf("Please create a username:\n");
    scanf("%s", user);
    return user;
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

void prompt(const char * user)
{   
    char cwd[1000];
    getcwd(cwd, sizeof(cwd));
    char* tmp = cwd;
    char* dirprompt;
    // get the first token
    tmp = strtok(tmp, "/");
    printf("world\n");
    while(tmp)
    {
        dirprompt = tmp;
        tmp = strtok(NULL, "/");
    }

    printf("%s: %s #", user, dirprompt);
    return;
}

int main()
{
    const char * user = initshell();
    prompt(user);

    return 0;
}

