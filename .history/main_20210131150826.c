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
#define clear() printf("\033[H\033[J") 

typedef struct user{
    char * username;
    char * host;
    char * dir;
}user_t;

typedef struct commands {
    char * command;
    int commandnum;
    char description[MAXLETTERS];
}builtin;

typedef struct commands {
    char * command;
    int argc;
    char * argv;
}usercomms;

user_t initshell(user_t user)
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
    user.username = getenv("USER");
    user.host = malloc(sizeof(char) * MAXLETTERS);
    gethostname(user.host, sizeof(user.host));
    printf("USER is: %s\n", user.username);
    printf("HOST is: %s\n", user.host);
    sleep(1);
    clear();
    return user;
}


void addtohistory(char * buffer, FILE * fp)
{
    
    int i = 0;
    fprintf(fp, "%d", buffer);
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

int readinput(FILE * fp)
{   
    size_t size, last = 0;
    char * buffer  = NULL;

    do {
        size += MAXLETTERS;
        buffer = realloc(buffer, size);
        fgets(buffer + last, size, stdin);
        last = strlen(buffer) - 1;
    } while (buffer[last] != '\n');
    


    addtohistory(buffer, fp);

    return 1;
}

int processcommand()
{
    return 0;
}

void prompt(user_t user)
{   
    char cwd[MAXLETTERS];
    getcwd(cwd, sizeof(cwd));
    char* tmp = cwd;
    char* dirprompt;
    // get the first token
    tmp = strtok(tmp, "/");

    while(tmp)
    {
        dirprompt = tmp;
        tmp = strtok(NULL, "/");
    }
    
    fprintf(stdout, GREEN "%s@%s: " RESET BLUE "%s " RESET "# ", user, host, dirprompt);
    return;
}

int main()
{
    FILE *fp = fopen("history.txt", "a+");
    char * input, * pipe = NULL;
    int flag = 0;

    user_t * user = malloc(sizeof(user_t));
    user = initshell(user);
    
    while(1)
    {
        prompt(&user);
        input = readinput(fp);
        parser(input);

        switch (builtin) {
            case 0: // empty
                break;

            case 1: // cd command
                changedir();
                break;

            case 2: // exit command

            case 3: // help

            case 4:

            case 5:

            case 6:        
        }

        // process whatever is in the buffer            
        // flag = processcommand(input, args, pipe);
        exit(0);
    }

    fclose(fp);
    return 0;
}

