//
// Created by pavel on 4/29/18.
//

#ifndef PARALLEL_INTEGRAL_COMMANDS_H
#define PARALLEL_INTEGRAL_COMMANDS_H
#define MYPORT 1025 // the port users will be connecting to
#define BACKLOG 10 // how many pending connections queue will hold
#define MAXDATASIZE 500

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
#include "sstream"
#include "cmath"

double function(double x){                                                        //function value calculation
    return x*x*exp(x) - tan(sqrt(x));
}

void integrate(double lower_limit, double upper_limit, int number, double &result){
    double current_result = 0;
    double step = (upper_limit-lower_limit)/number;
    for(int i = 0; i<number; i++)
        current_result += step * function(lower_limit+(i+1)*step);
    result = current_result;
}

void send_command(int socket, int number_of_threads, double left_limit, double right_limit, double step){
    std::stringstream A;
    A << number_of_threads << " " << left_limit << " " << right_limit << " " << step;
    char command[MAXDATASIZE];
    A >> command;
    send(socket, command, MAXDATASIZE - 1, 0);
}


void receive_answer(int socket, double &result){
    char mes[500];
    recv(socket, mes, MAXDATASIZE - 1, 0);
    std::stringstream A;
    A << mes;
    double  current_result;
    A >> current_result;
    result = current_result;
};

#endif //PARALLEL_INTEGRAL_COMMANDS_H
