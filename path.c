#define socketPathName "/home/feri/Desktop/sockets"
#define BuffSize 4096

static char buffer[BuffSize];
static char bufferThread[BuffSize];

void initializeBuffer(char* buffer, size_t sizeOfBuffer) {
    for(size_t i = 0; i < sizeOfBuffer; ++i)
        buffer[i] = '\0';
}

void* startThread(void* args) {

    while(read(*((int*) args), bufferThread, BuffSize) > 0) {
        if(write(STDOUT_FILENO, bufferThread, BuffSize) == -1)
            errExit("write of writeThread");
        initializeBuffer(bufferThread, BuffSize);
    }
    
    
    return NULL;
}




