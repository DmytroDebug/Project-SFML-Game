#include "enemy.h"

Enemy::Enemy(sf::Vector2f startPosition, float movementSpeed)
    : GameObject(startPosition),
      speed(movementSpeed),
      health(3),
      scoreValue(10)
{
    body.setSize({55.f, 55.f});
    body.setOrigin({27.5f, 27.5f});
    body.setPosition(position);

    body.setFillColor(sf::Color{120, 30, 40});
    body.setOutlineThickness(3.f);
    body.setOutlineColor(sf::Color{220, 80, 90});
}

void Enemy::update(float deltaTime)
{
    position.x -= speed * deltaTime;

    body.setPosition(position);

    if (position.x < -80.f)
    {
        destroy();
    }
}

void Enemy::draw(sf::RenderWindow& window)
{
    window.draw(body);
}

sf::FloatRect Enemy::getBounds() const
{
    return body.getGlobalBounds();
}
void Enemy::takeDamage(int damage)
{
    health -= damage;

    if (health <= 0)
    {
        destroy();
    }
}

int Enemy::getScoreValue() const
{
    return scoreValue;
}