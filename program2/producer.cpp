#include "header.h"

void producer(){
/***********************************************************
* void producer()
* Author:      rxd
* Date:        4 October 2020
* Description: Opens shared memory
*              and use available
*              semaphores to store
*              items into the buffer
*              asynchronously so children
*              may consume them.
* Parameters:
*              None
************************************************************/

    //open shared mem object
    int fd = shm_open("sharedMem", O_RDWR, S_IRWXU);
    buffer *b = (buffer*)(mmap(NULL, sizeof(buffer), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));

    //request access
    sem_wait(&b->empty);
    sem_wait(&b->pmut);

    //generate and place items into buffer
    for(long i = 0; i<1000000; i++){
        b->buf[b->nextin] = i;
        b->numItemsProduced++;
        b->nextin++;
        b->nextin %= BUFFSIZE;
    }

    //signal that we are done
    sem_post(&b->pmut);
    sem_post(&b->occupied);

    //cleanup
    munmap((char*)b, 0);
}

