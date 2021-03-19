#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

typedef struct
{
    int	seq_ack; // SEQ for data and ACK for Acknowledgement
    int len; // Length of the data in byes (zero for ACKS)
    int cksum; // Checksum calculated (by byte)
} Header;

typedef struct
{
    Header	header;
    char	data[10];
} Packet;
