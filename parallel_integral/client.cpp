#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <future>
#include <vector>
#include "sstream"
#include "methods.h"
#include "mutex"

#define PORT 1025 /*This is the port for the client server connection*/
#define MAXDATASIZE 500 // max number of bytes we can get at once




int main(int argc, char *argv[]) {
    int sockfd, numbytes;
    char buf[MAXDATASIZE];
    bool flag = false;

    struct hostent *he;
    struct sockaddr_in their_addr; // connector.s address information

    //***************************block of code finds the localhost IP
    char hostn[400]; //placeholder for the hostname
    char ipadd[400]; //placeholder for my IP address

    struct hostent *hostIP; //placeholder for the IP address

    //if the gethostname returns a name then the program will get the ip address using gethostbyname
    if((gethostname(hostn, sizeof(hostn))) == 0)
    {
        hostIP = gethostbyname(hostn); //the netdb.h function gethostbyname

    }
    else
    {
        printf("ERROR:FC4539 - IP Address not found."); //error if the hostname is not found
    }
    //****************************************************************


    if (argc != 3)
    {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    if ((he=gethostbyname(argv[1])) == NULL)
    { // get the host info
        perror("gethostbyname");
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }
    their_addr.sin_family = AF_INET; // host byte order
    their_addr.sin_port = htons(PORT); // short, network byte order
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    memset(&(their_addr.sin_zero), 0, 8); // zero the rest of the
    if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("connect");
        exit(1);
    }
    if ((numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1)
    {
        perror("recv");
        exit(1);
    }
    buf[numbytes] = 0;
    printf("\n\nLocalhost: %s\n", inet_ntoa(*(struct in_addr *)hostIP->h_addr));
    printf("Local Port: %d\n", PORT);
    printf("Remote Host: %s\n", inet_ntoa(their_addr.sin_addr));
    printf("Received daytime data: %s\n",buf);
    char mes[500];
    bzero(mes, MAXDATASIZE);

    std::cout<<recv(sockfd, &mes, MAXDATASIZE, 0)<<std::endl;

    std::stringstream A, B;
    int number_of_threads = 0;
    double left_limit, right_limit, step;
    A << mes;
    A >> number_of_threads >> left_limit >> right_limit >> step;


    //doesn't count integral properly

    double limits_of_integration[number_of_threads];

    for(int i = 0; i<=number_of_threads; i++)
        limits_of_integration[i] = left_limit + i*(right_limit - left_limit)/number_of_threads;

    double new_step = step/number_of_threads;
    std::vector<std::thread> threads;
    double *results = new double[number_of_threads];

    for(int i = 0; i<number_of_threads; i++)
        threads.push_back(std::thread(integrate, limits_of_integration[i], limits_of_integration[i+1],
                                      new_step, std::ref(results[i])));



    for(auto &th : threads)
        th.join();


    double result = 0;

    for (int i = 0; i<number_of_threads; i++)
        result += results[i];

    std::stringstream C;
    bzero(mes, MAXDATASIZE);
    std::string answer;
    C << result;
    answer = C.str();
    send(sockfd, &answer[0u], answer.length(), 0);    //doesn't send message
    return 0;
}
                  //client is ready
