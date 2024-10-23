// Snake.hpp
#ifndef SNAKE_HPP
#define SNAKE_HPP

#include "Point.hpp"
#include <deque>

class Snake {
public:
    int bodyLength;
    int bodyWidth;
    bool hasEaten;
    std::deque<Point> body;
    enum Direction { UP, DOWN, LEFT, RIGHT };
    Direction currentDirection;

    Snake();
    ~Snake();
    
    void move(); // Di chuyển tự động
    void changeDirection(Direction newDirection); // Chuyển hướng
    Point* Spawn();
    void reset();
    bool checkCollisionWithApple(const Point& applePosition);
    bool checkCollisionWithWall(int screenWidth, int screenHeight);
    bool checkCollisionWithSelf();
};

#endif // SNAKE_HPP
