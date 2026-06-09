#include "game.h"
#include "../GameObject/Player/player_.h"
#include "../GameObject/Projectile/projectile.h"
#include "../GameObject/Enemy/enemy.h"
#include "../GameObject/Projectile/enemy_bullet.h"

#include <string>
#include <cmath>
#include <iostream>
#include <memory>
#include <cstdlib>
#include <ctime>

Game::Game() : window(sf::VideoMode({1280, 720}), "Robo Cleaner: Lab Escape"), gameState(GameState::MainMenu), enemySpawnTime(2.5f)
{
    window.setFramerateLimit(60);
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    if (!font.openFromFile("Attachments/type/menu.ttf"))
    {
        std::cout << "Failed to load font" << std::endl;
    }

    if (!menuBackgroundTexture.loadFromFile("Attachments/textures/menu_background.png"))
    {
        std::cout << "Failed to load menu background" << std::endl;
    }
    if (!labBackgroundTexture.loadFromFile("Attachments/textures/lab_background.png"))
    {
        std::cout << "Failed to load lab background" << std::endl;
    }

    playButtonPosition = {60.f, 377.f};
    playButtonSize = {350.f, 50.f};

    exitButtonPosition = {60.f, 465.f};
    exitButtonSize = {350.f, 50.f};

    enemyLines = {170.f, 260.f, 350.f, 440.f, 530.f};

    enemyShootTime = 1.4f;
    lives = 3;

    score = 0;
}

void Game::run()
{
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        handleEvents();
        update(deltaTime);
        render();
    }
}

void Game::handleEvents()
{
    while (auto event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }

        if (auto key = event->getIf<sf::Event::KeyPressed>())
        {
            if (gameState == GameState::MainMenu)
            {
                if (key->code == sf::Keyboard::Key::Enter)
                {
                    startGame();
                }

                if (key->code == sf::Keyboard::Key::Escape)
                {
                    window.close();
                }
            }

            if (gameState == GameState::Playing)
            {
                if (key->code == sf::Keyboard::Key::Escape)
                {
                    window.setMouseCursorVisible(true);
                    gameState = GameState::MainMenu;
                }
            }
        }

        if (auto mouse = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouse->button == sf::Mouse::Button::Left)
            {
                if (gameState == GameState::MainMenu)
                {
                    if (isMouseHere(playButtonPosition, playButtonSize))
                    {
                        startGame();
                    }
                    else if (isMouseHere(exitButtonPosition, exitButtonSize))
                    {
                        window.close();
                    }
                }
                else if (gameState == GameState::Playing)
                {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    sf::Vector2f targetPosition = window.mapPixelToCoords(mousePosition);

                    shoot(targetPosition);
                }
            }
        }
    }
}

void Game::update(float deltaTime)
{
    if (gameState != GameState::Playing)
    {
        return;
    }

    for (auto &object : objects)
    {
        object->update(deltaTime);
    }

    if (enemySpawnClock.getElapsedTime().asSeconds() >= enemySpawnTime)
    {
        spawnEnemy();
        enemySpawnClock.restart();
    }

    if (enemyShootClock.getElapsedTime().asSeconds() >= enemyShootTime)
    {
        enemyShoot();
        enemyShootClock.restart();
    }

    checkCollisions();
    checkPlayerHit();

    for (int i = static_cast<int>(objects.size()) - 1; i >= 0; i--)
    {
        if (!objects[i]->isAlive())
        {
            objects.erase(objects.begin() + i);
        }
    }
}

void Game::render()
{
    window.clear();

    if (gameState == GameState::MainMenu)
    {
        renderMainMenu();
    }

    if (gameState == GameState::Playing)
    {
        renderPlaying();
    }

    window.display();
}

void Game::renderMainMenu()
{
    drawBackground(menuBackgroundTexture);

    if (isMouseHere(playButtonPosition, playButtonSize))
    {
        sf::RectangleShape hover(playButtonSize);
        hover.setPosition(playButtonPosition);
        hover.setFillColor(sf::Color{255, 255, 255, 20});
        window.draw(hover);
    }

    if (isMouseHere(exitButtonPosition, exitButtonSize))
    {
        sf::RectangleShape hover(exitButtonSize);
        hover.setPosition(exitButtonPosition);
        hover.setFillColor(sf::Color{255, 255, 255, 20});
        window.draw(hover);
    }
}

void Game::renderPlaying()
{
    drawBackground(labBackgroundTexture);

    for (auto &object : objects)
    {
        object->draw(window);
    }

    drawUI();
    drawCursor();
}

void Game::drawBackground(const sf::Texture &texture)
{
    sf::Sprite background(texture);

    sf::Vector2u textureSize = texture.getSize();
    sf::Vector2u windowSize = window.getSize();

    float scaleX = static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x);
    float scaleY = static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y);

    background.setScale({scaleX, scaleY});
    window.draw(background);
}

