#define socketPathName "/home/feri/Desktop/sockets"
#define BuffSize 4096
#include <string.h>
#include <sys/socket.h>
#include <signal.h>
#include <setjmp.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdio.h>
#include "error_functions.c"


static char bufferThread[BuffSize];



void initializeBuffer(char* buffer, size_t sizeOfBuffer) {
    for(size_t i = 0; i < sizeOfBuffer; ++i)
        buffer[i] = '\0';
}





