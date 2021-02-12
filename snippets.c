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




        // fprintf(stdout, YELLOW "%s" RESET, argv[0]);

        if (!strcmp(argv[0], "whereami"))
        {
            cwd(user);
            continue;
        }

        else if (!strcmp(argv[0], "start"))
            if (launch(user, argv) < 0)
                continue;

        else if (!strcmp(argv[0], "movetodir"))
        {
            cd(argv, user);
            continue;
        }

        else if (!strcmp(argv[0], "history"))
        {
            fp = readhistory(argv, fp);
            continue;
        }

        else if (!strcmp(argv[0], "byebye"))
        {
            bye(fp, user, history);
            break;
        }
