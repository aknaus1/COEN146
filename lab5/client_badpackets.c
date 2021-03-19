//Lab5 Client
#include "lab5.h"

int seqnum = 0;

// function to clear buffer
void clearBuf(char *b) {
    int i;
    for (i = 0; i < 10; i++)
        b[i] = '\0';
}

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

//clientSend()
void clientSend(int sockfd, const struct sockaddr *address, socklen_t addrlen, Packet packet) {
    while (1) {
        int random = (rand() % (((10 + 1) - 1) + 1));
        printf("rand: %d\n", random);
        if (random < 7) {
            packet.header.seq_ack = seqnum;
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
        printf("Client: sending packet\n");
        //******STUDENT WORK******
        sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);

        // receive an ACK from the server
        Packet recvpacket;
        //******STUDENT WORK******
        recvfrom(sockfd, &recvpacket, sizeof(recvpacket), 0, (struct sockaddr *)address, &addrlen);

        // log what was received
        printf("Received ACK %d, checksum %d - ", recvpacket.header.seq_ack, recvpacket.header.cksum);

        //print received packet
        //******STUDENT WORK******
        printPacket(packet);

        // validate the ACK
        if (recvpacket.header.cksum != getChecksum(recvpacket)) {
            // bad checksum, resend packet
            printf("Bad checksum, expected checksum was: %d\n", getChecksum(recvpacket));
        } else if (recvpacket.header.seq_ack != packet.header.seq_ack) {
            // incorrect sequence number, resend packet
            printf("Bad seqnum, expected sequence number was: %d\n", packet.header.seq_ack);
        } else {
            // good ACK, we're done
            printf("Good ACK\n");
            break;
        }
    }
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
        perror("Failure to setup an endpoint socket");
        exit(1);
    }

    // initialize the server address structure using argv[2] and argv[1]
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    //******STUDENT WORK******

    socklen_t addr_len = sizeof(address);
    struct hostent *host;
    host = (struct hostent *)gethostbyname(argv[1]);
    address.sin_family = AF_INET;
    address.sin_port = htons(atoi(argv[2]));
    address.sin_addr = *((struct in_addr *)host->h_addr);

    // open file using argv[3]
    //******STUDENT WORK******
    int fp = open(argv[3], O_RDWR);

    // send file contents to server
    //-create a packet
    //-set the seq_ack field
    //-set the len field
    //-calculate the checksum
    //-send the packet
    //******STUDENT WORK******
    Packet packet;
    while (read(fp, packet.data, sizeof(packet.data)) > 0) {  // Reads from file then sends to server
        packet.header.seq_ack = seqnum;
        packet.header.len = strlen(packet.data);
        packet.header.cksum = getChecksum(packet);
        clientSend(sockfd, (struct sockaddr *)&address, addr_len, packet);
        seqnum = !seqnum;
        clearBuf(packet.data);
    }

    // send zero-length packet to server to end connection
    //******STUDENT WORK******
    packet.header.seq_ack = !seqnum;
    packet.header.len = 0;
    packet.header.cksum = getChecksum(packet);
    clientSend(sockfd, (struct sockaddr *)&address, addr_len, packet);

    // clean up
    //-close the sockets and file
    //******STUDENT WORK******
    close(fp);
    close(sockfd);

    return 0;
}
