#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int read()
{
    char* buffer;
    buffer = readline("\n# ");
    if (strlen(buffer) != 0)
    {
        
    }
}

int main()
{


    return 0;
}

int commands(char ** cmd)
{
    int commandnum = 7, i , args = 0;
    char * commandlist[commandnum];
    char * user;

    commandlist[0] = "byebye";
    commandlist[1] = "whereami";
    commandlist[2] = "history";
    commandlist[3] = "movetodir";
    commandlist[4] = "replay";
    commandlist[5] = "start";
    commandlist[6] = 


}

void help()
{
    puts("
    \nCommands supported:
    \nmovetodir
    \nwhereami
    \nhistory [-c]
    \nbyebye
    \nreplay <number>
    \nstart program [parameters]");
    return;
}