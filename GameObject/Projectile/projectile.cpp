#include "projectile.h"

#include <cmath>

Projectile::Projectile(sf::Vector2f startPosition, sf::Vector2f targetPosition)
    : GameObject(startPosition),
      direction{0.f, 0.f},
      speed(700.f)
{
    sf::Vector2f difference{
        targetPosition.x - startPosition.x,
        targetPosition.y - startPosition.y};

    float length = std::sqrt(difference.x * difference.x + difference.y * difference.y);

    if (length != 0.f)
    {
        direction.x = difference.x / length;
        direction.y = difference.y / length;
    }

    shape.setRadius(7.f);
    shape.setOrigin({7.f, 7.f});
    shape.setPosition(position);
    shape.setFillColor(sf::Color{20, 60, 170});
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color{80, 180, 255});
}

void Projectile::update(float deltaTime)
{
    position.x += direction.x * speed * deltaTime;
    position.y += direction.y * speed * deltaTime;

    shape.setPosition(position);

    if (position.x < -20.f || position.x > 1300.f ||
        position.y < -20.f || position.y > 740.f)
    {
        destroy();
    }
}

void Projectile::draw(sf::RenderWindow &window)
{
    window.draw(shape);
}

sf::FloatRect Projectile::getBounds() const
{
    return shape.getGlobalBounds();
}