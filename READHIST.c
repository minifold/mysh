// READHIST
// 1. make file
// 2. malloc size of history based on 
// 3. fopen 
// 4. read the history file into history array
// 5. 

// SAVETOFILE
// 1. open file
// 2. write string to file 
// 3. free 
// 4. 
<<<<<<< HEAD
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
            history[i] = malloc(sizeof(buffer));
            strcpy(history[i], buffer);
            histindex++;
        }
    }
    free(buffer);
    return history;
=======
char ** inithistory(FILE * fp)
{
    char * history = "history.txt";
    if (fopen(fp, history, "a+") != NULL)
    {
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            history[i] = malloc(sizeof(buffer + 1));
            strcpy(history[i], buffer);
            i++;
        }
    }

    for (int i = 0; i < ) {

    }
>>>>>>> origin/master
}

void readhistory(char ** history, char * args)
{
    if (!strcmp(args, "-c"))
    {
        free(history);
        freopen();
    }
<<<<<<< HEAD
}

int addhistory(char * input, FILE * fp, char ** history, int i)
{
    if (histindex >= size) {
        size += MAXLETTERS;
        history = (char **)realloc(history, size * sizeof(char *));
    }

    history[i] = (char *)realloc(history[i], sizeof(input));
    strcpy(history[i], input);
    fprintf(fp, "%s", input);
    i++;

    return i;
=======
>>>>>>> origin/master
}