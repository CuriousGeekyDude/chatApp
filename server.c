#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "error_functions.c"
#include "path.c"

int main(int argc, char* argv[]) 
{
    char buffer[100];
    struct sockaddr_un addr;
    int fd, bindResult, acceptResultfd, numRead;

/*A socket is created and then binded to the
address "socketPathName" that is defined in "path.c" */

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



/*Server that has just started to listen, repeatedly tries
to accept any incoming connection requests from clients. The
server can only handle one client at a time. The moment 
that a client successfully connects to the server they can 
exchange messages, starting from the client. Unfortunately, 
they can only exchange one message at a time each, for otherwise
their message will get intermingled.*/

//fflush() is used to ensure messages are immediately 
//written to the output file from the kernel's buffer cache.

    while(true) {
    acceptResultfd = accept(fd,NULL,NULL);

    if(acceptResultfd > 0)
        printf("client connected\n");
    else
        continue;
    
    while(numRead = read(acceptResultfd, buffer, 100) > 0) {
        printf("client: ");
        fflush(stdout);
        write(STDOUT_FILENO, buffer, 100);
        fflush(stdout);
        initializeBuffer(buffer, 100);

        printf("server: ");
        fflush(stdout); 
        read(STDIN_FILENO, buffer, 100);                   
        write(acceptResultfd, buffer, 100);
    }

    close(acceptResultfd);
    
    }
    exit(EXIT_SUCCESS);

}