#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>
#include "error_functions.c"
#include "path.c"


int main(int argc, char* argv[])
{
    char buffer[100];
    int fd, numRead;
    struct sockaddr_un addr;


/*The client first initializes the sockaddr_un structure
to the address that the server is binded to: "socketPathName".
That address is defined in "path.c" file.*/

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socketPathName, sizeof(addr.sun_path)-1);
    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    connect(fd, (struct sockaddr*) &addr, sizeof(struct sockaddr_un));


/*If connect() was successful, then we will start exchanging 
messages with the server. It is necessary that we start the conversation.
We should only exchange one message at a time. Otherwise when 
we register a message that we are not prompted to do, we 
run the risk of intermingling our messages with that of the server's.*/

//fflush() is used to ensure messages are immediately 
//written to the output file from the kernel's buffer cache.
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