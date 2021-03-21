#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <sys/wait.h>
#include <signal.h>

int main()
{
    char pwd[1000];
    char * dir = getcwd(pwd, sizeof(pwd));
    char path[1000] = "..";

    printf("%s\n", dir);
    char * newdir = realpath(path, dir);
    printf("%s\n", newdir);
    printf("%s\n", realpath(path, newdir));
    return 0;
}