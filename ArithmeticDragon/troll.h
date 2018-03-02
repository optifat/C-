//
// Created by pavel on 11/21/17.
//

#ifndef UNTITLED1_TROLL_H
#define UNTITLED1_TROLL_H

#include "enemy.h"
#include <string>
#include <sstream>

class Troll: public Enemy{

public:
    static const int c_killScores = 300;
    Troll(std::string color, int start_health, int _attackForce)
    :Enemy(color, start_health, _attackForce)
    {}

    virtual std::string generateQuestion() = 0;

};

class BlueTroll: public Troll{

    static const int c_blueTrollHealth = 50;
    static const int c_blueTrollAttackForce = 5;

public:

    BlueTroll()
            :Troll("Blue", c_blueTrollHealth, c_blueTrollAttackForce)
    {}

    std::string generateQuestion()
    {
        int number = 1 + rand()%100, result = 0, factor = 1;
        my_answer = number;
        std::stringstream question;
        while(number >= 1){
            result += factor*(number%2);
            number /= 2;
            factor*=10;
        }
        question << "Change number "<<my_answer<<" "<<result<<" from binary to decimal"<<std::endl;
        return question.str();
    };

};

class YellowTroll: public Troll{

    static const int c_yellowTrollHealth = 50;
    static const int c_yellowTrollAttackForce = 5;
public:

    YellowTroll()
            :Troll("Yellow", c_yellowTrollHealth, c_yellowTrollAttackForce)
    {}

    std::string generateQuestion(){
        int number = 1 + rand()%100, result = 0, factor = 1;
        my_answer = number;
        std::stringstream question;
        while(number >= 1){
            result += factor*(number%8);
            number /= 8;
            factor*=10;
        }
        question << "Change number "<<my_answer<<" "<<result<<" from octal to decimal"<<std::endl;
        return question.str();
    };
};

#endif //UNTITLED1_TROLL_H
