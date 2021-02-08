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



void help()
{
    puts("
    \nCommands supported:
    \nmovetodir
    \nwhereami
    \nhistory [-c]
    \nbyebye
    \nreplay <number>");
    return;
}