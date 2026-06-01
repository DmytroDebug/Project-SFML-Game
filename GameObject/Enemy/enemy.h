#pragma once

#include "../Base/game_object.h"

class Enemy : public GameObject
{
public:
    Enemy(sf::Vector2f startPosition, float movementSpeed);

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;

private:
    sf::RectangleShape body;
    float speed;
};