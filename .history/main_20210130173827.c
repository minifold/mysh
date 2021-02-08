#include <stdio.h>
#include <string.h>
#include <stdlib.h>



void help()
{
    puts("
    \nCommands supported:
    \nmovetodir
    \nwhereami
    \nhistory [-c]
    \nbyebye
    \nreplay <number>
    \nstart <program> [parameters]
    \nbackground <program> [parameters]
    \ndalek <PID>
    \nrepeat n command ...");
    
    return;
}