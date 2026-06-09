#include "enemy_bullet.h"

EnemyBullet::EnemyBullet(sf::Vector2f startPosition)
    : GameObject(startPosition),
      speed(420.f)
{
    shape.setRadius(8.f);
    shape.setOrigin({8.f, 8.f});
    shape.setPosition(position);

    shape.setFillColor(sf::Color{180, 30, 70});
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color{255, 120, 150});
}

void EnemyBullet::update(float deltaTime)
{
    position.x -= speed * deltaTime;
    shape.setPosition(position);

    if (position.x < -30.f)
    {
        destroy();
    }
}

void EnemyBullet::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

sf::FloatRect EnemyBullet::getBounds() const
{
    return shape.getGlobalBounds();
}