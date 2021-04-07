#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

int is_dir(const char* path);
void copyFile_(char * inDir, char * outDir);
void copyDir_(const char * inputDir, char * outDir);

int main()
{

    char * srcDir = "C:\\testDirectory";
    char * destDir = "C:\\destDir";

    copyDir_(srcDir,  destDir);

    return 0;
}

void copyDir_(const char *inputDir, char * outDir)
{
    DIR * dir;
    struct dirent entry;
    char *tmpStr, *tmpStrPath, *outStrPath, *inputDir_str;

    if (is_dir(inputDir) == 0) {
        printf("Not a directory\n");
        return;
    }

    if( (dir = opendir(inputDir)) ) {
        while(entry = readdir(dir) ) {
            // get folders and files names
            tmpStr = entry.d_name;
            if( strcmp(entry.d_name, ".")  != 0 && strcmp(entry.d_name, "..") != 0 )
            {
                tmpStrPath = strdup(inputDir);
                strcat(tmpStrPath, "/" );
                strcat(tmpStrPath, tmpStr );

                if (is_dir(tmpStrPath)) {
                    printf("It's a folder\n");
                    // Create Folder on the destination path
                    outStrPath = strdup(outDir);
                    strcat(outStrPath, "/");
                    outStrPath.append( tmpStr );
                    mkdir(outStrPath);

                    copyDir_(tmpStrPath, outStrPath);
                }
                else
                {
                    printf("--> It's a file\n");
                    // copy file on the destination path
                    outStrPath = outDir;
                    outStrPath.append( "\\" );
                    outStrPath.append( tmpStr );
                    copy(tmpStrPath, outStrPath);
                }
            }
        }
        closedir(dir);
    }
}

int is_dir(const char* path) {
    struct stat s;
    stat(path, &s);
    return S_ISDIR(s.st_mode);
}

// void copyFile_(char * inDir, char * outDir) {
//     CopyFile(inDir.c_str(), outDir.c_str(), 1);
//     DWORD Error = GetLastError();
// }