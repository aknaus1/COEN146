/*
* Name: Andrew Knaus
* Date: 2/1/2021
* Title: Lab4 - Step 1
* Description: TCP Server   
*/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define domain AF_INET // IPv4 protocol
#define type SOCK_STREAM // TCP (reliable, connection oriented)
#define protocol 0 // Protocol value for Internet Protocol (IP)
#define BSIZE 1024 //Buffer Size

int send_file(int sockfd) {
    char buffer[BSIZE];
    FILE *fp = fopen("send.txt", "r");
    printf("Sending file...\n");
    while(!feof(fp)) {
        int character = fread(buffer, 1, BSIZE, fp);
        send(sockfd, buffer, character, 0);
    }
    printf("File sent!\n");
    fclose(fp);
    close(sockfd);
    return 1;
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
    listen(sockfd, 5); //Can queue 5 client requests
    sin_size = sizeof(struct sockaddr_in);

    printf("Server waiting for client\n");
    //Extracts the firbt connection request on the queue of pending connections for the listening socket
    //Creates a new connected socket, and returns a new file descriptor referring to that socket
    if((new_socket = accept(sockfd, (struct sockaddr *)&clientAddr, (socklen_t *)&sin_size)) < 0) {
        perror("Failed to accept connection to the client");
        exit(1);
    }

    send_file(new_socket);

    close(sockfd);
    printf("Server closed. Goodbye.\n");
    return 0;
}