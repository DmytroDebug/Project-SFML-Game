#include "player_.h"

#include <cmath>

PlayerRobot::PlayerRobot(sf::Vector2f startPosition, float movementSpeed, sf::Vector2f arenaSize)
    : GameObject(startPosition), speed(movementSpeed), arenaSize(arenaSize)
{
    body.setRadius(35.f);
    body.setOrigin({35.f, 35.f});
    body.setPosition(position);

    body.setFillColor(sf::Color{220, 230, 235});
    body.setOutlineThickness(4.f);
    body.setOutlineColor(sf::Color{80, 220, 255});
}

void PlayerRobot::update(float deltaTime)
{
    handleMovement(deltaTime);
    keepInsideArena();

    body.setPosition(position);
}

void PlayerRobot::draw(sf::RenderWindow& window)
{
    window.draw(body);
}

sf::FloatRect PlayerRobot::getBounds() const
{
    return body.getGlobalBounds();
}

void PlayerRobot::handleMovement(float deltaTime)
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

    if (direction.x != 0.f || direction.y != 0.f)
    {
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        direction.x /= length;
        direction.y /= length;

        position.x += direction.x * speed * deltaTime;
        position.y += direction.y * speed * deltaTime;
    }
}

void PlayerRobot::keepInsideArena()
{
    float radius = body.getRadius();

    if (position.x < radius)
    {
        position.x = radius;
    }

    if (position.x > arenaSize.x - radius)
    {
        position.x = arenaSize.x - radius;
    }

    if (position.y < radius)
    {
        position.y = radius;
    }

    if (position.y > arenaSize.y - radius)
    {
        position.y = arenaSize.y - radius;
    }
}