char ** inithistory(FILE * fp, char ** history)
{
    // Preventing any sort of memory errors by just starting history with an 
    // initial size of MAXLETTERS.
    int init = MAXLETTERS;
    // Initializing a counter variable as well.
    int i = 0;
    history = (char **)malloc(init * sizeof(char *));
    char * buffer = (char *)malloc(MAXLETTERS * sizeof(char));

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        history[i] = malloc(sizeof(buffer + 1));
        strcpy(history[i], buffer);
        i++;
    }
    
    if (feof(fp)) {
        // When there's an end of file signal just create a single char pointer to NULL.
        // The pointer doesn't incriment here so it should be overwritten.
        history[i] = (char *)malloc(sizeof(char));
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

