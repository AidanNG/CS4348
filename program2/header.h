#include <sys/mman.h>
#include <sys/stat.h> //For mode constants
#include <fcntl.h> //For O_ constants
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstddef>
#include <iostream>
#include <signal.h>

#define BUFFSIZE 1000

typedef struct buffer_s {
    long numItemsProduced;
    long numItemsConsumed;
    int consumerNumber;
    int consumerRet[10];
    int buf[BUFFSIZE];
    int nextin;
    int nextout;
    sem_t occupied;
    sem_t empty;
    sem_t pmut;
    sem_t cmut;
} buffer;

void producer();
long consumer();


