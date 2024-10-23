#include "Game.hpp"
#include <SDL.h>

int main(int argc, char* argv[]) {
    const int screenWidth = 800;
    const int screenHeight = 1080;

    // Set độ dài và rộng cho màn hình
    Game game(screenWidth, screenHeight);

    if (!game.Init()) {
        return -1;
    }

    while (game.isRunning) {
        game.handleEvents();
        if (!game.update()) { // Kiểm tra nếu update() trả về false
                break; // Thoát vòng lặp nếu game kết thúc
            }
        game.render();
        SDL_Delay(100); // Tốc độ
    }

    game.clear();
    SDL_Quit();

    return 0;
}
