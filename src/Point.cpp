#include "Point.hpp"

Point::Point() : X(0), Y(0) {}

Point::Point(int x, int y) : X(x), Y(y) {}

void Point::setPosition(int x, int y) {
    X = x;
    Y = y;
}
