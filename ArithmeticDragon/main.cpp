#include <iostream>
#include <sstream>
#include <cstdlib>
#include <list>

#include "unit.h"
#include "hero.h"
#include "dragon.h"
#include "troll.h"

using namespace std;


void showHealth(const Hero &hero, const Dragon &dragon)
{
    cout << "Hero health: " << hero.getHealth()
        << ", Enemy health: " << dragon.getHealth() << endl;
}

list<Enemy *> generateDragonList()
{
    list<Enemy *> competitors;
    competitors.push_back(new GreenDragon());
    competitors.push_back(new RedDragon());
    competitors.push_back(new BlackDragon());
    competitors.push_back(new BlueTroll());
    competitors.push_back(new YellowTroll());

    return competitors;
}


bool playGame(Hero &hero, list<Enemy*> &enemies)
{
    bool gameOver = false;

    for (list<Enemy *>::iterator enemy = enemies.begin();
            enemy != enemies.end(); enemy++)
    {
        cout << "You have met a new " << (*enemy)->color() << " beast. Fight!" << endl;

        while ((*enemy)->isAlive() && hero.isAlive())
        {
            hero.attack(**enemy);
        }
        if (!hero.isAlive())
        {
            gameOver = true;
            break;
        }
        else // dragon was killed!
        {
            hero.addScores(Dragon::c_killScores);
            cout << "Ough! You have killed an enemy!" << endl;
        }
    }
    return gameOver;
}


int main() {
    Hero hero;
    list<Enemy *> enemies = generateDragonList();
    bool gameResult = playGame(hero, enemies);

    if (gameResult) {
        cout << "Game over! Your score is: " << hero.getScores() << endl;
    } else {
        cout << "You win! Your score is: " << hero.getScores() << endl;
    }
    for (list<Enemy *>::iterator enemy = enemies.begin();
         enemy != enemies.end(); enemy++)
        delete *enemy;
    return 0;
}