#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

typedef struct commands {
    char * command;
    int commandnum;
}commands;

char * initshell()
{
    char * user = malloc(sizeof(char) * 1024);
    puts("Please create a username: ");
    Fgets(user);
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

void prompt(char * user)
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

    fprintf(stdout, GREEN "%s:" RESET BLUE "%s" RESET "# ", user, dirprompt);
    return;
}

int main()
{
    char * user = initshell();
    prompt(user);

    return 0;
}

