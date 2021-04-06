#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <sys/wait.h>
#include <signal.h>

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

user_t initshell(user_t user);
void exterminate(int * pid);
int dalek(int pid);
int background(char ** args, int * pidarr, int i);
char ** inithistory(FILE * fp, char ** history);
int addhistory(char * input, FILE * fp, char ** history, int i);
char ** parser(char * input);
int start(char ** args);
void help();

user_t initshell(user_t user)
{   
    // Finangling this took me forever. But at least it looks nice.
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

    // Get all system information for shell prompt
    user.username = getenv("USER");
    user.host = malloc(sizeof(char) * MAXLETTERS);
    char pwd[MAXLETTERS];
    user.dir = getcwd(pwd, sizeof(pwd));

    gethostname(user.host, sizeof(user.host));
    
    // Just checking for debugging purposes.
    printf("USER is: %s\n", user.username);
    printf("HOST is: %s\n", user.host);
    printf("Current Directory: %s\n", user.dir);

    // Clear the shell afterwards.
    sleep(2);
    clear();
    return user;
}

char ** inithistory(FILE * fp, char ** history)
{
    char * buffer = NULL;
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
    // Cleaning up.
    free(buffer);
    return history;
}

int addhistory(char * input, FILE * fp, char ** history, int i)
{
    int size = sizeof(history) / sizeof(char **);
    if (i > size)
        size += MAXLETTERS;
        history = realloc(history[i], size * sizeof(char **));
    
    strcpy(history[i], input);
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
    size_t size = 0;
    char * buffer = NULL;
    if (getline(&buffer, &size, stdin) == -1)
    {
        if(feof(stdin))
           exit(0); 
        else
        {
            fprintf(stderr, "mysh : error reading from line\n");
            exit(-1);
        }
    }
    
    return buffer;
}

char ** replay(char ** args, char ** history)
{
    // Pretty easy oneliner. So long as you don't accidentally choose to replay a replay. 
    // It should be that replay isn't stored in the history command but choosing to go into an 
    // infinite loop isn't any of my business.
    return parser(history[(int)args[1]]);
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
    // Shouldn't be able to cd to more than one place at a time.
    if (args[2] != NULL)
    {
        fprintf(stderr, "mysh: too many arguments\n");
        return user;
    }

    // Nothing to cd to!
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
            strncpy(args[1], buf, PATH_MAX);
        }

        char buffer[PATH_MAX];
        char * dir = realpath(args[1], buffer);
        if (dir) {
            printf("%s\n", buffer);
            strcpy(user.dir, buffer);
        }

        else
            // Unfortunately, you get a "no directory found" as realpath 
            // only ever searches in the directory of the currently running 
            // program, which is infuriating. So any relative pathing would 
            // be pointless, as you can only relpath around the directory this
            // program is issued from.
            fprintf(stderr, "mysh: no directory found\n");
    }

    return user;
}

void prompt(user_t user)
{    
    char * dirprompt; 
    char home[PATH_MAX];

    strcpy(home, "/home/");
    strcat(home, user.username);
    // change home to ~
    if (!strcmp(user.dir, home))
        dirprompt = strdup("~");
    
    // truncate directory for viewing ease
    else {
        dirprompt = strrchr(user.dir, '/');
        dirprompt++;
    }
    // print the prompt
    fprintf(stdout, GREEN "%s@%s: " RESET BLUE "%s " RESET "# ", user.username,
                                                                 user.host, dirprompt);

    
    return;
}

int start(char ** args) {
    // Fork the current process, then change the child process to the 
    // program you want to run. Yay.
    pid_t pid, wpid;
    int status = 0;
    char env[PATH_MAX] = "/usr/bin/";
    
    if (strstr(args[0], "/"))
        strcat(env, args[0]);
    
    if ((pid = fork()) <= 0)
    {
        if (execvp(args[0], args) == -1)
            fprintf(stderr, "mysh : error forking process\n");
        return -1;  
    }
    
    else
        waitpid(pid, &status, 0);

    return 1;
}

