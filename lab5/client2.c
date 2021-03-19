//Aimee Kerr
//5/5/2020
//Lab 5
//This is the client of Lab 5
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "lab5.h"
// NOTE: beware of sign extension if the checksum is stored in a char, then converted to an int!
int getChecksum(Packet packet) {
    packet.header.cksum = 0;
    int checksum = 0;
    char *ptr = (char *)&packet;
    char *end = ptr + sizeof(Header) + packet.header.len;
    //Please find an error from the remaining part getChecksum() function
    //******STUDENT WORK******
    while (ptr < end) {
        checksum ^= *ptr;
        ptr++;
    }
    return checksum;
}
void logPacket(Packet packet) {
    printf("Packet{ header: { seq_ack: %d, len: %d, cksum: %d }, data: \"",
           packet.header.seq_ack,
           packet.header.len,
           packet.header.cksum);
    fwrite(packet.data, (size_t)packet.header.len, 1, stdout);
    printf("\" }\n");
}
int number = 1;
void ClientSend(int sockfd, const struct sockaddr *address, socklen_t addrlen,
                Packet packet) {
    while (1) {
        printf("%d\n", number);
        int random = (rand() % (((10 + 1) - 1) + 1));
        printf("rand: %d\n", random);
        if (random < 7) {
            packet.header.seq_ack = number;
            printf("sending the right seq\n");
        } else {
            packet.header.seq_ack = 70;
            printf("sending the wrong seq\n");
        }

        packet.header.len = strlen(packet.data);
        if (random < 5) {
            packet.header.cksum = getChecksum(packet);
            printf("sending the right check\n");
        } else {
            printf("sending the wrong check\n");
        }

        // send the packet
        printf("Sending packet\n");
        //******STUDENT WORK******
        logPacket(packet);
        sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);
        // receive an ACK from the server
        Packet recvpacket;
        //******STUDENT WORK******
        recvfrom(sockfd, &recvpacket, sizeof(recvpacket), 0, (struct sockaddr *)address, &addrlen);
        // log what was received
        printf("Received ACK %d, checksum %d - ", recvpacket.header.seq_ack,
               recvpacket.header.cksum);
        // validate the ACK
        if (recvpacket.header.cksum != getChecksum(recvpacket))  //******STUDENT WORK******)
        {
            // bad checksum, resend packet
            printf("Bad checksum, expected checksum was: %d\n",
                   getChecksum(recvpacket));  //******STUDENT WORK******);
        } else if (recvpacket.header.seq_ack != packet.header.seq_ack)
        //******STUDENT WORK******)
        {
            // incorrect sequence number, resend packet
            printf("Bad seqnum, expected sequence number was: %d\n",
                   packet.header.seq_ack);  //******STUDENT WORK******);
        } else {
            // good ACK, we're done
            printf("Good ACK\n");
            number = !number;
            break;
        }
    }
    printf("\n");
}
int main(int argc, char *argv[]) {
    // check arguments
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <ip> <port> <infile>\n", argv[0]);
        return 1;
    }
    // seed the RNG
    srand((unsigned)time(NULL));
    // create and configure the socket [name the socket sockfd]
    //******STUDENT WORK******
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Error\n");
        return 0;
    }
    // initialize the server address structure using argv[2] and argv[1]
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    //******STUDENT WORK******
    socklen_t length = sizeof(struct sockaddr);
    struct hostent *host;
    host = (struct hostent *)gethostbyname(argv[1]);
    address.sin_family = AF_INET;
    address.sin_port = htons(atoi(argv[2]));
    address.sin_addr = *((struct in_addr *)host->h_addr);
    // open file using argv[3]
    //******STUDENT WORK******
    int file, byte;
    file = open(argv[3], O_RDWR);
    // send file contents to server
    //-create a packet
    //-set the seq_ack field
    //-set the len field
    //-calculate the checksum
    //-send the packet
    //******STUDENT WORK******
    Packet pack;
    while ((byte = read(file, pack.data, sizeof(pack.data))) > 0) {  //Reads from file then sends to server
        //printf("Client sending file contents to server...\n")
        ClientSend(sockfd, (struct sockaddr *)&address, length, pack);
        memset(&pack, 0, sizeof(pack));
    }
    // send zero-length packet to server to end connection
    //******STUDENT WORK******
    pack.header.seq_ack = !number;
    pack.header.len = 0;
    pack.header.cksum = getChecksum(pack);
    pack.data[0] = '\0';
    ClientSend(sockfd, (struct sockaddr *)&address, length, pack);
    // clean up
    //-close the sockets and file
    //******STUDENT WORK******
    close(file);
    close(sockfd);
    return 0;
}