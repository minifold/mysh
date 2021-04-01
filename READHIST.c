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
}

void readhistory(char ** history, char * args)
{
    if (!strcmp(args, "-c"))
    {
        free(history);
        freopen();
    }
}