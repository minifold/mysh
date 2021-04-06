#include <stdio.h>
#include <ctype.h>
#include <errno.h>
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
#define SIGKILL 9
// Clear function
#define clear() printf("\033[H\033[J") 

typedef struct user {
    char * username;
    char * host;
    char * dir;
} user_t;

typedef struct commands {
    char * command;
    int pid;
} commands;

// Global variables
int histindex = 0;
int pidsize = 0;
long MAXHISTSIZE = 0;

// Function declarations
char ** addtohistory(FILE * fp, char ** history, char * args);
int background(char ** args, int * pidarr, user_t user, int i);
void builtin(char * input, char ** argv, user_t user, pid_t * pid, 
                        char ** history, int histindex, FILE * fp);
user_t cd(char ** args, user_t user);
int cwd(user_t user);
void repeat(char ** args, int * pidarr, user_t user, int i);
dalek(pid_t pid, pid_t * pidarr, long pidsize);
void echo(char ** argv);
void exterminate(int * pid);
char ** inithistory(FILE * fp);
user_t initshell(user_t user);
char ** parser(char * input);
char * pathcheck(char * path, user_t user);
int start(char ** args, user_t user);
void bye(user_t user, FILE * fp);


// Initialize shell function
user_t initshell(user_t user)
{   
    clear();
    puts("================================\n"
        "                                 \n"
        "         Welcome to MySH!        \n"
        "                                 \n"
        "=================================\n"
        "Alex Cingoranelli | Spring 2021  \n"
        "This was written with Unix in    \n"
        "mind. It is not compatible with  \n"
        "MSDOS.                        \n\n");

    // Retrieve username
    user.username = getenv("USER");

    // Retrieve directory
    // This is just the way it works. I don't know why, but I tried setting it
    // Through malloc and got a world of pain. So at least I don't have to free
    // this.
    char pwd[MAXLETTERS];
    user.dir = malloc(sizeof(pwd));
    strcpy(user.dir, getcwd(pwd, sizeof(pwd)));
    
    // Retrieve hostname
    user.host = (char *)malloc(sizeof(char) * MAXLETTERS);
    gethostname(user.host, sizeof(user.host));
    
    printf("USER is: %s\n", user.username);
    printf("HOST is: %s\n", user.host);
    printf("Current Directory: %s\n", user.dir);

    sleep(2);
    clear();
    return user;
}

char ** replay(char ** args, char ** history, int index) {
    // It should be that replay isn't stored in the history command but 
    // choosing to go into an infinite loop isn't any of my business.
    int len = strlen(args[1]);
    for (int i = 0;  i < len - 1; i++)
        if (!isdigit(args[1][i])) {
            fprintf(stderr, RED "ERROR " RESET "mysh: %s not a number\n", args[1]);
            return NULL;
        }
    
    int num = atoi(args[1]);
    if (num > 0 && num < index)
        return parser(history[num]);
    else 
        fprintf(stderr, RED "ERROR " RESET "mysh: number out of range\n");    

}

char ** inithistory(FILE * fp) {
    long size = MAXLETTERS;
    char ** history = (char **)malloc(size * sizeof(char *)); 
    char * buffer = (char *)malloc(MAXLETTERS * sizeof(char));

    if (fp != NULL) {
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            if (histindex >= size) {
                size += MAXLETTERS;
                history = (char **)realloc(history, size * sizeof(char *));
            } 
            history[histindex] = malloc(sizeof(buffer));
            strcpy(history[histindex], buffer);
            histindex++;
        }
    }
    free(buffer);
    return history;
}

FILE * readhistory(char ** argv, char ** history, FILE * fp)
{
    // reopen file
    if (!strcmp(argv[1], "-c")) {
        // I checked to make sure this doesn't lose any of the history commands.
        // It shouldn't, but sometimes it did.
        for (int i = 0; i < histindex; i++)
            free(history[i]);
        
        freopen("history.txt", "w+", fp);

        fprintf(stdout, "mysh: history cleared\n");
        histindex = 0;    
    }

    return fp;
}

