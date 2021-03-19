/*
# Name: Andrew Knaus
# Date: 1 - 7 - 2021
# Title: Lab1 – step 6 and 7
# Description: Circuit switching and Packet switching calculations
*/

#include <math.h> 
#include <stdio.h>

double factorial(int input) {
    double result = 1;
    for (int i = 1; i <= input; i++) {
        result *= i;
    }
    return result;
}

double sum(int min, int max, int nPSusers, double pPSusers, double pPSusersNotBusy) {
    double result = 0;
    for (int i = min; i <= max; i++) {
        result += (factorial(nPSusers) / (factorial(i) * factorial(nPSusers - i))) * pow(pPSusers, i) * pow(pPSusersNotBusy, nPSusers - i);
    }
    return result;    
}

int main() {
    int linkBandwidth; //The bandwidth of a network link
    int userBandwidth; //The bandwidth required for a given user
    int nCSusers; //The number of circuit switching users
    int nPSusers; //The number of packet switching users
    double tPSuser; //The percentage of time a packet switching user needs to transmit
    double pPSusers; //The probability that a given (specific) packet switching user is busy transmitting
    double pPSusersNotBusy; //The probability that one (specific) packet switching user is not busy transmitting

    linkBandwidth = 200; //in Mbps
    userBandwidth = 20; //in Mbps
    tPSuser = 0.10;
    nPSusers = 19;


    printf("Circuit switching scenario\n");
    nCSusers = linkBandwidth/ userBandwidth; //The number of circuit-switched users that can be supported
    printf("6a: %d\n", nCSusers);
    

    printf("Packet switching scenario\n");
    pPSusers = tPSuser; //The probability that a given (specific) user is busy transmitting
    printf("7a: %f\n", pPSusers);
    pPSusersNotBusy = 1 - pPSusers; //The probability that one specific other user is not busy
    printf("7b: %f\n", pPSusersNotBusy);

    printf("7c: %f\n" ,pow((1 - pPSusers), (nPSusers - 1)));
    printf("7d: %f\n", pPSusers * pow(pPSusersNotBusy, nPSusers - 1));
    printf("7e: %f\n", nPSusers * (pPSusers * pow(pPSusersNotBusy, nPSusers -1)));
    printf("7f: %g\n", pow(pPSusers, 10) * pow(pPSusersNotBusy, nPSusers - 10));
    printf("7g: %g\n", (factorial(nPSusers) /(factorial(10)*factorial(nPSusers - 10))) * pow(pPSusers, 10) * pow(pPSusersNotBusy, nPSusers - 10));
    printf("7h: %g\n", sum(11, nPSusers, nPSusers, pPSusers, pPSusersNotBusy));
}