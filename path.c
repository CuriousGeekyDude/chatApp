#define socketPathName "/home/feri/Desktop/sockets"


void initializeBuffer(char* buffer, size_t sizeOfBuffer) {
    for(size_t i = 0; i < sizeOfBuffer; ++i)
        buffer[i] = '\0';
}