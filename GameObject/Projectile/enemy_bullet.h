#pragma once

#include "../Base/game_object.h"

class EnemyBullet : public GameObject
{
public:
    EnemyBullet(sf::Vector2f startPosition);

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;

private:
    sf::CircleShape shape;
    float speed;
};