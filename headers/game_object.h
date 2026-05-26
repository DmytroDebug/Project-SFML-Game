#pragma once

#include <SFML/Graphics.hpp>

class GameObject
{
public:
    GameObject(sf::Vector2f startPosition);
    virtual ~GameObject();
    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow &window) = 0;
    virtual sf::FloatRect getBounds() const = 0;
    bool isAlive() const;
    void destroy();

protected:
    sf::Vector2f position;
    bool alive;
};