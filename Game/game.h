#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "../GameObject/Base/game_object.h"

class Game
{
public:
    Game();
    void run();

private:
    enum class GameState
    {
        MainMenu,
        Playing,
        GameOver
    };

    void handleEvents();
    void update(float deltaTime);
    void render();

    void renderMainMenu();
    void renderPlaying();
    void renderGameOver();
    void drawBackground(const sf::Texture &texture);

    void startGame();
    bool isMouseHere(sf::Vector2f position, sf::Vector2f size) const;
    void drawCursor();

    void shoot(sf::Vector2f targetPosition);

    sf::RenderWindow window;
    sf::Clock clock;

    void spawnEnemy();
    std::vector<float> enemyLines;
    sf::Clock enemySpawnClock;
    float enemySpawnTime;
    void checkCollisions();
    int score;
    void drawUI();
    sf::Clock enemyShootClock;
    float enemyShootTime;
    int lives;
    void enemyShoot();
    void checkPlayerHit();

    sf::Texture menuBackgroundTexture;
    sf::Texture labBackgroundTexture;

    sf::Vector2f playButtonPosition;
    sf::Vector2f playButtonSize;

    sf::Vector2f exitButtonPosition;
    sf::Vector2f exitButtonSize;

    GameState gameState;

    std::vector<std::unique_ptr<GameObject>> objects;
    sf::Font font;
};