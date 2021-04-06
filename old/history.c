int histindex = 0;

char ** inithistory(FILE * fp)
{
    long size = MAXLETTERS;
    char * histstring = "history.txt";
    char ** history = (char **)malloc(size * sizeof(char *)); 
    char * buffer = (char *)malloc(MAXLETTERS * sizeof(char));

    if (fopen(fp, histstring, "a+") != NULL)
    {
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            if (i >= size)
            {
                size += MAXLETTERS;
                history = (char **)realloc(size * sizeof(char *));
            } 
            history[i] = malloc(sizeof(buffer + 1));
            strcpy(history[i], buffer);
            histindex++;
        }
    }
    free(buffer);
    return history;
}

int addhistory(char * input, FILE * fp, char ** history, int i)
{
    int size = sizeof(history) / sizeof(char);
    if (i >= size) {
        size += MAXLETTERS;
        history = (char **)realloc(history, size * sizeof(char *));
    }

    history[i] = (char *)realloc(history[i], sizeof(input));
    strcpy(history[i], input);
    fprintf(fp, "%s", input);
    i++;

    return i;
}

int checkhistory()
{   
    // Kind of stupid but you have to open the file in read to get to the 
    // beginning of the file. So in order to read in all of the history files
    // I just open history again, rifle through it, and close it. 
    FILE * fp = fopen("history.txt", "r");
    int i = 0;
    // This is probably bad practice.
    for (char c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n') // Increment count if this character is newline
            i++;
    fclose(fp);
    return i;
}

FILE * readhistory(char ** argv, char ** history, FILE * fp, int index)
{
    // This checks three things: if the file should be cleared
    // If the user checks a specific history number
    // And if the user doesn't put anything, to just show the whole history.
    char * buffer = malloc(MAXLETTERS * sizeof(char));

    if (argv[2]) {
        fprintf(stderr, RED "ERROR " RESET "mysh : too many arguments\n");
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

        fprintf(stdout, "mysh : history cleared\n");
        index = 0;    
    }

    else if (argv[1] != NULL)
        fprintf(stdout, "%s: %s\n", argv[1], history[index - atoi(argv[1])]);

    else if (argv[1] == NULL) {
        int histind = 0;
        for (int i = index - 1; i >= 0; i--)
            fprintf(stdout, "%d: %s\n", histind++, history[i]);
    }

    free(buffer);
    return fp;
}

static const char *history[HISTORY_MAX_SIZE];
int history_count = 0;

