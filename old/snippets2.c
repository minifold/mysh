// Function declarations
user_t initshell(user_t user);
int inithistory(FILE * fp, commands ** history);
commands ** addtohistory(char * buffer, char ** args, FILE * fp, 
                        commands ** history, int i);
FILE * readhistory(char ** argv, FILE * fp);
void help();
char * readinput(FILE * fp);
char ** parser(char * input);
int launch(user_t user, char ** args, commands comm);
int cwd(user_t user);
void prompt(user_t user);
int cd(char ** args, user_t user);
int bye(FILE * fp, commands ** history);


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


commands ** addtohistory(char * buffer, char ** args, FILE * fp, 
                        commands ** history, int i) {

    int size = 0;
    char c;
    while(!feof(fp))
    {
        c = fgetc(fp);
        if(c == '\n')
            size++;
    } 

    if (fprintf(fp, "%s\n", buffer) < 0) {
            fprintf(stderr, "mysh: error saving command to history.\n");
            return history;
    }

    if (i > size)
    {
        size += MAXLETTERS;
        history = realloc(history, size * sizeof(commands));
    }

    commands * command = (commands *)malloc(sizeof(commands)); 

    command->command = (char * )malloc(sizeof(buffer));
    strcpy(command->command, buffer);
    command->argv = (char ** )malloc(sizeof(args));
    command->argv = args;
    command->pid = 0;
    
    history[i] = command;

    return history;
}

FILE * readhistory(char ** argv, FILE * fp)
{
    char * buffer = malloc(MAXLETTERS * sizeof(char));

    size_t len = 0;
    if (!strcmp(argv[1], "-c"))
        freopen("history.txt", "w+", fp);
    
    else
        while (getline(&buffer, &len, fp) != EOF)
            fprintf(stdout, "%s\n", buffer);

    free(buffer);
    return fp;
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

int launch(user_t user, char ** args, commands comm)
{
    pid_t pid, wpid;
    int status;
    pid = fork();
    
    if (pid <= 0) {
        if (pid == -1)
        {
            // on timeout kill process
            // kill (pi, SIGERM);
            //
    
        }
        if (execv(user.dir, args) < 0) {
            fprintf(stderr, "mysh: error executing program\n");
            return -1;
        }
        fprintf(stderr, "mysh: error forking program\n");
        return -1;
    }

    else {
        comm.pid = pid;
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int cwd(user_t user) {
    if (user.dir == NULL) {
        fprintf(stderr, "mysh: current directory not found\n");
        return 0;
    }
    
    fprintf(stdout, "%s\n", user.dir);
    return 1;
}

void prompt(user_t user)
{    
    char cwd[MAXLETTERS];
    strcpy(cwd, user.dir);
    // truncate directory for viewing ease
    char * dirprompt = strrchr(user.dir, '/');
    dirprompt++;
    // print the prompt
    fprintf(stdout, GREEN "%s@%s: " RESET BLUE "%s " RESET "# ", user.username,
                                                                 user.host, dirprompt);
    return;
}

int cd(char ** args, user_t user)
{
    if (args[2] != NULL)
    {
        fprintf(stderr, "mysh: too many arguments\n");
        return 0;
    }

    if (args[1] == NULL)
    {
        fprintf(stderr, "mysh: argument required\n");
        return 0;
    }

    else
    {
        char buffer[MAXLETTERS];
        DIR * dir;
        if (dir = opendir(args[1])) {
            strcpy(buffer, args[1]);
            closedir(dir);
        }

        else {
            fprintf(stderr, "mysh: no directory found");
        }

        strrchr(buffer, '/');
    }
    return 0;
}

int bye(FILE * fp, commands ** history)
{
    fclose(fp);
    int size = sizeof(history) / sizeof(commands);

    for (int i = 0; i < size; i++)
    {
        free(history[i]->argv);
        free(history[i]->command);
    }

    free(history);
    clear();
    exit(0);
}

int dalek()
{

}

int bye(FILE * fp)
{
    fclose(fp);
    clear();
    exit(0);
}