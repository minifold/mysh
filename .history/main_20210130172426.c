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
    int commandnum = 6, i , args = 0;
    char * commandlist[commandnum];
    char * user;

    commandlist[0] = "exit";

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