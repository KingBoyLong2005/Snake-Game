#ifndef GAME_HPP
#define GAME_HPP

#include "Snake.hpp"
#include "Apple.hpp"

class Game {
public:
    Game(int screenWidth, int screenHeight);
    ~Game();

    bool Init();
    void handleEvents();
    bool update();
    void render();
    void clear();
    void pauseGame(); // Tạm dừng game
    void handleGameOver(); // Xử lý game over
    void resetGame();

    bool isRunning;
    int screenWidth, screenHeight;
private:
    Snake* player;
    Apple* apple;
    int Score;
    bool isPaused; // Trạng thái tạm dừng

};

#endif
