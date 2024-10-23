#include "Apple.hpp"
#include <cstdlib>
#include <random>

Apple::Apple() : Size(10) {}

void Apple::spawnRandom(int windowWidth, int windowHeight) {
    static std::random_device rd;
    static std::mt19937 gen(rd()); // Khởi tạo bộ tạo số ngẫu nhiên
    std::uniform_int_distribution<> distX(0, windowWidth / Size - 1);
    std::uniform_int_distribution<> distY(0, windowHeight / Size - 1);

    X = distX(gen) * Size;
    Y = distY(gen) * Size;
}