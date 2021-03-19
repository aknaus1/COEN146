/*
# Name: Andrew Knaus
# Date: 1 - 14 - 2021
# Title: Lab2 – step 4
# Description: Write your C program to copy multiple files (say 10 files) by creating threads, 
#               each of which will be responsible of copying one file
*/

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <pthread.h>
#include <string.h>

void* fileCopy(void *arg) {
    char* fname = (char*) arg;

    char buffer[64];
    FILE *fp, *fp2;
    size_t bytes;

    char copy[strlen(fname)+4];
    copy[0] = 'c';
    copy[1] = 'o';
    copy[2] = 'p';
    copy[3] = 'y';
    strcat(copy, fname);
    
    fp = fopen(fname, "r");
    fp2 = fopen(copy, "w");
    
    while (0 < (bytes = fread(buffer, 1, sizeof(buffer), fp))){
        fwrite(buffer, 1 , bytes, fp2);
    }

    fclose(fp);
    fclose(fp2);

    return 0;
}

int main(int argc, char *argv[]) {
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    if(argv[1] == NULL) {
        printf("Must include file name\n");
        return 0;
    }

    pthread_t threads[argc-1];
    
    for(int i = 0; i < argc-1; i++) {
        pthread_create(&threads[i], NULL, fileCopy, (void*)argv[i+1]);
    }

    for (int i = 0; i < argc-1; i++) {
        pthread_join(threads[i],NULL);
    }

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time Used: %f\n", cpu_time_used);
}