void Game::startGame()
{
    objects.clear();
    score = 0;
    lives = 3;

    objects.push_back(std::make_unique<PlayerRobot>(sf::Vector2f{310.f, 360.f}, 230.f, sf::Vector2f{1280.f, 720.f}));

    enemySpawnClock.restart();
    enemyShootClock.restart();

    window.setMouseCursorVisible(false);

    gameState = GameState::Playing;
}

bool Game::isMouseHere(sf::Vector2f position, sf::Vector2f size) const
{
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

    return mousePosition.x >= position.x &&
           mousePosition.x <= position.x + size.x &&
           mousePosition.y >= position.y &&
           mousePosition.y <= position.y + size.y;
}
void Game::shoot(sf::Vector2f targetPosition)
{
    if (objects.empty())
    {
        return;
    }

    sf::Vector2f playerPosition = objects[0]->getPosition();

    sf::Vector2f direction{
        targetPosition.x - playerPosition.x,
        targetPosition.y - playerPosition.y};

    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length == 0.f)
    {
        return;
    }

    direction.x /= length;
    direction.y /= length;

    sf::Vector2f gunPosition{
        playerPosition.x + direction.x * 45.f,
        playerPosition.y + direction.y * 45.f};

    objects.push_back(std::make_unique<Projectile>(gunPosition, targetPosition));
}
void Game::drawCursor()
{
    sf::Vector2i mousePixelPosition = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosition = window.mapPixelToCoords(mousePixelPosition);

    sf::CircleShape cursor(10.f);
    cursor.setOrigin({10.f, 10.f});
    cursor.setPosition(mousePosition);

    cursor.setFillColor(sf::Color::Transparent);
    cursor.setOutlineThickness(2.f);
    cursor.setOutlineColor(sf::Color{20, 60, 170});

    window.draw(cursor);

    sf::CircleShape dot(2.f);
    dot.setOrigin({2.f, 2.f});
    dot.setPosition(mousePosition);
    dot.setFillColor(sf::Color{80, 180, 255});

    window.draw(dot);
}
void Game::spawnEnemy()
{
    int lineIndex = std::rand() % enemyLines.size();
    float enemyY = enemyLines[lineIndex];

    objects.push_back(std::make_unique<Enemy>(sf::Vector2f{1330.f, enemyY}, 70.f));
}
void Game::checkCollisions()
{
    for (auto &firstObject : objects)
    {
        Projectile *projectile = dynamic_cast<Projectile *>(firstObject.get());

        if (projectile == nullptr || !projectile->isAlive())
        {
            continue;
        }

        for (auto &secondObject : objects)
        {
            Enemy *enemy = dynamic_cast<Enemy *>(secondObject.get());

            if (enemy == nullptr || !enemy->isAlive())
            {
                continue;
            }

            if (projectile->getBounds().findIntersection(enemy->getBounds()))
            {
                enemy->takeDamage(projectile->getDamage());
                projectile->destroy();

                if (!enemy->isAlive())
                {
                    score += enemy->getScoreValue();
                }

                break;
            }
        }
    }
}
void Game::drawUI()
{
    sf::Text scoreText(font, "Score: " + std::to_string(score), 28);

    scoreText.setPosition({25.f, 20.f});
    scoreText.setFillColor(sf::Color{20, 60, 170});
    scoreText.setOutlineThickness(2.f);
    scoreText.setOutlineColor(sf::Color::White);

    window.draw(scoreText);

    sf::Text livesText(font, "Lives: " + std::to_string(lives), 28);

    livesText.setPosition({25.f, 55.f});
    livesText.setFillColor(sf::Color{20, 60, 170});
    livesText.setOutlineThickness(2.f);
    livesText.setOutlineColor(sf::Color::White);

    window.draw(livesText);
}
void Game::enemyShoot()
{
    for (auto &object : objects)
    {
        Enemy *enemy = dynamic_cast<Enemy *>(object.get());

        if (enemy != nullptr && enemy->isAlive())
        {
            sf::Vector2f enemyPosition = enemy->getPosition();

            objects.push_back(std::make_unique<EnemyBullet>(sf::Vector2f{enemyPosition.x - 35.f, enemyPosition.y}));

            return;
        }
    }
}
void Game::checkPlayerHit()
{
    PlayerRobot *player = nullptr;

    for (auto &object : objects)
    {
        player = dynamic_cast<PlayerRobot *>(object.get());

        if (player != nullptr)
        {
            break;
        }
    }

    if (player == nullptr)
    {
        return;
    }

    for (auto &object : objects)
    {
        EnemyBullet *bullet = dynamic_cast<EnemyBullet *>(object.get());

        if (bullet == nullptr || !bullet->isAlive())
        {
            continue;
        }

        if (bullet->getBounds().findIntersection(player->getBounds()))
        {
            bullet->destroy();
            lives--;

            if (lives <= 0)
            {
                window.setMouseCursorVisible(true);
                gameState = GameState::MainMenu;
            }
        }
    }
}