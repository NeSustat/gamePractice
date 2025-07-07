#include "dice.h"
#include <random>

Dice::Dice(int seed) : value(1){
    Dice::roll(seed);
    shape.setSize(sf::Vector2f(48, 48));
    switch (value) 
    {
        case 1: 
            shape.setFillColor(sf::Color::White);
            break;
        case 2: 
            shape.setFillColor(sf::Color::Red);
            break;
        case 3: 
            shape.setFillColor(sf::Color::Green);
            break;
        case 4: 
            shape.setFillColor(sf::Color::Yellow);
            break;
        case 5: 
            shape.setFillColor(sf::Color::Blue);
            break;
        case 6: 
            shape.setFillColor(sf::Color::Magenta);
            break;
        default:
            shape.setFillColor(sf::Color::White);
            break;
    }
    // shape.setFillColor(sf::Color::White);
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::Red);
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