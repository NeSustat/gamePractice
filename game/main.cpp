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

int randomBySeed(int seed, int a, int b)
{
    std::mt19937 gen(seed);
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
}

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

void rollDiceF(bool &turn, bool &needNewDice, bool &moveRollDice, 
    int &countDice, int &randomSeed, int &randomSeedDice, Dice* (&diceArr)[200], Clock &dilayEnemy,
        RectangleShape &rollDice, Texture &texture, Clock &textureTimer)
{
    int x, y;
    if (turn)
    {
        x = 130;
        y = 710; 
    } else 
    {
        x = 910;
        y = 160;
    }
    if (dilayEnemy.getElapsedTime().asMilliseconds() > 1500)
    {
        rollDice.move({0.0f, 0.0f});
        diceArr[countDice] = new Dice(countDice + randomSeed);
        diceArr[countDice]->setPosition(x + randomBySeed(countDice + randomSeedDice, 0, 50), y + randomBySeed(countDice + randomSeedDice, 0, 50));
        needNewDice = false;
        dilayEnemy.restart();
        return;
    }
    if (textureTimer.getElapsedTime().asMilliseconds() > 200)
    {
        rollDice.setPosition({x + randomBySeed(countDice + randomSeedDice, 0, 50), y + randomBySeed(countDice + randomSeedDice, 0, 50)});
        std::string filename = "pic/dice" + std::to_string(random(1, 6)) + ".png";
        if (texture.loadFromFile(filename)) 
        {
            rollDice.setTexture(&texture);
            
        }
        if (moveRollDice)
        {
            rollDice.move({0.0f, 5.0f});
            moveRollDice = !moveRollDice;
        } else
        {
            rollDice.move({-0.0f, -.0f});
            moveRollDice = !moveRollDice;
        }
        textureTimer.restart();
    }
}

int setColor(int i, int (&table)[3][3][4], Dice* (&diceArr)[200])
{
    int count = 0;
    int arr[6][4] = 
    {
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}
    };
    for (int j = 0; j < 3; j++)
    {
        arr[table[i][j][0] - 1][0]++;
        for (int k = 1; k <=3; k++)
        {
            if (arr[table[i][j][0] - 1][k] == 0)
            {
                arr[table[i][j][0] - 1][k] = table[i][j][1];
                break;
            }
        }
    }
    for (int j = 0; j < 6; j++)
    {
        if (arr[j][0] != 0)
        {
            for (int k = 1; k <= arr[j][0]; k++)
            {
                diceArr[arr[j][k]]->setColor(arr[j][0]);
            }
        }
        count += (arr[j][0] * (j + 1)) * arr[j][0];
    }
    return count;
}

