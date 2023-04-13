#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>
#include "error_functions.c"
#define socketPathName "/home/feri/Desktop/sockets"



int main(int argc, char* argv[])
{
    char buffer[100], *clientBuff = "client: ";
    int fd, numRead;
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socketPathName, sizeof(addr.sun_path)-1);    

    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    connect(fd, (struct sockaddr*) &addr, sizeof(struct sockaddr_un));

    while(numRead = read(STDIN_FILENO, buffer, 100) > 0) {
        printf("%s", clientBuff);
        write(fd, buffer, 100);
        for(size_t i = 0; i <= 99; ++i)
            buffer[i] = '\0';
    }
    close(fd);
    exit(EXIT_SUCCESS);
}