/*
* Name: Andrew Knaus
* Date: 2/1/2021
* Title: Lab4 - Step 1
* Description: TCP Multithreaded Client   
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

int write_file(int sockfd, char sbuffer[BSIZE]) {
    int bytes;
    char buffer[BSIZE];
    printf("Creating file: %s\n", sbuffer);
    FILE *fp = fopen(sbuffer, "w");
    printf("Retrieving File...\n");
    while((bytes = recv(sockfd, buffer, BSIZE, 0)) > 0) {
        printf("This ran for %s\n", sbuffer);
        fwrite(buffer, 1, bytes, fp);
        fflush(fp);
    }
    printf("File Recieved!\n");
    fclose(fp);
    return 1;
}

int main() {
    //Initialize Variables
    int sockfd, ws;
    char sbuffer[BSIZE];
    struct sockaddr_in serverAddr;
    struct hostent *host;

    //Create an unbound socket in a communications domain, and return a file descriper
    if ((sockfd = socket(domain, type, protocol)) < 0) {
        perror("Cannot create socket");
        exit(1);
    }

    //Setup the server address to bind
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5000);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    //Conenct the socket referred to by the file descriptor sockfd to the address specified by
    if(connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr))){
        perror("Failed to connect to the server");
        exit(1);
    }

    read(sockfd, sbuffer, BSIZE);

    write_file(sockfd, sbuffer);

    close(sockfd);
}