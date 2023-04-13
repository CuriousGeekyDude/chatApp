#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>
#include "error_functions.c"
#define socketPathName "/home/feri/Desktop/sockets"


void initializeBuffer(char* buffer, size_t sizeOfBuffer) {
    for(size_t i = 0; i < sizeOfBuffer; ++i)
        buffer[i] = '\0';
}

int main(int argc, char* argv[])
{
    char buffer[100];
    int fd, numRead;
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socketPathName, sizeof(addr.sun_path)-1);    

    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    connect(fd, (struct sockaddr*) &addr, sizeof(struct sockaddr_un));

    printf("client: ");
    fflush(stdout);
    while(numRead = read(STDIN_FILENO, buffer, 100) > 0) {
        write(fd, buffer, 100);
        initializeBuffer(buffer, 100);

        read(fd, buffer, 100);
        printf("server: ");
        fflush(stdout);

        write(STDOUT_FILENO, buffer, 100);
        fflush(stdout);
        initializeBuffer(buffer, 100);
        printf("client: ");
        fflush(stdout);
    }
    close(fd);
    exit(EXIT_SUCCESS);
}