int main()
{   
    Font font("C:/Windows/Fonts/consola.ttf");

    RectangleShape rollDice({100, 100});
    rollDice.setFillColor(Color::White);
    Texture texture;

// стол на котором будет лежать кубик
    RectangleShape tableEnemy({200, 200});
    tableEnemy.setFillColor(Color(71, 201, 148));
    tableEnemy.setOutlineThickness(10.f);
    tableEnemy.setOutlineColor(Color(138,102,66));
    tableEnemy.setPosition({890, 130});

    RectangleShape tablePlayer({200, 200});
    tablePlayer.setFillColor(Color(71, 201, 148));
    tablePlayer.setOutlineThickness(10.f);
    tablePlayer.setOutlineColor(Color(138,102,66));
    tablePlayer.setPosition({110, 680});

// настройка пространства игрока
    RectangleShape drawTablePlayer[3];
    for (int i = 0; i < 3; i++) 
    {
        drawTablePlayer[i].setSize({110, 340});
        drawTablePlayer[i].setFillColor(Color(71, 201, 148));
        drawTablePlayer[i].setOutlineThickness(10.f);
        drawTablePlayer[i].setOutlineColor(Color(138,102,66));
        drawTablePlayer[i].setPosition({420 + i * 120, 610});
    }

// настройка пространства врага
    RectangleShape drawTableEnemy[3];
    for (int i = 0; i < 3; i++) 
    {
        drawTableEnemy[i].setSize({110, 340});
        drawTableEnemy[i].setFillColor(Color(71, 201, 148));
        drawTableEnemy[i].setOutlineThickness(10.f);
        drawTableEnemy[i].setOutlineColor(Color(138,102,66));
        drawTableEnemy[i].setPosition({420 + i * 120, 50});
    }
    
    int randomSeed = random(1, 100000);
    int randomSeedDice = random(1, 100000);

    // unsigned int hightWin = 510;
    // unsigned int widthWin = 200;
    unsigned int hightWin = 1000;
    unsigned int widthWin = 1200;
    RenderWindow window(VideoMode({widthWin, hightWin}), "Dice game");
    RectangleShape endDisplay({widthWin, hightWin});
    endDisplay.setFillColor(Color(0, 0, 0, 200));

    Image icon({32, 32});
    icon.loadFromFile("pic/dice.png");
    window.setIcon(icon);
    
    int playerTable[3][3][4] = 
    { 
        {{0, 0, 425, 620}, {0, 0, 425, 730}, {0, 0, 425, 840}},
        {{0, 0, 545, 620}, {0, 0, 545, 730}, {0, 0, 545, 840}},
        {{0, 0, 665, 620}, {0, 0, 665, 730}, {0, 0, 665, 840}}
    };
    
    int enemyTable[3][3][4] = 
    {
        {{0, 0, 425, 280}, {0, 0, 425, 170}, {0, 0, 425, 60}},
        {{0, 0, 545, 280}, {0, 0, 545, 170}, {0, 0, 545, 60}},
        {{0, 0, 665, 280}, {0, 0, 665, 170}, {0, 0, 665, 60}}
    };

    Clock dilayEnemy;
    Clock textureTimer;

    const int quantityDice = 200;
    Dice* diceArr[quantityDice] = {nullptr};
    int coutPlayerWon = 0;
    int countEnemyWon = 0;
    int countDice = 1;

    bool needNewDice = true;

    Text wonPlayer(font, "You won! :)", 72);
    wonPlayer.setFillColor(Color::White);
    wonPlayer.setPosition({400, (hightWin / 2) - 200.0f});

    Text wonEnemy(font, "You lose! :(", 72);
    wonEnemy.setFillColor(Color::White);
    wonEnemy.setPosition({380, (hightWin / 2) - 200.0f});

    Text draw(font, "draw :|", 72);
    draw.setFillColor(Color::White);
    draw.setPosition({450, (hightWin / 2) - 200.0f});

    int countScorePlayer = 0;
    Text scorePlayer(font, std::to_string(countScorePlayer), 100);
    scorePlayer.setFillColor(Color::Black);
    FloatRect boundsPlayer = scorePlayer.getLocalBounds();
    scorePlayer.setOrigin(boundsPlayer.getCenter());
    scorePlayer.setPosition({210, 600});

    int countScoreEnemy = 0;
    Text scoreEnemy(font, std::to_string(countScoreEnemy), 100);
    scoreEnemy.setFillColor(Color::Black);
    FloatRect boundsEnemy = scoreEnemy.getLocalBounds();
    scoreEnemy.setOrigin(boundsEnemy.getCenter());
    scoreEnemy.setPosition({990, 410});

    bool moveRollDice = true;

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
                                dilayEnemy.restart();
                                flag = false;
                            } 
                       }
                    }
                }
            }
        }
        if (coutPlayerWon != 9 && countEnemyWon != 9)
        {
            coutPlayerWon = 0;
            countEnemyWon = 0;
            countScorePlayer = 0;
            countScoreEnemy = 0;
            for (int i = 0; i < 3; i++)
            {
                if (playerTable[i][0][0] != 0)
                {
                    countScorePlayer += setColor(i, playerTable, diceArr);
                }
                if (enemyTable[i][0][0] != 0)
                {
                    countScoreEnemy += setColor(i, enemyTable, diceArr);
                }
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
            scorePlayer.setString(std::to_string(countScorePlayer));
            boundsPlayer = scorePlayer.getLocalBounds();
            scorePlayer.setOrigin(boundsPlayer.getCenter());
            scoreEnemy.setString(std::to_string(countScoreEnemy));
            boundsEnemy = scoreEnemy.getLocalBounds();
            scoreEnemy.setOrigin(boundsEnemy.getCenter());

            if (needNewDice && dilayEnemy.getElapsedTime().asMilliseconds() > 500)
            {
                rollDiceF(turn, needNewDice, moveRollDice, 
                            countDice, randomSeed, randomSeedDice, diceArr, dilayEnemy,
                                rollDice, texture, textureTimer);
            }
                        
            if (!turn && !needNewDice && dilayEnemy.getElapsedTime().asMilliseconds() > 1000)
            {
                bool flag = true;
                for (int j = 0; j < 3 && flag; j++)
                {
                    for (int i = 0; i < 3 && flag; i++)
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
                            dilayEnemy.restart();
                        }
                    }
                }
            }

            setPosMap(playerTable, diceArr);
            setPosMap(enemyTable, diceArr);
        }

// рисунки
        window.clear(Color::White);

        window.draw(tableEnemy);
        window.draw(tablePlayer);

        if (needNewDice && dilayEnemy.getElapsedTime().asMilliseconds() > 500 && dilayEnemy.getElapsedTime().asMilliseconds() < 1500)
        {
            window.draw(rollDice);
        }
        
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
        window.draw(scorePlayer);
        window.draw(scoreEnemy);
        if ((coutPlayerWon == 9 || countEnemyWon == 9) || true)
        {
            window.draw(endDisplay);
            if (countScoreEnemy == countScorePlayer)
            {
                window.draw(draw);
            } else if (countScoreEnemy > countScorePlayer)
            {
                window.draw(wonEnemy);
            } else 
            {
                window.draw(wonPlayer);
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