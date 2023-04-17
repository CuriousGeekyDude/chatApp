#include "path.c"

static sigjmp_buf env;
static char buffer[BuffSize];
                

/*Signal handler for SIGINT*/

void sigIntHandler(int sig) {

    /*Did not use longjmp() because of portability issues surrounding
      signal masks. On some systems when SIGINT gets blocked before the 
      call to the handler, it remains so after longjmp(), while on 
      some systems it is removed. siglongjmp() gaurantees the removal on 
      all systems.*/

    siglongjmp(env, 1); 
                        

}


/*Thread responsible for reading from the socket*/

void* clientThread(void* args) {
    int numRead;
    
    while(numRead = read(*((int*) args), bufferThread, BuffSize) > 0) {
        if(numRead == 0) {
            printf("\nserver closed the connection!\n");
            fflush(stdout);
            break;
        }
        printf("server: ");
        fflush(stdout);
        if(write(STDOUT_FILENO, bufferThread, BuffSize) == -1)
            errExit("write() of clientThread");
        initializeBuffer(bufferThread, BuffSize);
    }
    if(numRead == -1)
        errExit("read() of clientThread");
    
    pthread_exit(NULL);
}


int main(int argc, char* argv[])
{
    int fd, connectResult;
    pthread_t WriteThread;
    struct sockaddr_un addr;
    struct sigaction signalStructure;

    memset(&signalStructure, 0, sizeof(struct sigaction));
    signalStructure.sa_handler = &sigIntHandler;
    if(sigaction(SIGINT, &signalStructure, NULL) == -1)
        errExit("sigaction");


/*Using abstract namespace linux-specific feature instead of explicit pathname*/

    buffer[0] = '\0';
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path+1, "socketAddress", sizeof(addr.sun_path)-2);
    fd = socket(AF_UNIX, SOCK_STREAM, 0);

    while(true) {
    connectResult = connect(fd, (struct sockaddr*) &addr, sizeof(struct sockaddr_un));
    if(connectResult == 0)
        break; 
    }


    switch(sigsetjmp(env, 1)) {
 
    case 0:
        int threadCreatRes;
        threadCreatRes = pthread_create(&WriteThread, NULL, &clientThread, &fd);
        if(threadCreatRes != 0)
            errExitEN(threadCreatRes, "pthread_create"); 

        while(read(STDIN_FILENO, buffer, BuffSize) > 0) {
            write(fd, buffer, BuffSize);
            initializeBuffer(buffer, BuffSize);
        }
    break;

    default:
        close(fd);
        printf("\nclient closed\n");
        fflush(stdout);
        break;
    }
    exit(EXIT_SUCCESS);
}