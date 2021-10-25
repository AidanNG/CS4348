#include "header.h"

long consumer(){
/***********************************************************
* void producer()
* Author:      rxd
* Date:        4 October 2020
* Description: Opens shared memory
*              and use available
*              semaphores to consume
*              items from the buffer
*              asynchronously.
* Parameters:
*              None
************************************************************/

    long _numItemsGrabbed = 0; //local var to store num of items consumed by this consumer

    //open shared memory
    int fd = shm_open("sharedMem", O_RDWR, S_IRWXU);
    buffer* b = (buffer*)(mmap(NULL, sizeof(buffer), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));

    //request access
    sem_wait(&b->occupied);
    sem_wait(&b->cmut);

    //take items from the buffer and increment _numItemsGrabbed
    while(b->numItemsConsumed<=b->numItemsProduced){
        int item = b->buf[b->nextout];
        if(item) _numItemsGrabbed++;
        b->numItemsConsumed++;
        b->nextout++;
        b->nextout %= BUFFSIZE;
    }

    //signal that this consumer is finished
    sem_post(&b->cmut);
    sem_post(&b->empty);

    //cleanup & ret
    munmap((char*)b, 0);
    return _numItemsGrabbed;
}

