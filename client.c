#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include "error_functions.c"
#include "path.c"


int main(int argc, char* argv[])
{
    int fd, connectResult;
    pthread_t WriteThread;
    struct sockaddr_un addr;


/*The client first initializes the sockaddr_un structure
to the address that the server is binded to: "socketPathName".
That address is defined in "path.c" file.*/

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socketPathName, sizeof(addr.sun_path)-1);
    fd = socket(AF_UNIX, SOCK_STREAM, 0);

    while(true) {
    connectResult = connect(fd, (struct sockaddr*) &addr, sizeof(struct sockaddr_un));
    if(connectResult == 0)
        break; 
    }

/*If connect() was successful, then we will start exchanging 
messages with the server. It is necessary that we start the conversation.
We should only exchange one message at a time. Otherwise when 
we register a message that we are not prompted to do, we 
run the risk of intermingling our messages with that of the server's.*/

 
    int threadCreatRes;
    threadCreatRes = pthread_create(&WriteThread, NULL, &startThread, &fd);
    if(threadCreatRes != 0)
        errExitEN(threadCreatRes, "pthread_create"); 

    while(read(STDIN_FILENO, buffer, BuffSize) > 0) {
        write(fd, buffer, BuffSize);
        initializeBuffer(buffer, BuffSize);
    }
    close(fd);
    exit(EXIT_SUCCESS);
}