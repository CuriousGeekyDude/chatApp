#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "error_functions.c"
#define socketPathName "/home/feri/Desktop/sockets"

void initializeBuffer(char* buffer, size_t sizeOfBuffer) {
    for(size_t i = 0; i < sizeOfBuffer; ++i)
        buffer[i] = '\0';
}


int main(int argc, char* argv[]) 
{
    char buffer[100];
    struct sockaddr_un addr;
    int fd, bindResult, acceptResultfd, numRead;
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socketPathName, sizeof(addr.sun_path) - 1);

    remove(socketPathName);
    fd = socket(AF_UNIX, SOCK_STREAM, 0);

    if(fd == -1)
        errExit("socket");
    bindResult = bind(fd, (struct sockaddr*) &addr, sizeof(struct sockaddr_un));
    if(bindResult == -1)
        errExit("bind");
    
    if(listen(fd, SOMAXCONN) == -1)
        errExit("listen");

    while(true) {
    acceptResultfd = accept(fd,NULL,NULL);

    if(acceptResultfd > 0)
        printf("client connected\n");

    printf("client: ");
    fflush(stdout);
    while(numRead = read(acceptResultfd, buffer, 100) > 0) {
        write(STDOUT_FILENO, buffer, 100);
        fflush(stdout);
        initializeBuffer(buffer, 100);

        printf("server: ");
        fflush(stdout); 
        read(STDIN_FILENO, buffer, 100);                   
        write(acceptResultfd, buffer, 100);
        printf("client: ");
        fflush(stdout);
    }

    close(acceptResultfd);
    
    }
    exit(EXIT_SUCCESS);

}