//
// Created by pavel on 3/28/18.
//

#ifndef UNTITLED6_INTEGRAL_H
#define UNTITLED6_INTEGRAL_H

#include "sstream"
#include "cmath"

double function(double x){                                                        //function value calculation
    return x*x*exp(x) - tan(sqrt(x));
}

double integrate(double lower_limit, double upper_limit, int number){               //number - number of integr steps
    double current_result = 0;                                                    //calculating integral
    double step = (upper_limit-lower_limit)/number;
    for(int i = 0; i<number; i++)
        current_result += step * function(lower_limit+(i+1)*step);
    return current_result;
}

#endif //UNTITLED6_INTEGRAL_H
