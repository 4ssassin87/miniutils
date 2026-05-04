#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>

struct stat sx;

int readBytes(int fd, struct stat *sx) {
    fstat(fd, sx);
    return sx->st_size;
}

int getMode(int fd, struct stat *sx) {
    fstat(fd, sx);
    return sx->st_mode;
}

int main(int argc, char* argv[]) {

    int in_fd = 0;
    int out_fd = 0;
    int bytes_in_file = 0;
    int file_mode = 0;
    int file_sent = 0;

    if (argc < 3) {
        printf("minicp: missing file operand\n");
    }

    in_fd = open(argv[1], O_RDONLY, 0);

    file_mode = getMode(in_fd, &sx);

    out_fd = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, file_mode);

    bytes_in_file = readBytes(in_fd, &sx);

    file_sent = sendfile(out_fd, in_fd, NULL, bytes_in_file);

    if(file_sent < 0){
        printf("No se a podido mandar el archivo!\n");
        return EXIT_FAILURE;
    }

    close(in_fd);
    close(out_fd);
    return EXIT_SUCCESS;

}