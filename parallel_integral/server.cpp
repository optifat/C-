#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctime>
#include <future>
#include <sys/wait.h>
#include <csignal>
#include <iostream>
#include "thread"
#include <vector>
#include "methods.h"

#define MYPORT 1025 // the port users will be connecting to
#define BACKLOG 10 // how many pending connections queue will hold
#define MAXDATASIZE 500


void sigchld_handler(int s)
{
    while(wait(NULL) > 0);
}


int main(int argc, char* argv[]) {


    std::stringstream A;
    if (argc == 5) {
        A << argv[1] << " " << argv[2] << " " << argv[3] << " " << argv[4];
    }
    else {
        std::cerr << "Wrong input! Please, enter number of connections, number of threads per connection and limits of "
                "integration \n";
        exit(1);
    }


    int number_of_connection, number_of_threads, number = 100000000;;
    double left_limit, right_limit;
    A >> number_of_connection >> number_of_threads >> left_limit >> right_limit;
    double limits_of_integration[number_of_connection + 1];
    for (int i = 0; i <= number_of_connection; i++) {
        limits_of_integration[i] = left_limit + i * (right_limit - left_limit) / number_of_connection;
    }
    double step = number / number_of_connection;

    //std::cout<<number_of_connection<<" "<<number_of_threads<<" "<<left_limit<<" "<<right_limit<<std::endl;

    time_t timer;
    int sockfd;
    std::vector<int> sockets;
    struct sockaddr_in my_addr; // my address information
    std::vector<struct sockaddr_in> their_addrs; // connector.s address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    my_addr.sin_family = AF_INET; // host byte order
    my_addr.sin_port = htons(MYPORT); // short, network byte order
    my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
    memset(&(my_addr.sin_zero), 0, 8); // zero the rest of the struct


    if (bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(1);
    }
    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }
    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
    while(true) {
        sin_size = sizeof(struct sockaddr_in);

        for (int i = 0; i < number_of_connection; i++) {
            int new_fd = 0;
            struct sockaddr_in new_addr;
            if ((new_fd = accept(sockfd, (struct sockaddr *) &new_addr, &sin_size)) == -1) {
                //perror("accept");
                //continue;
            } else {
                printf("Received request from Client: %s:%d\n",
                       inet_ntoa(new_addr.sin_addr), MYPORT);
            }
            sockets.push_back(new_fd);
            their_addrs.push_back(new_addr);
        }
        auto *results = new double[number_of_connection];
        double result = 0;

        if (!fork()) {
            timer = time(NULL);
            for (auto sock : sockets) {
                if (send(sock, ctime(&timer), 30, 0) == -1) {
                    perror("send");
                }
            }
            std::vector<std::thread> threads;
            for (int i = 0; i < number_of_connection; i++) {
                threads.emplace_back([=, &limits_of_integration, &results] {
                    std::stringstream B;
                    B << number_of_threads << " " << limits_of_integration[i] << " " << limits_of_integration[i + 1]
                      << " " << step;
                    std::string command;
                    char mes[500];
                    command = B.str();
                    send(sockets[i], &command[0u], command.length(), 0);
                    recv(sockets[i], mes, MAXDATASIZE - 1, 0);   //receives but what's later?
                    std::stringstream C;
                    C << mes;
                    C >> results[i];
                    close(sockets[i]);
                });
            }

            for (auto &th: threads) {
                th.join();
            }

            for (int i = 0; i < number_of_connection; i++)
                result += results[i];
        }
        if(result) {
            std::cout << result << std::endl;
            exit(0);
        }
    }
    return 0;
}