char ** addtohistory(FILE * fp, char ** history, char * args) {
    // Print to file
    // Check if history is openable
    if (!(fp = fopen("history.txt", "w"))) {
        printf("mysh: error opening history file.\n");
        return history;
    }

    fprintf(fp, "%s\n", args);
    if (histindex < MAXHISTSIZE) {
        history[histindex++] = strdup(args);
    } 
    // After some time simply delete the most recent history commands and 
    // replace them.
    else {
        free(history[0]);
        for (unsigned i = 1; i < MAXHISTSIZE; i++) {
            history[i - 1] = history[i];
        }
        history[MAXHISTSIZE - 1] = strdup(args);
    }

    return history;
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

char * readinput(FILE * fp) {   
    size_t size = 0, last = 0;
    char * buffer = NULL;

    do {
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

char ** parser(char * input) {
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
    char * buffer = (char *)calloc(PATH_MAX, sizeof(char));
    DIR * dir = NULL;
    
    if (args[2] != NULL) {
        fprintf(stderr, "mysh: too many arguments\n");
        free(buffer);
        return user;
    }

    if (args[1] == NULL) {
        fprintf(stderr, "mysh: argument required\n");
        free(buffer);
        return user;
    }

    else {
        if (args[1][0] != '/') {
            strcpy(buffer, user.dir);
            strcat(buffer, "/");
            strcat(buffer, args[1]);
        }

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
            fprintf(stderr, RED "ERROR" RESET "mysh: no directory found\n");
            free(buffer);
            return user;
        }

        strcpy(user.dir, buffer);
        // printf("%s\n", buffer);
    }

    free(buffer);
    return user;
}

void prompt(user_t user)
{    
    char cwd[PATH_MAX] = { 0 };
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
    pid_t pid = fork();
    int status;

    if (pid < 0)
        fprintf(stderr, RED "ERROR" RESET "mysh: process forking failed\n");
    
    if (pid == 0)
        if (execvp(args[0], args) == -1) {
            fprintf(stderr, RED "ERROR " RESET "mysh : program exec failure\n");
            return -1; 
        }
    
    else {
        waitpid(pid, &status, 0);
    }

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

    if (pid < 0) 
        fprintf(stderr, RED "ERROR " RESET "mysh : process forking failed\n");
    
    if (pid == 0)
        if (execvp(args[0], args) == -1){
            fprintf(stderr, RED "ERROR " RESET "mysh : program exec failure\n");
            return -1;
        }
       
    // Add pid to the pid table.
    pidarr[i] = pid;
    // Increment the history counter.
    i++;
    printf("+ background pid: %i, command: %s\n", pid, args[0]);
    return i;
}

void repeat(char ** args, int * pidarr, user_t user, int i) {
    int num;
    char ** tmp = args;
    if (!(num = atoi(args[0])))
    {
        fprintf(stderr, "mysh: first argument must be a number\n");
        return;
    }
    tmp++;
    while (num > 0) {
        i = background(tmp, pidarr, user, i);
        num++;
    }
    return;
}

int dalek(pid_t pid, pid_t * pidarr, long pidsize) {
    // There should be some sort of logic to check if the pid is negative so 
    // you don't do kill -1.
    if (pid <= 0)
        pid *= -1;  // + change to positive

    int exists = 0;
    for (int i = 0; i < pidsize; i++)
        if (pidarr[i] == pid)
            exists = 1;
    
    if (exists == 0)
    {
        fprintf(stderr, RED "ERROR " RESET "mysh : pid does not exist.\n");
        return -1;
    }

    if (kill(pid, SIGKILL) == -1)
    {
        fprintf(stderr, RED "ERROR " RESET "mysh : Error killing process.\n");
        return -1;
    }
    
    // Yay! We killed a process.
    fprintf(stderr, " + killed %d\n", pid);

    return 0;
}

void echo(char ** argv) {
    // int i = 0;
    // char strings[2] = { '"', ''' };
    // if (!strcmp(argv[i][0], '"')) {
    //    while (!strrchr(argv[i], '"'))
    //        fprintf(stdout, "%s ", argv[i]);
    //    fprintf(stdout, "\n");
    // }
}

void exterminate(int * pid) {
    // This *should* work. 
    int size = sizeof(pid) / sizeof(int);
    // Go through all pids and delete them, 
    for (int i = 0; i < size; i++)
        if (pid[i] > 0)
            dalek(pid[i], pid);

    return;
}

void bye(user_t user, FILE * fp) {
    // I was under the impression I would have to free the user struct pointers
    // after I was done with them, but when I did so I got an error.
    fclose(fp);
    clear();
    return;
}

int make(char * filename) {
    FILE * fp;

    if (!(fp = fopen(filename, "w"))) {
        fprintf(stderr, RED "ERROR" RESET "mysh: file could not be created\n");
        return 0;
    }

    fclose(fp);
    return 1;
}

void builtin(char * input, char ** argv, user_t user, pid_t * pid, 
                        char ** history, int histindex, FILE * fp) {
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
        
        dalek(strtol(argv[1], ptr, 10), pid, pidsize);
        pid[histindex] = 0;
    }
        
    else if (!strcmp(argv[0], "echo")) {
        tmp++;
        echo(tmp);
        tmp = NULL;
    }

    else if (!strcmp(argv[0], "background")) {
        tmp++;
        background(tmp, pid, user, histindex);
        tmp = NULL;
    }

    else if (!strcmp(argv[0], "repeat")) {
        tmp++;
        repeat(argv, pid, user, histindex);
        tmp = NULL;
    }

    else if (!strcmp(argv[0], "history")) {
        fp = readhistory(argv, history, fp);
    }

    else if (!strcmp(argv[0], "movetodir"))
        user = cd(argv, user);

    else
        fprintf(stderr, "%s is not a valid command.\n", input);      
}

// Driver function.
int main() {
    FILE * fp = fopen("history.txt", "a+");
    char ** argv;
    char * input = NULL;
    user_t user = initshell(user);
    char ** history = inithistory(fp);
    pid_t pid[MAX_INPUT] = { 0 };

    while(1)
    {
        prompt(user);
        // process whatever is in the buffer
        fflush(stdin);
        input = readinput(fp);
        history = addtohistory(fp, history, input);
        argv = parser(input);

        if (argv[0] == NULL)
            continue;

        // If you put this at the top it still has 
        if (!strcmp(argv[0], "replay")) {
            argv = replay(argv, history, histindex);
            builtin(input, argv, user, pid, history, histindex, fp);
        }

        else if (!strcmp(argv[0], "byebye") || !strcmp(argv[0], "^C")) {
            bye(user, fp);
            break;
        }

        else
           builtin(input, argv, user, pid, history, histindex, fp);

        free(input);
        free(argv);
    }

    return 0;
}
