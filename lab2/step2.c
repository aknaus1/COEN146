/*
# Name: Andrew Knaus
# Date: 1 - 14 - 2021
# Title: Lab2 – step 2
# Description: Write your C program to copy files (binary and text) using system calls
*/

#include <stdio.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <fcntl.h>
#include <time.h>

void fileCopy(char *fname) {
    char buffer[64];
    int fp[2];
    long int n1;

    fp[0] = open(fname, O_RDONLY);
    fp[1] = open("copy.txt", O_WRONLY | O_CREAT);
    
    while ((n1 = read(fp[0], buffer, sizeof(buffer))) > 0){
        write(fp[1], buffer, n1);
    }

    close(fp[0]);
    close(fp[1]);
}

int main(int argc, char *argv[]) {
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    if(argv[1] == NULL) {
        printf("Must include file name\n");
        return 0;
    }
    
    fileCopy(argv[1]);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time Used: %f\n", cpu_time_used);
}