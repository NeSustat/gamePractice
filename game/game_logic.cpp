#include "game_logic.h"
#include <SFML/Graphics.hpp>


// определяет какие кубики надо удалить и сдвигает те который удалять не надо
void removeDice(int value, int i, int (&table)[3][3][4], Dice* (&diceArr)[200])
{
    int bufferColumn[3];
    int countLine = 0;
    bool found = false;

    for (int j = 0; j < 3; j++)
    {
        if (table[i][j][0] != value && table[i][j][0] != 0)
        {
            bufferColumn[countLine] = table[i][j][1];
            countLine++;
        } else if (diceArr[table[i][j][1]] != nullptr && table[i][j][0] == value)
        {
            delete diceArr[table[i][j][1]];
            diceArr[table[i][j][1]] = nullptr;
            found = true;
        }
    }
    if (!found)
    {
        return;
    }
    for (int j = 0; j < 3; j++) 
    {
        table[i][j][0] = 0;
        table[i][j][1] = 0;
    }
    for (int j = 0; j < countLine && countLine != 0; j++)
    {
        table[i][j][0] = diceArr[bufferColumn[j]]->getValue();
        table[i][j][1] = bufferColumn[j];
        diceArr[bufferColumn[j]]->setPosition(table[i][j][2], table[i][j][3]);
    }
}


void removeDice(int value, int i, int (&table)[3][3])
{
    int bufferColumn[3];
    int countLine = 0;
    bool found = false;

    for (int j = 0; j < 3; j++)
    {
        if (table[i][j] != value && table[i][j] != 0)
        {
            bufferColumn[countLine] = table[i][j];
            countLine++;
        } else if (table[i][j] == value)
        {
            found = true;
        }
    }
    if (!found)
    {
        return;
    }
    for (int j = 0; j < 3; j++) 
    {
        table[i][j] = 0;
    }
    for (int j = 0; j < countLine && countLine != 0; j++)
    {
        table[i][j] = bufferColumn[j];
    }
}

int columnScore(int i, int (&table)[3][3])
{
    int count = 0;
    int arr[6][4] = 
    {
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}
    };
    for (int j = 0; j < 3; j++)
    {
        arr[table[i][j] - 1][0]++;
        for (int k = 1; k <=3; k++)
        {
            if (arr[table[i][j] - 1][k] == 0)
            {
                arr[table[i][j] - 1][k] = table[i][j];
                break;
            }
        }
    }
    for (int j = 0; j < 6; j++)
    {
        count += (arr[j][0] * (j + 1)) * arr[j][0];
    }
    return count;
}

// функция которая будет АНАЛизировать ходы робота
void brainEnemy(int (&enemyTable)[3][3][4], int (&playerTable)[3][3][4], Dice* (&diceArr)[200], int &countDice, bool &turn, bool &needNewDice, sf::Clock &dilayEnemy){
    int player[3][3];
    int enemy[3][3];
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            player[i][j] = playerTable[i][j][0];
            enemy[i][j] = enemyTable[i][j][0];
        }
    }
    
    int countDiceColumn[3] = {0, 0, 0};
    int maxDiff[3] = {-1, 0, 0};
    bool flagQuet = true;

    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if (enemy[i][j] != 0){
                countDiceColumn[i]++;
            }
        }
    }

    for (int i = 0; i < 3; i++){

        bool flag = true;

        int startPlayerScore = columnScore(i, player);
        removeDice(diceArr[countDice]->getValue(), i, player);
        int endPlaerScore = columnScore(i, player);
        int diffPlayer = startPlayerScore - endPlaerScore;

        int startEnemyScore = columnScore(i, enemy);
        for (int j = 0; j < 3 && flag; j++){
            if (enemy[i][j] == 0){
                enemy[i][j] = diceArr[countDice]->getValue();
                int endEnemyScore = columnScore(i, enemy);
                int diffEnemy = endEnemyScore - startEnemyScore;
                int diff = diffEnemy + diffPlayer;
                if (diff > maxDiff[1] || maxDiff[0] == -1){
                    maxDiff[1] = diff;
                    maxDiff[0] = i;
                    maxDiff[2] = countDiceColumn[i];
                } else if (diff == maxDiff[1] && maxDiff[2] > countDiceColumn[i]){
                    maxDiff[1] = diff;
                    maxDiff[0] = i;
                    maxDiff[2] = countDiceColumn[i];
                }
                flag = false;
                flagQuet = false;
            }
        }
    }
    if (flagQuet){
        return;
    }
    if (maxDiff[0] != -1){
        bool flag = true;
        for (int i = 0; i < 3 && flag; i++)
        {
            if (enemyTable[maxDiff[0]][i][0] == 0)
            {
                enemyTable[maxDiff[0]][i][0] = diceArr[countDice]->getValue();
                enemyTable[maxDiff[0]][i][1] = countDice;
                // diceArr[countDice]->setPosition(enemyTable[i][j][2], enemyTable[i][j][3]);
                removeDice(diceArr[countDice]->getValue(), maxDiff[0], playerTable, diceArr);
                // setPosMap(enemyTable, diceArr);
                countDice++;
                turn = true;
                needNewDice = true;
                flag = false;
                dilayEnemy.restart();
            } 
        }                  
    }
}

void enemyTurn(int (&enemyTable)[3][3][4], int (&playerTable)[3][3][4], Dice* (&diceArr)[200], bool &turn, bool &needNewDice, sf::Clock &dilayEnemy, int &countDice){
    bool flag = true;
    for (int j = 0; j < 3 && flag; j++){
        for (int i = 0; i < 3 && flag; i++){
            if (enemyTable[i][j][0] == 0){
                enemyTable[i][j][0] = diceArr[countDice]->getValue();
                enemyTable[i][j][1] = countDice;
                // diceArr[countDice]->setPosition(enemyTable[i][j][2], enemyTable[i][j][3]);
                removeDice(diceArr[countDice]->getValue(), i, playerTable, diceArr);
                // setPosMap(enemyTable, diceArr);
                countDice++;
                turn = true;
                needNewDice = true;
                flag = false;
                dilayEnemy.restart();
            }
        }
    }
}