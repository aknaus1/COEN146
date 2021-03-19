//Lab5 Server
#include "lab5.h"

//getChecksum()
int getChecksum(Packet packet) {
    packet.header.cksum = 0;
    int checksum = 0;
    char *ptr = (char *)&packet;
    char *end = ptr + sizeof(Header) + packet.header.len;
    //Find the error (if any) from the remaining part getChecksum() function and correct it
    //******STUDENT WORK******
    while (ptr < end) {
        checksum ^= *ptr++;
    }
    return checksum;
}

// printPacket()
void printPacket(Packet packet) {
    printf("Packet{ header: { seq_ack: %d, len: %d, cksum: %d }, data: \"",
           packet.header.seq_ack,
           packet.header.len,
           packet.header.cksum);
    fwrite(packet.data, (size_t)packet.header.len, 1, stdout);
    printf("\" }\n");
}

//Server sending ACK to the client
void serverSend(int sockfd, const struct sockaddr *address, socklen_t addrlen, int seqnum) {
    Packet packet;
    //Prepare the packet headers
    //send the packet
    //******Student Work******
    int random = (rand() % (((10 + 1) - 1) + 1));
    printf("rand: %d\n", random);
    if (random < 7) {
        packet.header.seq_ack = seqnum;
        printf("sending the right seq\n");
    } else {
        packet.header.seq_ack = 50;
        printf("sending the wrong seq\n");
    }
    packet.header.len = 0;
    if (random < 5) {
        packet.header.cksum = getChecksum(packet);
        printf("sending the right cehck\n");
    } else {
        printf("sending the wrong check\n");
    }
    
    sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);

    printf("\t Server: sending ACK %d, checksum %d\n", packet.header.seq_ack, packet.header.cksum);
}

//Server receiving packet
Packet serverReceive(int sockfd, struct sockaddr *address, socklen_t *addrlen, int seqnum) {
    Packet packet;
    while (1) {
        //recv packets from the client
        //******Student Work******
        recvfrom(sockfd, &packet, sizeof(packet), 0, address, addrlen);

        // Print received packet
        printPacket(packet);

        if (packet.header.cksum != getChecksum(packet)) {
            printf("Bad checksum, expected checksum was: %d\n", getChecksum(packet));
            serverSend(sockfd, address, *addrlen, !seqnum);
        } else if (packet.header.seq_ack != seqnum) {
            printf("Bad seqnum, expected sequence number was:%d\n", seqnum);
            //******STUDENT WORK******
            serverSend(sockfd, address, *addrlen, !seqnum);
        } else {
            printf("Good packet\n");
            //******STUDENT WORK******
            serverSend(sockfd, address, *addrlen, seqnum);
            break;
        }
    }
    return packet;
}

int main(int argc, char *argv[]) {
    // check arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <port> <outfile>\n", argv[0]);
        exit(1);
    }
    // seed the RNG
    srand((unsigned)time(NULL));

    // create a socket
    //******STUDENT WORK******
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Failure to setup an endpoint socket");
        exit(1);
    }

    // initialize the server address structure using argv[1]
    struct sockaddr_in address;
    //******STUDENT WORK******
    address.sin_family = AF_INET;
    address.sin_port = htons(atoi(argv[1]));
    address.sin_addr.s_addr = INADDR_ANY;

    // bind the socket
    // [Question]: Do we need to bind the socket on the client side? Why?/Why not?
    //******STUDENT WORK******
    if ((bind(sockfd, (struct sockaddr *)&address, sizeof(struct sockaddr))) < 0) {
        perror("Failure to bind server address to the endpoint socket");
        exit(1);
    }

    // open file using argv[2]
    //******STUDENT WORK******
    int fp = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644);

    // get file contents from client
    int seqnum = 0;
    Packet packet;
    struct sockaddr_in clientaddr;
    socklen_t clientaddr_len = sizeof(clientaddr);
    do {
        packet = serverReceive(sockfd, (struct sockaddr *)&clientaddr, &clientaddr_len, seqnum);
        //******STUDENT WORK******
        write(fp, packet.data, packet.header.len);
        seqnum = !seqnum;
    } while (packet.header.len != 0);

    close(fp);
    close(sockfd);
    return 0;
}