#pragma once
#include "dice.h"
#include <SFML/Graphics.hpp>


void removeDice(int value, int i, int (&table)[3][3][4], Dice* (&diceArr)[200]);
void removeDice(int value, int i, int (&table)[3][3]);
int columnScore(int i, int (&table)[3][3]);
void brainEnemy(int (&enemyTable)[3][3][4], int (&playerTable)[3][3][4], Dice* (&diceArr)[200], int &countDice, bool &turn, bool &needNewDice, sf::Clock &dilayEnemy);
void enemyTurn(int (&enemyTable)[3][3][4], int (&playerTable)[3][3][4], Dice* (&diceArr)[200], bool &turn, bool &needNewDice, sf::Clock &dilayEnemy, int &countDice);