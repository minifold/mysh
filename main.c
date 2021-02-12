#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <sys/wait.h>

// ANSI color formatting for a fun shell experience :)
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

#define MAXLETTERS 4024
#define clear() printf("\033[H\033[J") 

typedef struct user {
    char * username;
    char * host;
    char * dir;
}user_t;

typedef struct commands {
    char * command;
    int pid;
}commands;

user_t initshell(user_t user)
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

    user.username = getenv("USER");
    user.host = malloc(sizeof(char) * MAXLETTERS);
    char pwd[MAXLETTERS];
    user.dir = getcwd(pwd, sizeof(pwd));

    gethostname(user.host, sizeof(user.host));
    
    printf("USER is: %s\n", user.username);
    printf("HOST is: %s\n", user.host);
    printf("Current Directory: %s\n", user.dir);

    sleep(2);
    clear();
    return user;
}

int inithistory(FILE * fp, char ** history)
{
    char *buffer = NULL;
    size_t len = 0;
    int i = 0;
    int size = MAXLETTERS;

    history = (char **)malloc(MAXLETTERS * sizeof(char *));

    while (getline(&buffer, &len, fp) != -1) {
        history[i] = malloc(sizeof(buffer));
        strcpy(history[i], buffer);
        if (i >= size)
        {
            size += MAXLETTERS;
            history = (char **)realloc(history, size * sizeof(char **));
        }
        i++;     
    }

    i++;
    free(buffer);
    return i;
}

int addhistory(char * input, FILE * fp, char ** history, int i)
{
    // history[i] = realloc(history[i], sizeof(input));
    // strcpy(history[i], input);
    fprintf(fp, "%s", input);
    i++;
    return i;
}

void help() {
    puts("*** MySH ***"
    "\n============\nCommands supported:"
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
    
    return buffer;
}

char ** parser(char * input)
{
    int i = 0, size = MAXLETTERS;
    char ** argv = malloc(MAXLETTERS * sizeof(char *));
    // Deal with all cases of delimiters: tabs, newlines, spaces, bell spaces, 
    char * token = strtok(input, "\t\r\n\a ");

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
                return 0;
            }
        }
        token = strtok(NULL, "\t\r\n\a ");
    }
    // add NULL args for execvp
    argv[i] = NULL;
    return argv;
}

int cwd(user_t user) {
    if (user.dir == NULL) {
        fprintf(stderr, "mysh: current directory not found\n");
        return 0;
    }
    
    fprintf(stdout, "%s\n", user.dir);
    return 1;
}

user_t cd(char ** args, user_t user)
{
    if (args[2] != NULL)
    {
        fprintf(stderr, "mysh: too many arguments\n");
        return user;
    }

    if (args[1] == NULL)
    {
        fprintf(stderr, "mysh: argument required\n");
        return user;
    }

    else
    {
        if (!strcmp(args[1], "~"))
        {
            char buf[PATH_MAX] = "/home/";
            strcat(buf, user.username);
            strcpy(args[1], buf);
        }

        char buffer[PATH_MAX];
        DIR * dir;
        if (dir = opendir(args[1])) {
            realpath(args[1], buffer);
            closedir(dir);
        }

        else {
            fprintf(stderr, "mysh: no directory found");
            return user;
        }

        strcpy(user.dir, buffer);
        printf("%s\n", buffer);
    }

    return user;
}

void prompt(user_t user)
{    
    char cwd[PATH_MAX];
    char * dirprompt;

    // change home to ~
    if (!strcmp(user.dir, "/home/quantum"))
    {
        strcpy(cwd, "/home/");
        strcat(cwd, user.username);
        dirprompt = malloc(sizeof("~"));
        strcpy(dirprompt, "~");
    }
    
    // truncate directory for viewing ease
    else {
        strcpy(cwd, user.dir);
        dirprompt = strrchr(user.dir, '/');
        dirprompt++;
    }
    // print the prompt
    fprintf(stdout, GREEN "%s@%s: " RESET BLUE "%s " RESET "# ", user.username,
                                                                 user.host, dirprompt);
    
    return;
}

int bye(FILE * fp)
{
    fclose(fp);
    clear();
    exit(0);
}


int main()
{
    FILE * fp = fopen("history.txt", "a+");
    char ** argv, ** history = NULL;
    char * input, * pipe = NULL;
    user_t user = initshell(user);
    int i = inithistory(fp, history);

    while(1)
    {
        prompt(user);
        input = readinput(fp);
        i = addhistory(input, fp, history, i);
        // fprintf(stdout, YELLOW "%s\n" RESET, input);
        argv = parser(input);

        // process whatever is in the buffer     
        if (!strcmp(argv[0], "byebye"))
            bye(fp);

        else if (!strcmp(argv[0], "whereami"))
            cwd(user);  

        else if (!strcmp(argv[0], "movetodir"))
            user = cd(argv, user);    
    }
    return 0;
}
