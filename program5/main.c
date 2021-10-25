/***************************************************************************
* File: main.c
* Author: Toby
* Modification History:
* 	Toby - 4 October 2020
* Procedures:
* 	main - main function that will call the necessary amount of producers and consumers, and simulate the 'market'
* 	consumer - Creatse a consumer that will buy from the producer
* 	producer - Creates a producer that will make the max amount of items within the limitations of the buffer
***************************************************************************/

#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
//initialize the correct amount for the buffer and the maximum items
#define max 1000000
#define bufSize 1000
//initialize semaphores and buffers
sem_t em;
sem_t f;
int total;
int in = 0;
int out = 0;
int buffer[bufSize];
pthread_mutex_t m;
/***************************************************************************
* void *producer(void *args)
* Author: Toby
* Date: Toby - 4 October 2020
* Modification History:
* 	Toby - 4 October 2020
* Description: makes a producer that will make a number of items for the market/consumers
*
* Parameters:
* 	none
**************************************************************************/
void *producer(){
    int item;
    int x = 0;
    while(true) {
        if(total == max) break;//if max items made, then break
        item = x+1; //makes some item that is one more than the last
        sem_wait(&em);//wait for empty signal
        pthread_mutex_lock(&m);
        buffer[in] = item;//add item to the buffer
        in = (in+1)%bufSize;
        total++;//increase total items made
        pthread_mutex_unlock(&m);
        sem_post(&f);//send full signal
    }
}
/***************************************************************************
* void *consumer(void *args)
* Author: Toby
* Date: 4 October 2020
* Modification History:
* 	Toby - 4 October 2020
* Description: creates a consumer function that will consume from the buffer
*
* Parameters:
*   *args   I/P     void    used to save the number of the consumer
* 	main 	O/P 	int 	Print the Consumer and the amount consumed/bought
**************************************************************************/
void *consumer(void *args){
    int counter =0;//counter to track amount consumed
    while(true) {
        sem_wait(&f);//wait until full signal
        pthread_mutex_lock(&m);
        int item = buffer[out];//remove item from buffer
        counter++;//item has been consumed
        out = (out+1)%bufSize;
        pthread_mutex_unlock(&m);
        sem_post(&em);//send empty signal
        if(total == max) break;//if all items are gone break
    }
    printf("Consumer %d: %d items\n",*((int *)args),counter);//print the consumer and the number of their items
}
/***************************************************************************
* int main()
* Author: Toby
* Date: 4 October 2020
* Modification History:
* 	Toby - 4 October 2020
* Description: calls uname to print the name and information about the current kernel by creating a utsname struct
*
* Parameters:
* 	main 	O/P 	int    Print the Consumer and the amount consumed/bought using the consumer function
**************************************************************************/
int main(){
    pthread_t pro,con[10];//initialize the threads
    pthread_mutex_init(&m, NULL);
    sem_init(&em,0,bufSize);//initalize the semaphores
    sem_init(&f,0,0);
    total=0;//reset total
    int num[10] = {1,2,3,4,5,6,7,8,9,10}; //storing the number of the consumer
    int pid[11];
    for(int i = 0; i < 11; i++) {//11 total for the 1 producer and the 10 consumers
        pid[i] = 0;
        pid[i] = fork();//fork the process
        if(i==0){//if the first process, create the producer
            pthread_create(&pro, NULL, (void *)producer, (void *)&num[i]);
        }//else create the consumers
        i++;
        pthread_create(&con[i], NULL, (void *)consumer, (void *)&num[i]);
    }
    //join the threads so that they end
    pthread_join(pro, NULL);
    for(int i = 0; i < 10; i++) {
        pthread_join(con[i], NULL);
    }
    //necessary destruction methods
    pthread_mutex_destroy(&m);
    sem_destroy(&em);
    sem_destroy(&f);
    printf("%d",total);
    return 0;
}
