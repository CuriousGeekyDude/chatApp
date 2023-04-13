#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "error_functions.c"
#define socketPathName "/home/feri/Desktop/sockets"


int main(int argc, char* argv[]) 
{
    char buffer[100], *serverBuff = "client: ";
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
    while(numRead = read(acceptResultfd, buffer, 100) > 0) {
        printf("%s", serverBuff);
        fflush(stdout);
        write(STDOUT_FILENO, buffer, 100);
        fflush(stdout);
        for(size_t i = 0; i <= 99; ++i)
            buffer[i] = '\0';
    }

    close(acceptResultfd);
    
    }
    exit(EXIT_SUCCESS);

}