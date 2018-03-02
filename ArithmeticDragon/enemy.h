//
// Created by pavel on 11/21/17.
//

#ifndef UNTITLED1_ENEMY_H
#define UNTITLED1_ENEMY_H

#include <string>
#include "unit.h"
#include "hero.h"

class Enemy: public Unit{

protected:
    int my_answer;
    std::string my_color;
public:
    Enemy(std::string color, int start_health, int _attackForce)
            :Unit(start_health, _attackForce), my_color(color)
    {}

    virtual std::string generateQuestion() = 0;

    bool checkAnswer(int answer) const
    {
        return answer == my_answer;
    }
    std::string color() const
    {
        return my_color;
    }
};

#endif //UNTITLED1_ENEMY_H
