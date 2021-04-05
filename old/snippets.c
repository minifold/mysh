char ** parser(char * input);
user_t * initshell(user_t * user);
char * readinput(FILE * fp);
void help();
commands * addtohistory(char * buffer, char ** args, FILE * fp,
                        commands * history);
FILE * history(char ** argv, FILE * fp);
int cd(char ** args, user_t * user);
int bye(FILE * fp, user_t * user, commands * history);

// int main()
// {


//     return 0;
// }

// int commands(char ** cmd)
// {
//     int commandnum = 7, i , args = 0;
//     char * commandlist[commandnum];
//     char * user;

//     commandlist[0] = "byebye";
//     commandlist[1] = "whereami";
//     commandlist[2] = "history";
//     commandlist[3] = "movetodir";
//     commandlist[4] = "replay";
//     commandlist[5] = "start";
//     commandlist[6] = "background";


// }

// int read(char * args)
// {
//     char* buffer;
//     // buffer = readline("\n# ");
//     if (strlen(buffer) != 0)
//     {
//         addtohistory(buffer);
//         // strcpy(args);
//         return 0;
//     }
//         return 1;

//     while(scanf("%49[^ \t\n]%*c", input))
//     {   
//         if (strlen(input) > MAXLETTERS || i > MAXCOMMS)
//             return 0;    
//         strcpy(buffer[i], input);
//         i++;
//     }
// }

    
// char * getline()
// {
//     size_t size, len, last = 0;
//     char * buffer  = NULL;

//     do {
//         size += MAXLETTERS;
//         buffer = realloc(buffer, size);            
//         /* Actually do the read. Note that fgets puts a terminal '\0' on the
//            end of the string, so we make sure we overwrite this */
//         fgets(buffer + last, size, stdin);
//         len = strlen(buffer);
//         last = len - 1;
//     } while (buffer[last] != '\n');
//     return buf;
// }

 char buffer[MAXLETTERS];
    size_t len = 0;
    int i = 0;
    int size = MAXLETTERS;

    history = (char **)malloc(MAXLETTERS * sizeof(char **));

    while (fscanf(fp, "%s", buffer) == 1) {
        // printf("%s\n", buffer);
        history[i] = malloc(sizeof(buffer));
        strcpy(history[i], buffer);
        i++;     
    }
    
    history[i] = malloc(sizeof(buffer));
    strcpy(history[i], buffer);

    i++;
    // Cleaning up.
    return history;