/*
* Name: Andrew Knaus
* Date: 2/1/2021
* Title: Lab4 - Step 1
* Description: TCP Multithreaded Server   
*/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <pthread.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define domain AF_INET // IPv4 protocol
#define type SOCK_STREAM // TCP (reliable, connection oriented)
#define protocol 0 // Protocol value for Internet Protocol (IP)
#define BSIZE 1024 // Buffer Size
#define NCONN 5 // Number of threads

int created_threads;
pthread_mutex_t lock;

typedef struct {
    int sockfd;
    struct sockaddr_in *serverAddr;
} HOLDER;

void* send_file(void* arg) {
    int sockfd = *((int *)arg);
    char buffer[BSIZE];

    pthread_mutex_lock(&lock);

    char fname[13] = "newfile";
    fname[7] = '0' + created_threads;
    char* addr = ".txt";
    strcat(fname, addr);

    created_threads++;

    pthread_mutex_unlock(&lock);

    FILE *fp = fopen("send.txt", "r");
    printf("Sending file...\n");
    write(sockfd, fname, 13);
    sleep(1);
    while(!feof(fp)) {
        int character = fread(buffer, 1, BSIZE, fp);
        send(sockfd, buffer, character, 0);
    }
    printf("File sent!\n");
    fclose(fp);
    close(sockfd);
    return NULL;
}


int main() {
    //Initialize Variables
    int sockfd, new_socket, sin_size, rb, fp;
    char buffer[BSIZE];
    struct sockaddr_in serverAddr, clientAddr;

    //Create an unbound socket in a communications domain, and return a file descriper
    if ((sockfd = socket(domain, type, protocol)) < 0) {
        perror("Cannot create socket");
        exit(1);
    }

    //Setup the server address to bind
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5000);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    //Bind socket to the address and port number specified in addr
    if(bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr)) < 0) {
        perror("Failed to bind server address to the endpoint socket");
        exit(1);
    }

    //Put server socket in passive mode, where it waits for the client to approach the server to make a connection
    listen(sockfd, NCONN); //Can queue 5 client requests
    sin_size = sizeof(struct sockaddr_in);

    //Initialize threads and mutex lock
    pthread_t threads[NCONN];
    created_threads = 1;
    pthread_mutex_init(&lock, NULL);

    printf("Server waiting for client\n");
    for(int i = 0; i < NCONN; i++) {
        if((new_socket = accept(sockfd, (struct sockaddr *)&clientAddr, (socklen_t *)&sin_size)) < 0) {
            perror("Failed to accept connection to the client");
            exit(1);
        }
        int *arg = malloc(sizeof(*arg));
        *arg = new_socket;
        pthread_create(&threads[i], NULL, send_file, arg);
    }
    for(int i = 0; i < NCONN; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    close(sockfd);
    printf("Server closed. Goodbye.\n");
    return 0;
}