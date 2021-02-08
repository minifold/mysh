#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>

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
}builtin_t;

typedef struct usercommands {
    char * command;
    int argc;
    char * argv;
}usercomms;


char ** parser(char * input);
user_t * initshell(user_t * user);
char * readinput(FILE * fp);
void help();
int addtohistory(char * buffer, FILE * fp);
int cd(char * args);


user_t * initshell(user_t * user)
{   
    clear();
    puts("================================\n"
        "                                 \n"
        "         Welcome to MySH!        \n"
        "                                 \n"
        "=================================\n"
        "Written by Alex Cingoranelli.    \n"
        "This was written with Unix in    \n"
        "mind. It is not compatible with  \n"
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


int addtohistory(char * buffer, FILE * fp)
{
    
    int i = 0;
    fprintf(fp, "%s", buffer);
    return 0;
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
            fprintf(stderr, "mysh: memory allocation error while reading user input\n");
            return 0;    
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
                fprintf(stderr, "mysh: memory allocation error while parsing args\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, "\t\r\n\a");
    }
    // add NULL args for execvp
    argv[i] = NULL;
    return argv;
}

int launch(char ** args)
{
    pid_t pid, wpid;
    int status;
    pid = fork();
    
    if (pid == 0)
        if (execvp(args[0], args) < 0)
        {
            fprintf(stderr, "mysh: error executing program\n");
            return -1;
        }
    else if (pid < 0)
    {
        fprintf(stderr, "mysh: error forking program\n");
        return -1;
    }
    else
    {
        // int child;
        wpid = waitpid(pid, &status, WUNTRACED);
        return 1;
    }
}

int cwd(user_t * user)
{
    if (user->dir == NULL)
        fprintf(stdout, "%s\n", user->dir);
    
    else 
    {
        fprintf(stderr, "mysh: current directory not found\n");
        return 0;
    }
    return 1;
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

int execute(char ** args)
{
    
    return 0;
}

int cd(char * args)
{
    if (args == NULL)
    {
        fprintf(stderr, "mysh: argument required\n");
        return 0;
    }

    else
    {
        DIR * dir = opendir(args);
        if (dir)
    }
    return 0;
}

int main()
{
    FILE *fp = fopen("history.txt", "a+");
    char ** argv;
    char * input, * pipe = NULL;
    int flag = 0;

    user_t * user = malloc(sizeof(user_t));
    user = initshell(user);
    
    while(1)
    {
        prompt(user);
        input = readinput(fp);
        fprintf(stdout, YELLOW "%s\n" RESET, input);
        argv = parser(input);

        if (launch(argv) < 0)
            continue;
        
        if (strcmp(argv[0], "whereami") == 0)
            cwd(user);
            
        if (strcmp(argv[0], "movetodir") == 0)
            cd(argv);    
        
        // process whatever is in the buffer            
        // flag = processcommand(input, args, pipe);
    }
    
    fclose(fp);
    return 0;
}

