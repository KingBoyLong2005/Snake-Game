#ifndef APPLE_HPP
#define APPLE_HPP

#include "Point.hpp"

class Apple : public Point {
public:
    int Size;

    Apple();
    void spawnRandom(int windowWidth, int windowHeight);
};

#endif