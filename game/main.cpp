#include <SFML/Graphics.hpp>
#include <random>
#include "dice.h"

using namespace sf;

int random(int a, int b)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    return std::uniform_int_distribution<int>(a, b)(gen);
}

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
    // for (int j = 0; j < countLine && countLine != 0; j++)
    // {
    //     table[i][j][0] = diceArr[bufferColumn[j]]->getValue();
    //     table[i][j][1] = bufferColumn[j];
    //     diceArr[bufferColumn[j]]->setPosition(table[i][j][2], table[i][j][3]);
    // }
}

// void removeDiceEnemy(int value, int i, int (&enemyTable)[3][3][2], Dice* (&diceArr)[200])
// {
//     int bufferColumn[3];
//     int countLine = 0;
//     bool found = false;
//     for (int j = 0; j < 3; j++)
//     {
//         if (enemyTable[i][j][0] != value)
//         {
//             bufferColumn[countLine] = enemyTable[i][j][1];
//             countLine++;
//         } else if (diceArr[enemyTable[i][j][1]] != nullptr && enemyTable[i][j][0] == value)
//         {
//             delete diceArr[enemyTable[i][j][1]];
//             diceArr[enemyTable[i][j][1]] = nullptr;
//             found = true;
//         }
//     }
//     if (!found)
//     {
//         return;
//     }
//     for (int j = 0; j < 3; j++) 
//     {
//         enemyTable[i][j][0] = 0;
//         enemyTable[i][j][1] = 0;
//     }
//     // for (int j = 0; j < countLine && countLine != 0; j++)
//     // {
//     //     enemyTable[i][j][0] = diceArr[bufferColumn[j]]->getValue();
//     //     enemyTable[i][j][1] = bufferColumn[j];
//     //     diceArr[bufferColumn[j]]->setPosition(17 + i * 56 + 3, 118 - j * 52);
//     // }
// }

void setPosMap(int (&table)[3][3][4], Dice* (&diceArr)[200])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3 && table[i][j][0] != 0; j++)
        {
            diceArr[table[i][j][1]]->setPosition(table[i][j][2], table[i][j][3]);
        }
    }
}

// void buffer(int i, bool turn, int value, int (&playerTable)[3][3][4], int (&enemyTable)[3][3][4], Dice* (&diceArr)[200])
// {
//     if (turn)
//     {
//         removeDice(value, i, enemyTable, diceArr);
//     } else
//     {
//         removeDice(value, i, playerTable, diceArr);
//     }
// }

