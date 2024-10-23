#include "Game.hpp"
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>

static SDL_Window* window = nullptr; // Biến toàn cục cho cửa sổ
static SDL_Renderer* renderer = nullptr; // Biến toàn cục cho renderer

Game::Game(int width, int height) : screenHeight(width), screenWidth(height), Score(0), isRunning(true)
{
    player = new Snake();
    apple = new Apple();
    apple->spawnRandom(screenWidth, screenHeight);
}

Game::~Game()
{
    delete player;
    delete apple;
}

bool Game::Init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    if (TTF_Init() == -1)
    {
        std::cout << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }

    return true;
}
void Game::clear()
{
    //Giải phóng tài nguyên khi thoát
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit(); // Giải phóng SDL_ttf trước khi tắt SDL
    SDL_Quit();
}
void Game::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            isRunning = false;
        }
        if (event.type == SDL_KEYDOWN)
        {   
            switch (event.key.keysym.sym)
            {
                case SDLK_UP:
                player->changeDirection(Snake::UP);
                break;
                case SDLK_DOWN:
                player->changeDirection(Snake::DOWN);
                break;
                case SDLK_RIGHT:
                player->changeDirection(Snake::RIGHT);
                break;
                case SDLK_LEFT:
                player->changeDirection(Snake::LEFT);
                break;
            }
        }
    }
}

bool Game::update()
{
    // Di chuyển tự động
    player->move();

    // Kiểm tra nếu va chạm với táo
    if (player->checkCollisionWithApple(Point(apple->X, apple->Y)))
    {
        Score++;
        player->hasEaten = true; // Rắn đã ăn
        apple->spawnRandom(screenWidth, screenHeight); // Tạo táo mới
    }

    // Kiểm tra nếu va chạm với chính hoặc tường
    if (player->checkCollisionWithSelf() || player->checkCollisionWithWall(screenWidth, screenHeight))
    {
        handleGameOver(); // Trả về false để biểu thị game đã kết thúc
    }
    
    return true; // Trả về true nếu cập nhật thành công
}

void Game::render()
{
    // Set màu nền (đen)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Set màu cho táo (đỏ)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect appleRect = { apple->X, apple->Y, apple->Size, apple->Size };
    SDL_RenderFillRect(renderer, &appleRect);

    // Tạo font và màu chữ cục bộ (local)
    TTF_Font* font = TTF_OpenFont("assets/fonts/PIXEARG_.ttf", 24);
    if (!font) {
        std::cout << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        return;
    }
    
    SDL_Color textColor = {255, 255, 255}; // Màu trắng

    // Chuyển điểm số thành chuỗi
    std::string scoreText = "Score: " + std::to_string(Score);

    // Tạo bề mặt chữ và texture từ điểm số
    // Tạo ra một bề mặt (surface) chứa đoạn văn bản scoreText với fontđã được nạp trước đó, và sử dụng màu chữ textColor.
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    // Kiểm tra lỗi khi tạo
    if (!textSurface) {
        std::cout << "Unable to render text surface! TTF_Error: " << TTF_GetError() << std::endl;
        TTF_CloseFont(font);
        return;
    }
    
    // Dòng này tạo một texture từ textSurface. Texture này sẽ được sử dụng để hiển thị điểm số lên màn hình.
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        std::cout << "Unable to create texture from rendered text! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(textSurface);
        TTF_CloseFont(font);
        return;
    }

    // Tạo rect cho chữ
    SDL_Rect textRect = {10, 10, textSurface->w, textSurface->h};

    // Vẽ chữ lên màn hình
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

    // Giải phóng tài nguyên
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);

    // Set màu cho rắn (xanh lá cây)
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (auto& segment : player->body) {
        SDL_Rect snakeRect = { segment.X, segment.Y, player->bodyWidth, player->bodyWidth };
        SDL_RenderFillRect(renderer, &snakeRect);
    }

    // Vẽ lên màn hình
    SDL_RenderPresent(renderer);
}

void Game::pauseGame() 
{
    isPaused = true;
    while (isPaused) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_b: // Back to game
                        isPaused = false;
                        break;
                    case SDLK_r: // Reset game
                        resetGame();
                        Score = 0;
                        isPaused = false;
                        break;
                    case SDLK_e: // Exit game
                        isRunning = false;
                        isPaused = false;
                        break;
                }
            }
        }

        // Hiển thị menu tạm dừng
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Màu nền đen
        SDL_RenderClear(renderer);

        // Set màu cho menu (trắng)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        // Đặt ví trí chữ
        SDL_Rect pauseMenu = { screenWidth / 4, screenHeight / 4, screenWidth / 2, screenHeight / 2 };
        SDL_RenderFillRect(renderer, &pauseMenu);

        // Hiển thị lên màn hình
        SDL_RenderPresent(renderer);
    }
}

void Game::handleGameOver() 
{
    // Tạo font và màu chữ cục bộ cho thông báo game over
    TTF_Font* font = TTF_OpenFont("assets/fonts/PIXEARG_.ttf", 24);
    if (!font) {
        std::cout << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Color textColor = {255, 255, 255}; // Màu trắng cho chữ
    std::string gameOverText = "Game Over! Press Y to play again or N to quit.";
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, gameOverText.c_str(), textColor);
    if (!textSurface) {
        std::cout << "Unable to render text surface! TTF_Error: " << TTF_GetError() << std::endl;
        TTF_CloseFont(font);
        return;
    }
    
    // Đặt vị trí của dòng chữ lên màn hình
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = {screenWidth / 4, screenHeight / 2, textSurface->w, textSurface->h};
    SDL_FreeSurface(textSurface); // Tạo xong sẽ là giải phóng

    while (true) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_y: // Chơi lại
                        resetGame();
                        Score = 0;
                        return;
                    case SDLK_n: // Thoát game
                        isRunning = false;
                        return;
                }
            }
        }

        // Hiển thị màn hình game over
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
        SDL_RenderPresent(renderer);
    }
}

void Game::resetGame() 
{
    // Đặt lại điểm số
    Score = 0;

    // Reset rắn
    player->reset();

    // Spawn lại táo
    apple->spawnRandom(screenWidth, screenHeight);

    // Đảm bảo game tiếp tục chạy
    isRunning = true;
}
