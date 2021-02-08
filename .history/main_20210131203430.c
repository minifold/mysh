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

typedef struct builtincommands {
    char * command;
    int commandnum;
    char description[MAXLETTERS];
}builtin;

typedef struct usercommands {
    char * command;
    int argc;
    char * argv;
}usercomms;

user_t * initshell(user_t * user)
{   
    clear();
    puts("================================\n"
        "                                 \n"
        "         Welcome to MySH!        \n"
        "                                 \n"
        "=================================\n"
        "Written by Alex Cingoranelli.   \n"
        "This was written with Unix in   \n"
        "mind. It is not compatible with \n"
        "MSDOS.                        \n\n");   
    user->username = getenv("USER");
    user->host = malloc(sizeof(char) * MAXLETTERS);
    gethostname(user->host, sizeof(user->host));
    printf("USER is: %s\n", user->username);
    printf("HOST is: %s\n", user->host);
    sleep(3);
    clear();
    return user;
}


void addtohistory(char * buffer, FILE * fp)
{
    
    int i = 0;
    fprintf(fp, "%s", buffer);
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

char * readinput(FILE * fp)
{   
    size_t size = 0, last = 0;
    char * buffer = NULL;

    do
    {
        size += MAXLETTERS;
        
        if (!(buffer = realloc(buffer, size + 1)))
        {
            fprintf(stderr, "mysh: memory allocation error\n");
            exit(EXIT_FAILURE);    
        }

        fgets(buffer + last, size, stdin);
        last = strlen(buffer) - 1;

    } while(!feof(stdin) && buffer[last] != '\n');
    
    addtohistory(buffer, fp);
    
    return buffer;
}

char ** parser(char * input)
{
    int i = 0, size = MAXLETTERS;
    char ** argv = malloc(MAXLETTERS * sizeof(char *));
    char * token = strtok(input, " ");

    while (token != NULL)
    {
        argv[i] = token;
        i++;
        if (i >= size)
        {
            size += MAXLETTERS;
            if (!(argv = realloc(argv, size * sizeof(char *))))
            {
                fprintf(stderr, "mysh: memory allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, " ");
    }
    // remove the trailing \n
    argv[i] = NULL;
    return argv;
}

int launch()
{
    return 0;
}

void prompt(user_t * user)
{    
    char cwd[MAXLETTERS];
    getcwd(cwd, sizeof(cwd));
    user->dir = cwd;
    // truncate directory for viewing ease
    char* tmp = user->dir;
    char* dirprompt;
    // get the first token
    tmp = strtok(tmp, "/");
    // find the last directory in the file tree
    while(tmp)
    {
        dirprompt = tmp;
        tmp = strtok(NULL, "/");
    }
    // print the prompt
    fprintf(stdout, GREEN "%s@%s: " RESET BLUE "%s " RESET "# ", user->username, user->host, dirprompt);
    return;
}

int builtinparser()
{
    return 0;
}

int cd(char * input)
{
    return 0;
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
        prompt(user);
        input = readinput(fp);
        argv = parser(input);

        switch (builtinparser()) {
            case 0: // empty
                break;

            case 1: // cd command
                if (!cd(input))
                    break;

            case 2: // exit command
                break;
            case 3: // help
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:  
                break;      
        }

        // process whatever is in the buffer            
        // flag = processcommand(input, args, pipe);
        exit(0);
    }
    
    fclose(fp);
    return 0;
}

