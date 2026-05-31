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
        Playing
    };

    void handleEvents();
    void update(float deltaTime);
    void render();

    void renderMainMenu();
    void renderPlaying();
    void drawBackground(const sf::Texture& texture);

    void startGame();
    bool isMouseHere(sf::Vector2f position, sf::Vector2f size) const;

    sf::RenderWindow window;
    sf::Clock clock;

    sf::Texture menuBackgroundTexture;
    sf::Texture labBackgroundTexture;

    sf::Vector2f playButtonPosition;
    sf::Vector2f playButtonSize;

    sf::Vector2f exitButtonPosition;
    sf::Vector2f exitButtonSize;

    GameState gameState;

    std::vector<std::unique_ptr<GameObject>> objects;
};