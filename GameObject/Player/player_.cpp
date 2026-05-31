#include "player_.h"

#include <iostream>
#include <cmath>

PlayerRobot::PlayerRobot(sf::Vector2f startPosition, float movementSpeed, sf::Vector2f arenaSize)
    : GameObject(startPosition),
      sprite(texture),
      speed(movementSpeed),
      arenaSize(arenaSize),
      frameWidth(0),
      frameHeight(0),
      currentFrame(0),
      animationTimer(0.f)
{
    if (!texture.loadFromFile("Attachments/textures/player.png"))
    {
        std::cout << "Failed to load player texture" << std::endl;
    }

    frameWidth = static_cast<int>(texture.getSize().x) / 4;
    frameHeight = static_cast<int>(texture.getSize().y);

    sprite.setTexture(texture);
    setFrame(0);

    sprite.setOrigin({
        static_cast<float>(frameWidth) / 2.f,
        static_cast<float>(frameHeight) / 2.f
    });

    sprite.setScale({0.25f, 0.25f});
    sprite.setPosition(position);
}

void PlayerRobot::update(float deltaTime)
{
    bool moving = move(deltaTime);

    keepInsideArena();
    animate(deltaTime, moving);

    sprite.setPosition(position);
}

void PlayerRobot::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

sf::FloatRect PlayerRobot::getBounds() const
{
    return sprite.getGlobalBounds();
}

bool PlayerRobot::move(float deltaTime)
{
    sf::Vector2f direction{0.f, 0.f};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
    {
        direction.y -= 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
    {
        direction.y += 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        direction.x -= 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        direction.x += 1.f;
    }
    if (direction.x == 0.f && direction.y == 0.f)
    {
        return false;
    }
    if (direction.x > 0.f)
    {
        sprite.setScale({0.25f, 0.25f});
    }
    else if (direction.x < 0.f)
    {
        sprite.setScale({-0.25f, 0.25f});
    }

    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    direction.x /= length;
    direction.y /= length;

    position.x += direction.x * speed * deltaTime;
    position.y += direction.y * speed * deltaTime;

    return true;
}

void PlayerRobot::animate(float deltaTime, bool moving)
{
    if (!moving)
    {
        currentFrame = 0;
        animationTimer = 0.f;
        setFrame(currentFrame);
        return;
    }

    animationTimer += deltaTime;

    if (animationTimer >= 0.20f)
    {
        animationTimer = 0.f;
        currentFrame++;

        if (currentFrame >= 2)
        {
            currentFrame = 0;
        }

        setFrame(currentFrame);
    }
}

void PlayerRobot::setFrame(int frame)
{
    sprite.setTextureRect(
        sf::IntRect(
            {frame * frameWidth, 0},
            {frameWidth, frameHeight}
        )
    );
}

void PlayerRobot::keepInsideArena()
{
    sf::FloatRect bounds = sprite.getGlobalBounds();

    float halfWidth = bounds.size.x / 2.f;
    float halfHeight = bounds.size.y / 2.f;

    if (position.x < halfWidth)
    {
        position.x = halfWidth;
    }

    if (position.x > arenaSize.x - halfWidth)
    {
        position.x = arenaSize.x - halfWidth;
    }

    if (position.y < halfHeight)
    {
        position.y = halfHeight;
    }

    if (position.y > arenaSize.y - halfHeight)
    {
        position.y = arenaSize.y - halfHeight;
    }
}