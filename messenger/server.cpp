#include <cstdio>
#include <cstdlib>
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

#define MYPORT 1025 // the port users will be connecting to
#define BACKLOG 10 // how many pending connections queue will hold
#define MAXDATASIZE 500


void sigchld_handler(int s)
{
    while(wait(NULL) > 0);
}
int main()
{
    time_t timer;
    int sockfd, new_fd_1, new_fd_2; // listen on sock_fd, new connection on new_fd
    struct sockaddr_in my_addr; // my address information
    struct sockaddr_in their_addr_1, their_addr_2 ; // connector.s address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }
    if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1)
    {
        perror("setsockopt");
        exit(1);
    }
    my_addr.sin_family = AF_INET; // host byte order
    my_addr.sin_port = htons(MYPORT); // short, network byte order
    my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
    memset(&(my_addr.sin_zero), 0, 8); // zero the rest of the struct


    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }
    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("listen");
        exit(1);
    }
    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }
    while(true)  // main accept() loop
    {
        sin_size = sizeof(struct sockaddr_in);
        if ((new_fd_1 = accept(sockfd, (struct sockaddr *)&their_addr_1,&sin_size)) == -1)
        {
            //perror("accept");
            continue;
        }
        else{
            printf("Received request from Client: %s:%d\n",
                   inet_ntoa(their_addr_1.sin_addr),MYPORT);
        }

        if ((new_fd_2 = accept(sockfd, (struct sockaddr *)&their_addr_2,&sin_size)) == -1)
        {
            //perror("accept");
            continue;
        }
        else{
            printf("Received request from Client: %s:%d\n",
                   inet_ntoa(their_addr_2.sin_addr),MYPORT);
        }

        if (!fork())
        { // this is the child process
            //close(sockfd); // child doesn.t need the listener
            timer = time(NULL);
            if (send(new_fd_1, ctime(&timer), 30, 0) == -1)
                perror("send");
            if (send(new_fd_2, ctime(&timer), 30, 0) == -1)
                perror("send");
            //close(new_fd);
            //const char* message;
            //scanf(message);
            char mes[500];
            char buf[500];
            bool flag = false;

            //while (true) {
                bzero(buf, 256);
                bzero(mes, 256);
                auto a = std::thread([&]{
                    while(!flag) {
                        bzero(buf, 256);
                        recv(new_fd_2, buf, MAXDATASIZE - 1, 0);
                        if (*buf == '&'){
                            flag = true;
                            close(new_fd_1);
                            close(new_fd_2);
                            break;
                        }
                        else {
                            std::cerr << "fd_2:" << buf;
                            send(new_fd_1, buf, MAXDATASIZE - 1, 0);
                        }
                    }
                });

                //if (false) {
                //    break;
                //}

                auto b = std::thread([&]{
                    while(!flag){
                        bzero(mes, 256);
                        recv(new_fd_1, mes, MAXDATASIZE - 1, 0);
                        if (*mes == '&'){
                            flag = true;
                            close(new_fd_1);
                            close(new_fd_2);
                            break;
                        }
                        else {
                            std::cerr << "fd_1:" << mes;
                            send(new_fd_2, mes, MAXDATASIZE - 1, 0);
                        }
                    }
                });
                a.join();
                b.join();

            //}
            exit(0);
        }
        close(new_fd_1);
        close(new_fd_2);// parent doesn.t need this
    }
    return 0;
}
