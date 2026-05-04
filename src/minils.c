#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

void DirHandling(const char *path) {
    struct dirent *pDirent;
    DIR *pDir;

    pDir = opendir(path);

    while((pDirent = readdir(pDir)) != NULL) {

        if((strcmp(pDirent->d_name,".")==0 || strcmp(pDirent->d_name,"..")==0 || (*pDirent->d_name) == '.' )) {
            continue;
        }

        printf("%s\n", pDirent->d_name);
    }

    closedir(pDir);
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        char *cwd;
        cwd = getcwd(NULL, 0);

        DirHandling(cwd);

        free(cwd);
        return 0;
    }
    
    DirHandling(argv[1]);

    return 0;
}