int main()
{   
// стол на котором будет лежать кубик
    RectangleShape table({166, 166});
    table.setFillColor(Color(71, 201, 148));
    table.setPosition({17, 172});

// настройка пространства игрока
    RectangleShape drawTablePlayer[3];
    for (int i = 0; i < 3; i++) 
    {
        drawTablePlayer[i].setSize({54, 158});
        drawTablePlayer[i].setFillColor(Color(71, 201, 148));
        drawTablePlayer[i].setPosition({17 + i * 56, 342});
    }

// настройка пространства врага
    RectangleShape drawTableEnemy[3];
    for (int i = 0; i < 3; i++) 
    {
        drawTableEnemy[i].setSize({54, 158});
        drawTableEnemy[i].setFillColor(Color(71, 201, 148));
        drawTableEnemy[i].setPosition({17 + i * 56, 10});
    }
    
    int randomSeed = random(1, 100000);
    // unsigned int hightWin = 510;
    // unsigned int widthWin = 200;
    unsigned int hightWin = 510;
    unsigned int widthWin = 200;
    RenderWindow window(VideoMode({widthWin, hightWin}), "Dice game");
    
    int playerTable[3][3][4] = 
    { 
        {{0, 0, 20, 344}, {0, 0, 20, 392}, {0, 0, 20, 444}},
        {{0, 0, 76, 344}, {0, 0, 76, 392}, {0, 0, 76, 444}},
        {{0, 0, 132, 344}, {0, 0, 132, 392}, {0, 0, 132, 444}}
    };
    
    int enemyTable[3][3][4] = 
    {
        {{0, 0, 20, 118}, {0, 0, 20, 66}, {0, 0, 20, 14}},
        {{0, 0, 76, 118}, {0, 0, 76, 66}, {0, 0, 76, 14}},
        {{0, 0, 132, 118}, {0, 0, 132, 66}, {0, 0, 132, 14}}
    };

    Clock dilayEnemy;

    const int quantityDice = 200;
    Dice* diceArr[quantityDice] = {nullptr};
    int coutPlayerWon = 0;
    int countEnemyWon = 0;
    int countDice = 1;

    bool needNewDice = true;

    bool turn = randomSeed % 2;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (event->is<Event::MouseButtonPressed>() && turn && !needNewDice)
            {   
                Vector2i mousePos = Mouse::getPosition(window);
                Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                int flag = true;
                for (int i = 0; i < 3 && flag; i++)
                {
                    if (drawTablePlayer[i].getGlobalBounds().contains(mousePosF))
                    {
                       for (int j = 0; j < 3 && flag; j++)
                       {
                            if (playerTable[i][j][0] == 0)
                            {
                                playerTable[i][j][0] = diceArr[countDice]->getValue();
                                playerTable[i][j][1] = countDice;
                                // diceArr[countDice]->setPosition(playerTable[i][j][2], playerTable[i][j][3]);
                                removeDice(diceArr[countDice]->getValue(), i, enemyTable, diceArr);
                                // setPosMap(playerTable, diceArr);
                                countDice++;
                                turn = false;
                                needNewDice = true;
                                flag = false;
                            } 
                       }
                    }
                }
            }
        }
        coutPlayerWon = 0;
        countEnemyWon = 0;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (playerTable[i][j][0] != 0)
                {
                    coutPlayerWon++;
                }
                if (enemyTable[i][j][0] != 0)
                {
                    countEnemyWon++;
                }
            }
        }

        if (coutPlayerWon == 9)
        {
            window.close();
        }

        if (countEnemyWon == 9)
        {
            window.close();
        }

        if (needNewDice)
        {
            diceArr[countDice] = new Dice(countDice + randomSeed);
            diceArr[countDice]->setPosition(75, 230);
            dilayEnemy.restart();
            needNewDice = false;
        }
                      
        if (!turn && !needNewDice && dilayEnemy.getElapsedTime().asMilliseconds() > 1500)
        {
            bool flag = true;
            for (int i = 0; i < 3 && flag; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (enemyTable[i][j][0] == 0)
                    {
                        enemyTable[i][j][0] = diceArr[countDice]->getValue();
                        enemyTable[i][j][1] = countDice;
                        // diceArr[countDice]->setPosition(enemyTable[i][j][2], enemyTable[i][j][3]);
                        removeDice(diceArr[countDice]->getValue(), i, playerTable, diceArr);
                        // setPosMap(enemyTable, diceArr);
                        countDice++;
                        turn = true;
                        needNewDice = true;
                        flag = false;
                        break;
                    }
                }
            }
        }

        setPosMap(playerTable, diceArr);
        setPosMap(enemyTable, diceArr);

// рисунки
        window.clear(Color::Black);

        window.draw(table);
        
        for (int i = 0; i < 3; i++)
        {
            window.draw(drawTablePlayer[i]);
            window.draw(drawTableEnemy[i]);
        }

        if (!needNewDice && diceArr[countDice] != nullptr) {
            diceArr[countDice]->draw(window);
        }

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (playerTable[i][j][0] != 0 && diceArr[playerTable[i][j][1]] != nullptr) {
                    diceArr[playerTable[i][j][1]]->draw(window);
                }
                if (enemyTable[i][j][0] != 0 && diceArr[enemyTable[i][j][1]] != nullptr) {
                    diceArr[enemyTable[i][j][1]]->draw(window);
                }
            }
        }

        window.display();
    }
    for (int i = 0; i < quantityDice; i++) {
        if (diceArr[i] != nullptr) 
        {
            delete diceArr[i];
            diceArr[i] = nullptr;
        }
    }
}