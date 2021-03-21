#include <stdio.h>
#include <ctype.h>
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

// Max amount of letters. This is just for small things like literal strings 
// and initializations.
#define MAXLETTERS 4024
// Clear function
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

user_t initshell(user_t user);
void exterminate(int * pid);
void builtin(char ** argv, user_t user, pid_t * pid, char ** history, int histindex, FILE * fp);
user_t cd(char ** args, user_t user);
char * pathcheck(char * path, user_t user);
int cwd(user_t user);
int dalek(int pid);
int background(char ** args, int * pidarr, user_t user, int i);
char ** inithistory(FILE * fp, char ** history);
int addhistory(char * input, FILE * fp, char ** history, int i);
char ** parser(char * input);
int start(char ** args, user_t user);

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

char ** replay(char ** args, char ** history, int index)
{
    // It should be that replay isn't stored in the history command but 
    // choosing to go into an infinite loop isn't any of my business.
    int len = strlen(args[1]);
    for (int i = 0;  i < len - 1; i++)
        if (!isdigit(args[1][i])) {
            fprintf(stderr, RED "ERROR " RESET "mysh : %s not a number\n", args[1]);
            return NULL;
        }
    
    int num = atoi(args[1]);
    if (num > 0 && num < index)
        return parser(history[num]);
    else 
        fprintf(stderr, RED "ERROR " RESET "mysh : number out of range\n");    

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

int dalek(pid_t pid) {
    // There should be some sort of logic to check if the pid is negative so 
    // you don't do kill -1.
    if (pid <= 0)
        pid *= -1;  // + bit shift to positive

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
    int size = sizeof(pid) / sizeof(int *);
    // Go through all pids and delete them, 
    for (int i = 0; i < size; i++)
    {
        if (pid[i] > 0)
            dalek(pid[i]);
    }

    return;
}

int addhistory(char * input, FILE * fp, char ** history, int i)
{
    // long size = sizeof(history) / sizeof(char **);
    // fprintf(stdout, "%ld\n", size);
    // fprintf(stdout, "%d\n", i);
    // if (i > size)
    //     size += MAXLETTERS;
    //     history = realloc(history[i], size * sizeof(char **));
    
    // strcpy(history[i], input);
    // fprintf(fp, "%s", input);
    // i++;

    return i;
}

void help() {
    puts("*** MySH ***"
    "\n============\nCommands supported:"
    "\nmovetodir"
    "\nwhereami"
    "\nhistory [-c]"
    "\nclear"
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
        if (i >= size) {
            size += MAXLETTERS;
            if (!(argv = realloc(argv, size * sizeof(char *)))) {
                fprintf(stderr, RED "ERROR " RESET "mysh: memory allocation error while parsing args\n");
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

user_t cd(char ** args, user_t user) {
    char buffer[PATH_MAX];
    DIR * dir = NULL;
    
    if (args[2] != NULL) {
        fprintf(stderr, "mysh: too many arguments\n");
        return user;
    }

    if (args[1] == NULL) {
        fprintf(stderr, "mysh: argument required\n");
        return user;
    }

    else {
        if (!strcmp(args[1], "~")) {
            char buf[PATH_MAX] = "/home/";
            strcat(buf, user.username);
            strcpy(args[1], buf);
        }
        
        if (!strcmp(args[1], "."))
            strcpy(args[1], user.dir);

        if ((dir = opendir(args[1])) != NULL) {
            realpath(args[1], buffer);
            closedir(dir);
        }

        else {
            fprintf(stderr, RED "ERROR" RESET "mysh: no directory found");
            return user;
        }

        strcpy(user.dir, buffer);
        // printf("%s\n", buffer);
    }

    return user;
}

char * pathcheck(char * path, user_t user)
{
    char * new = (char *)calloc(MAXLETTERS, sizeof(char));
    char bin[MAXLETTERS] = "/usr/bin/";
    // It works by checking if the executable is within /usr/bin
    // If the string contains the substring /usr/bin then the start function
    // tries to call it; if not, and there's no beginning / then try and see 
    // if it's an exec in the current folder or a typo.
    if (strstr(path, bin) == NULL) {
            strcat(bin, path);
            strcpy (new, bin);
            // strcpy(new, path);
    }
    // These two should not interfere with each other, since the previous if 
    // statement adds /usr/bin at the end, and the next statement checks if
    // the input has a / in the front.
    if (path[0] != '/') {
        strcpy(new, user.dir);
        strcat(new, "/");
        strcat(new, path);
        // Checks if specified path exists. 
        if (!realpath(path, new)) {
            // if It doesn't exist in the current directory, check to see if 
            // it is a system function
            free(new);
            new = (char *)calloc(MAXLETTERS, sizeof(char));
            strcat(bin, path);
            strcpy(new, bin);
            if (!realpath(path, new)) {
                fprintf(stderr, RED "ERROR " RESET "mysh : program %s does not exist.\n", new);
                fprintf(stderr, "Try calling the program name or direct path from /usr/bin/ instead.\n");
                return NULL;
            }
        }
    }

    else
        strcpy(new, path);

    return new;
}

void prompt(user_t user)
{    
    char cwd[PATH_MAX];
    char * dirprompt;
    // get the user's name for their home directory.
    char home[MAXLETTERS] = "/home/";
    strcat(home, user.username);
    // change home to ~
    // If you go to /home/$USER it will display as ~ like in bash.
    if (!strcmp(user.dir, home)) {
        strcpy(cwd, home);
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

int start(char ** args, user_t user) {
    // Fork the current process, then change the child process to the 
    // program you want to run.
    pid_t pid;
    int status;
    
    char * path = pathcheck(args[0], user);

    if (!path)
        return 0;
    
    pid = fork();
    if (pid < 0)
        fprintf(stderr, RED "ERROR" RESET "mysh: process forking failed\n");
    
    if (pid == 0)
        if (execvp(path, args) == -1) {
            fprintf(stderr, RED "ERROR " RESET "mysh : program exec failure\n");
            free(path);
            return -1; 
        }
    
    else
        waitpid(pid, &status, 0);
    
    free(path);
    return 1;
}

int background(char ** args, int * pidarr, user_t user, int i)
{
    // Background should be the same as start, except we don't wait. 
    // I don't really know why, but when executed the function still shows up
    // on stdout rather than through  
    pid_t pid = pidarr[i];
    // check if already a daemon
    if (kill(pid, 0)) {
        fprintf(stderr, "mysh : process with pid %d already exists\n", pid);
        return -1;
    }

    pid = fork();
    char * path = pathcheck(args[0], user);
    if (!path) {
        free(path);
        return 0;
    }

    if (pid < 0) 
        fprintf(stderr, RED "ERROR " RESET "mysh : process forking failed\n");
    
    if (pid == 0)
        if (execvp(path, args) == -1){
            fprintf(stderr, RED "ERROR " RESET "mysh : program exec failure\n");
            free(path);
            return -1;
        }
       
    // Add pid to the pid table.
    pidarr[i] = pid;
    // Increment the history counter.
    i++;
    printf("+ background pid: %i, command: %s\n", pid, args[0]);
    free(path);
    return i;
}

FILE * readhistory(char ** argv, char ** history, FILE * fp, int index)
{
    // This checks three things: if the file should be cleared
    // If the user checks a specific history number
    // And if the user doesn't put anything, to just show the whole history.
    char * buffer = malloc(MAXLETTERS * sizeof(char));

    if (argv[2]) {
        fprintf(stderr, RED "ERROR " RESET "mysh : too many arguments");
        free(buffer);
        return fp;
    }

    // reopen file
    if (!strcmp(argv[1], "-c")) {
        // I checked to make sure this doesn't lose any of the history commands.
        // It shouldn't, but sometimes it did.
        freopen("history.txt", "w+", fp);
        for (int i = 0; i < index; i++)
            free(history[i]);

        index = 0;    
    }

    else if (argv[1] != NULL)
        fprintf(stdout, "%s: %s\n", argv[1], history[index - atoi(argv[2])]);

    else if (argv[1] == NULL) {
        int histind = 0;
        for (int i = index - 1; i >= 0; i--)
            fprintf(stdout, "%d: %s\n", histind++, history[i]);
        }

    free(buffer);
    return fp;
}

void bye(FILE * fp)
{
    fflush(stdin);
    fclose(fp);
    clear();
    return;
}

void builtin(char ** argv, user_t user, pid_t * pid, char ** history, int histindex, FILE * fp)
{
    char ** tmp = argv;
    
    if (!strcmp(argv[0], "whereami"))
            cwd(user);  
    
    else if (!strcmp(argv[0], "start")) {
        // I moved the pointer of the struct so that it wouldn't be 
        // included in the function call.
        tmp++;
        start(tmp, user);
        tmp = NULL;
    }

    else if (!strcmp(argv[0], "clear"))
        clear();
        
    else if (!strcmp(argv[0], "dalek")) {
        char ** ptr = NULL;
        if (argv[1] == NULL)
            fprintf(stderr, RED "ERROR " RESET "mysh : argument required for dalek\n");
        dalek(strtol(argv[1], ptr, 10));
        pid[histindex] = 0;
    }
        
    else if (!strcmp(argv[0], "background")) {
        tmp++;
        background(tmp, pid, user, histindex);
    }

    else if (!strcmp(argv[0], "history"))
        fp = readhistory(argv, history, fp, histindex);

    else if (!strcmp(argv[0], "movetodir"))
        user = cd(argv, user);

    else
        fprintf(stderr, "%s is not a valid command.\n", argv[0]);      
}

// Driver function.
int main()
{
    FILE * fp = fopen("history.txt", "a+");
    char ** argv;
    char * input = NULL;
    user_t user = initshell(user);
    char ** history = inithistory(fp, history);
    int histindex = 0;
    pid_t pid[MAX_INPUT] = { 0 };

    while(1)
    {
        prompt(user);
        // process whatever is in the buffer
        fflush(stdin);
        input = readinput(fp);
        histindex = addhistory(input, fp, history, histindex);
        argv = parser(input);

        // If you put this at the top it still has 
        if (!strcmp(argv[0], "replay")) {
            argv = replay(argv, history, histindex);
            builtin(argv, user, pid, history, histindex, fp);
        }

        else if (!strcmp(argv[0], "byebye")) {
            bye(fp);
            break;
        }

        else
           builtin(argv, user, pid, history, histindex, fp);

        free(input);
        free(argv);
    }

    free(user.host);
    return 0;
}
