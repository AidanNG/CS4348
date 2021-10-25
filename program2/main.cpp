#include "header.h"

using namespace std;

int main(int argc, char* argv[]){
    //create shared memory, place buffer
    int fd = shm_open("sharedMem", O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);
    ftruncate(fd, sizeof(buffer));
    buffer* b = (buffer*)mmap(NULL, sizeof(buffer), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    //initialize semaphores
    sem_init(&(b->occupied), 1, 0);
    sem_init(&(b->empty), 1, BUFFSIZE);
    sem_init(&(b->pmut), 1, 1);
    sem_init(&(b->cmut), 1, 1);
    //initialize buffer variables
    b->nextin = b->nextout = b->numItemsProduced = b->numItemsConsumed = b->consumerNumber = 0;

    int pid[11]; //create 11 processes so that the grandparent process is of neither consumer nor producer
    int items[11]; //array to store the number of items that each consumer() grabs

    for(int i = 0; i<11; i++){
        pid[i] = 0;
        pid[i] = fork();
        if(pid[i]==0){ //Child
            if(pid[0]==0){ //Producer
                producer();
            }
            else{ //Consumer
                items[i] = consumer();
            }
            exit(0);
        }
        else{ //Parent
            continue;
        }
    }

    //necessary cleanup
    munmap((char*)b, 0);
    shm_unlink("sharedMem");
    sem_destroy(&(b->occupied));
    sem_destroy(&(b->empty));
    sem_destroy(&(b->pmut));
    sem_destroy(&(b->cmut));

    wait(NULL);
    wait(NULL);

    //output
    int status;
    for(int i = 10; i>=0; i--){
        kill(pid[i],SIGKILL);
        if(i!=0) cout<<"Consumer "<<i<<" obtained "<<items[i]<<" items."<<endl;
    }
}
