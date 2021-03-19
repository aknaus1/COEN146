/*
# Name: Andrew Knaus
# Date: 1 - 14 - 2021
# Title: Lab2 – step 1
# Description: Write your C program to copy files (binary and text) using functions
*/

#include <stdio.h>
#include <pthread.h>
#include <time.h>

void fileCopy(char *fname) {
    char buffer[64];
    FILE *fp, *fp2;
    size_t bytes;

    fp = fopen(fname, "r");
    fp2 = fopen("copy.txt", "w");
    
    while (0 < (bytes = fread(buffer, 1, sizeof(buffer), fp))){
        fwrite(buffer, 1 , bytes, fp2);
    }
    
    fclose(fp);
    fclose(fp2);
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