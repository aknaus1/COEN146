/*
# Name: Andrew Knaus
# Date: 1 - 21 - 2021
# Title: Lab3 – step3.c
# Description: Write a C file that computes how much time elapses from when the client clicks on the link until the client receives the object
#               a) standard
#               b) 6 additional objects
#               c) n parallel TCP connections for persisten and non-persistent
*/

#include <stdio.h> //printf()
#include <math.h> //ciel()

int main() {
    double n = 2; //n parallel TCP connections

    int rtt1 = 3; //RTT for local DNS cache
    int rtt2 = 20; //RTT for non-local DNS server 1
    int rtt3 = 26; //RTT for non-local DNS server 2
    
    int sumBaseRTT = rtt1 + rtt2 + rtt3; //Sum of RTT for DNS servers
    
    int rtthttp = 47; //RTT between local host and Web server
    
    int totala = sumBaseRTT + 2 * rtthttp; //2a
    int totalb = totala + 6 * 2 * rtthttp; //2b
    int totalc = totala + ceil(6 / n) * rtthttp; //2c: Persistent
    int totald = totala + ceil(6 / n) * 2 * rtthttp; //2c: Non-Persistent

    printf("3a: %d ms, 3b: %d ms, 3c Persistent: %d ms, 3c Non-Persistent: %d ms\n", totala, totalb, totalc, totald);
}