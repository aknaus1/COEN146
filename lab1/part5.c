/*
# Name: Andrew Knaus
# Date: 1 - 7 - 2021
# Title: Lab1 – step 5
# Description: Rewrite progrom using threads instead of processes
*/

#include <stdio.h> /* printf, stderr */
#include <sys/types.h> /* pid_t */
#include <unistd.h> /* fork */
#include <stdlib.h> /* atoi */
#include <errno.h> /* errno */
#include <pthread.h> 

typedef struct {
    int type; //0 for parent, 1 for child
    int delay; //dela
}HOLDER;

void* go(void* arg) {
    HOLDER *h = (HOLDER*)arg;
    int type  = (*h).type;
    int delay = (*h).delay;
    if (type == 0){
        // Parent process
        for (int i=0;i<100;i++) {
            printf("\t \t \t Parent Process %d \n",i);
            usleep(delay);
        }
    }
    else{
        // Child process
        for (int i=0;i<100;i++) {
            printf("Child process %d\n",i);
            usleep(delay);
        }
    }
    return NULL;
}

/* main function with command-line arguments to pass */
int main(int argc, char *argv[]) {
    pid_t pid;
    int i, n = atoi(argv[1]); // n microseconds to input from keyboard for delay
    pthread_t threads[2];
    for(int i = 0; i < 2; i++) {
        HOLDER *h;
        h = malloc(sizeof(HOLDER));
        (*h).delay = n;
        (*h).type = i;
        pthread_create(&threads[i], NULL, go, (void *)h);
    }
    for (int i = 0; i < 2; i++) {
      pthread_join(threads[i],NULL);
    }
    return 0;
}