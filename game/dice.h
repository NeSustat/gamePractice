#pragma once
#include <SFML/Graphics.hpp>

class Dice {
private:
    sf::RectangleShape shape;
    sf::Texture texture;
    int value;
public:
    Dice(int seed);
    void roll(int seed);
    int getValue() const;
    void setPosition(float x, float y);
    void draw(sf::RenderWindow& window) const;
    void moveDice(float x, float y, int time);
};