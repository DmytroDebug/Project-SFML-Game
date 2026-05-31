#pragma once

#include "../Base/game_object.h"

class PlayerRobot : public GameObject
{
public:
    PlayerRobot(sf::Vector2f startPosition, float movementSpeed, sf::Vector2f arenaSize);

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;

private:
    bool move(float deltaTime);
    void animate(float deltaTime, bool moving);
    void setFrame(int frame);
    void keepInsideArena();

    sf::Texture texture;
    sf::Sprite sprite;

    float speed;
    sf::Vector2f arenaSize;

    int frameWidth;
    int frameHeight;
    int currentFrame;
    float animationTimer;
};