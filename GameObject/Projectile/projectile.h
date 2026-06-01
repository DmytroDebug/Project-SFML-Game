#pragma once

#include "../Base/game_object.h"

class Projectile : public GameObject
{
public:
    Projectile(sf::Vector2f startPosition, sf::Vector2f targetPosition);

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;
    int getDamage() const;

private:
    sf::CircleShape shape;
    sf::Vector2f direction;
    float speed;
    int damage;
};