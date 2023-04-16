#include "path.c"

static int acceptResultfd;

void* serverThread(void* args) {
    
    int socketFd = *((int*)args), numRead;

    label:      
        while(numRead = read(acceptResultfd, bufferThread, BuffSize) > 0) {
            printf("client: ");
            fflush(stdout);
            if(write(STDOUT_FILENO, bufferThread, BuffSize) == -1)
                errExit("write() of serverThread");
            initializeBuffer(bufferThread, BuffSize);
        }

        if(numRead == 0) {
                printf("\nclient closed the connection!\n");
                fflush(stdout);
                close(acceptResultfd);
            }
        if(numRead == -1)
            errExit("read() of serverThread");

    while(true) {
        acceptResultfd = accept(socketFd, NULL, 0);

        if(acceptResultfd > 0) {
            printf("New client has just connected!\n");
            goto label;
        }
        
    }
    
    pthread_exit(NULL);
}


int main(int argc, char* argv[]) 
{
    
    pthread_t WriteThread;
    int threadResult, fd, bindResult, numRead;;
    struct sockaddr_un addr;

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
    acceptResultfd = accept(fd,NULL,0);

    if(acceptResultfd > 0) {
        printf("client connected\n");
        fflush(stdout);
    }
    else
        continue;
    
    threadResult = pthread_create(&WriteThread, NULL, &serverThread, &fd);
    if(threadResult != 0)
        errExitEN(threadResult, "pthread_create[main thread, line: 80]");

    while(read(STDIN_FILENO, buffer, BuffSize) > 0) {

        write(acceptResultfd, buffer, BuffSize);
        initializeBuffer(buffer, BuffSize);
    
    }
    
    break;
    }
    printf("error or read 0: read() of main thread line 69\n");
    
    exit(EXIT_SUCCESS);

}