#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "game_object.h"

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
    void eventsHandeling();
    void update(float deltaTime);
    void render();
    void renderMainMenu();
    void renderPlaying();
    void startGame();
    bool isMouseHere(sf::Vector2f position, sf::Vector2f size) const;

    sf::RenderWindow window;
    sf::Clock clock;
    sf::Font font;
    sf::Texture menuBackgroundTexture;
    sf::Vector2f playButtonPosition;
    sf::Vector2f playButtonSize;
    sf::Vector2f exitButtonPosition;
    sf::Vector2f exitButtonSize;

    GameState gameState;

    std::vector<std::unique_ptr<GameObject>> objects;
};