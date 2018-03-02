#ifndef HERO_H_INCLUDED
#define HERO_H_INCLUDED

#include <string>
#include <iostream>

#include "dragon.h"
#include "unit.h"

class Hero: public Unit
{
    int scores;
public:
    static const int default_health = 100;
    static const int default_attackForce = 20;

    Hero(int health = default_health, int aForce = default_attackForce)
            :Unit(health, aForce), scores(0)
    {}
    void attack (Enemy &enemy)
    {
        std::string q = enemy.generateQuestion();
        std::cout << "question: " << q;
        int answer;
        std::cin >> answer;
        if (enemy.checkAnswer(answer))
        {
            enemy.getDamage(attackForce);
            std::cout << "Hit you, beast!" << std::endl;
        }
        else
        {
            getDamage(enemy.attackForce);
            std::cout << "Hero suffers..." << std::endl;
        }
    }
    void addScores(int number){
        scores += number;
    }

    int getScores(){
        return scores;
    }
};

#endif // HERO_H_INCLUDED