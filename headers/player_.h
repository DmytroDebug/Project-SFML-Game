#pragma once

#include "game_object.h"

class PlayerRobot : public GameObject
{
public:
    PlayerRobot(sf::Vector2f startPosition, float movementSpeed, sf::Vector2f arenaSize);
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;

private:
    void handleMovement(float deltaTime);
    void keepInsideArena();

    sf::CircleShape body;
    float speed;
    sf::Vector2f arenaSize;
};