int background(char ** args, int * pidarr, int i)
{
    // Background should be the same as start, except we don't wait. 
    // I don't really know why, but when executed the function still shows up
    // on stdout rather than through  
    pid_t pid;
    pid = fork();
    // check if already a daemon
    if(kill(pid, 0)){
        fprintf(stderr, "mysh : process with pid %d already exists\n", pid);
        return -1;
    }
    if (pid < 0) 
        fprintf(stderr, "mysh : error forking process\n");
    
    if (pid == 0)
    {
        if (execvp(args[0], args) == -1){
            fprintf(stderr, "mysh : error forking process\n");
            return -1;
        }


    }   
        // Add pid to the pid table.
        pidarr[i] = pid;
        // Increment the history counter.
        i++;
    printf("+ background pid: %i, command: %s\n", pid, args[0]);
    return i;
}

FILE * readhistory(char ** argv, char ** history, FILE * fp)
{
    // This checks three things: if the file should be cleared
    // If the user checks a specific history number
    // And if the user doesn't put anything, to just show the whole history.

    char * buffer = malloc(MAXLETTERS * sizeof(char));
    int size = sizeof(history) / sizeof(char **);
    size_t len = 0;
    
    // reopen file
    if (!strcmp(argv[1], "-c"))
        // I checked to make sure this doesn't lose any of the history commands.
        // It shouldn't, but sometimes it did.
        freopen("history.txt", "w+", fp);
    
    else if (argv[1] != NULL)
        fprintf(stdout, "%s: %s\n", argv[1], history[size - (int)argv[1]]);

    else if (argv[1] == NULL) {
        for (int i = size; i > 0; i--)
            fprintf(stdout, "%d: %s\n", size - i, history[i]);
        }

    free(buffer);
    return fp;
}

int dalek(int pid) {
    // There should be some sort of logic to check if the pid is negative so 
    // you don't do kill -1.

    if (kill(pid, SIGKILL) == -1)
    {
        fprintf(stderr, "mysh: Error killing process.\n");
        return -1;
    }
    
    // Yay! We killed a process.
    fprintf(stderr, " + killed %d\n", pid);

    return 0;
}

void exterminate(int * pid)
{
    // This *should* work. 
    int size = sizeof(pid) / sizeof(int);
    // Go through all pids and delete them, 
    for (int i = 0; i < size; i++)
    {
        if (pid[i] > 0)
            dalek(pid[i]);
    }

    return;
}

int main() {
    FILE * fp = fopen("history.txt", "a+");
    char ** argv, ** tmp, ** history = NULL;
    char * input, * ptr;
    int i = 0;

    user_t user = initshell(user);
    history = inithistory(fp, history);
    int * pid = malloc(MAX_INPUT * sizeof(int));

    while(1)
    {
        prompt(user);
        input = readinput(fp);
        addhistory(input, fp, history, i);
        // fprintf(stdout, YELLOW "%s\n" RESET, input);
        argv = parser(input);

        // run this one first, so that you can run the other 
        // commands afterwards
        if (!strcmp(argv[0], "replay"))
            argv = replay(argv, history);

        // process whatever is in the buffer     
        else if (!strcmp(argv[0], "byebye"))
            break;

        else if (!strcmp(argv[0], "whereami"))
            cwd(user);  

        else if (!strcmp(argv[0], "movetodir"))
            user = cd(argv, user);    
        
        else if (!strcmp(argv[0], "start"))
        {
            // I moved the pointer of the struct so that it wouldn't be 
            // included in the function call.
            tmp = argv;
            tmp++;
            start(tmp);
        }

        else if (!strcmp(argv[0], "dalek"))
        {
            if (argv[1] == NULL)
                fprintf(stderr, "argument required for dalek\n");
            dalek(strtol(argv[1], &ptr, 10));
        }
        
        else if (!strcmp(argv[0], "background"))
        {
            tmp = argv;
            tmp++;
            background(tmp, pid, i);
        }

        else if (!strcmp(argv[0], "history"))
            fp = readhistory(argv, history, fp);

        free(input);
        free(argv);
    }

    fclose(fp);
    clear();
    exit(0);
    free(pid);
    return 0;
}
