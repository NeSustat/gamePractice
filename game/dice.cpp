#include "dice.h"
#include <random>

Dice::Dice(int seed) : value(1){
    Dice::roll(seed);
    shape.setSize(sf::Vector2f(100, 100));
    shape.setFillColor(sf::Color::White);

    std::string filename = "pic/dice" + std::to_string(value) + ".png";
    if (texture.loadFromFile(filename)) {
        shape.setTexture(&texture);
    }
    
}
void Dice::roll(int seed){
    std::mt19937 gen(seed);
    value = std::uniform_int_distribution<int>(1, 6)(gen);
}

int Dice::getValue() const{
    return value;
}  

void Dice::setPosition(float x, float y){
    shape.setPosition({x, y});
}

void Dice::draw(sf::RenderWindow &window) const{
    window.draw(shape);
}

void Dice::setColor(int i){
    switch (i)
    {
    case 2:
        shape.setFillColor(sf::Color::Yellow);
        break;
    case 3:
        shape.setFillColor(sf::Color::Blue);
        break;
    default:
        shape.setFillColor(sf::Color::White);
        break;
    }
}