#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// ANSI color formatting for a fun shell experience :)
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

#define MAXLETTERS 1024
#define MAXCOMMS 100
#define clear() printf("\033[H\033[J") 

typedef struct commands {
    char * command;
    int commandnum;
}commands;

char * initshell()
{   
    clear();
    puts("===============================\n"
        "                                \n"
        "         Welcome to MySH!       \n"
        "                                \n"
        "================================\n"
        "Written by Alex Cingoranelli.   \n"
        "This was written with Unix in   \n"
        "mind. It is not compatible with \n"
        "MSDOS.                        \n\n");   
    char * user = getenv("USER");
    printf("USER is: %s\n", user);
    // sleep(1);
    // clear();
    return user;
}


void addtohistory(char buffer[MAXCOMMS][MAXLETTERS], int numcomms)
{
    int i = 0;
    char ** history;
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

int readinput(char buffer[MAXCOMMS][MAXLETTERS])
{   
    char input[MAXLETTERS]; 
    int i = 0;

    while(scanf("%49[^ \t\n]%*c", input))
    {   
        if (strlen(input) > MAXLETTERS || i > MAXCOMMS)
            return 0;    
        strcpy(buffer[i], input);
        i++;
    }

    addtohistory(buffer, i);

    return 1;
}

int processcommand()
{
    return 0;
}

void prompt()
{   
    char cwd[MAXLETTERS];
    getcwd(cwd, sizeof(cwd));
    char* tmp = cwd;
    char* dirprompt, * host;
    // get the first token
    tmp = strtok(tmp, "/");

    while(tmp)
    {
        dirprompt = tmp;
        tmp = strtok(NULL, "/");
    }

    char * user = getenv("USER");
    gethostname(host, 1000);
    fprintf(stdout, GREEN "%s@%s: " RESET BLUE "%s " RESET "# ", user, machine, dirprompt);
    return;
}

int main()
{
    FILE *fp = fopen(".history", "a+");
    char buffer[MAXCOMMS][MAXLETTERS] = { '\0' };
    char input[MAXLETTERS], *args[MAXCOMMS];
    char * pipe[MAXCOMMS];

    int flag = 0;

    char * user = initshell();
    
    while(1)
    {
        prompt(user);

        if (readinput(buffer))
            continue;

        // process whatever is in the buffer            
        flag = processcommand(input, args, pipe);

    }

    fclose(fp);
    return 0;
}

