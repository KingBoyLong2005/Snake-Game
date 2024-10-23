#include "Snake.hpp"

Snake::Snake() : bodyLength(2), bodyWidth(10), hasEaten(false), currentDirection(RIGHT)
{
    for (int i = 0; i < bodyLength; ++i)
    {
        body.push_back(Point(100 - i * bodyWidth, 100));
    }
}
Snake::~Snake() = default;

Point* Snake::Spawn()
{
    // Truy cập đến đầu của rắn
    return &body.front(); 
    // Lấy địa chỉ của phần tử đầu tiên trong deque
    // Nghĩa là trả về một con trỏ trỏ đến đầu rắn (phần tử đầu tiên của deque body).
}

void Snake::move()
{
    // Mảng chứa thay đổi X, Y theo các hướng UP, DOWN, LEFT, RIGHT
    int deltaX[] = {0, 0, -bodyWidth, bodyWidth};
    int deltaY[] = {-bodyWidth, bodyWidth, 0, 0};

    Point newHead = body.front();
    
    // Di chuyển theo hướng hiện tại
    switch (currentDirection)
    {
    case UP:
        newHead.Y -=bodyWidth;
        break;
    case DOWN:
        newHead.Y +=bodyWidth;
        break;
    case RIGHT:
        newHead.X +=bodyWidth;
        break;   
    case LEFT:
        newHead.X -=bodyWidth;
        break;    
    }

    // Thêm đầu mới vào
    body.push_front(newHead);

    // Nếu không ăn táo thì xóa đuôi
    if (!hasEaten)
    {
        body.pop_back();
    }
    else
    {
        hasEaten = false;
    }
}

void Snake::changeDirection(Direction newDirection)
{
    // Kiểm tra để không cho phép rắn quay đầu ngược lại
    if (currentDirection == UP && newDirection != DOWN)
    {
        currentDirection = newDirection;
    }
    else if (currentDirection == DOWN && newDirection != UP)
    {
        currentDirection = newDirection;
    }
    else if (currentDirection == LEFT && newDirection != RIGHT)
    {
        currentDirection = newDirection;
    }
    else if (currentDirection == RIGHT && newDirection != LEFT)
    {
        currentDirection = newDirection;
    }
}

// Kiểm tra đầu rắn có chạm vào quả táo không
bool Snake ::checkCollisionWithApple(const Point& applePosition)
{
    Point* head = Spawn(); // Lấy địa chỉ đầu rắn
    return (head->X == applePosition.X && head->Y == applePosition.Y);
    // Kiểm tra bằng cách đầu rắn và quả táo có cùng vị trí không
}

// Kiểm tra đầu rắn có chạm vào tường không
bool Snake::checkCollisionWithWall(int screenWidth, int screenHeight)
{
    Point* head = Spawn(); // Lấy địa chỉ đầu rắn
    return (head->X < 0 || head->X >= screenWidth || head->Y < 0 || head->Y >= screenHeight);
    // Kiểm tra bằng cách vị trí đầu rắn có bị nằm ngoài phạm vị của toàn màn hình không
}

// Kiểm tra đầu rắn có chạm vào thân không
bool Snake::checkCollisionWithSelf()
{
    Point* head = Spawn();
    // Kiểm tra đầu rắn có và bất kì phần thân nào có bị trùng vị trí không
    for (size_t i = 1; i < body.size(); ++i)
    {
        if (head->X == body[i].X && head->Y == body[i].Y)
        {
            return true;
        }
    }
    return false;
}

void Snake::reset()
{
    // Đặt lại rắn về trạng thái ban đầu
    body.clear(); // Xóa toàn bộ thân rắn hiện tại
    body.push_back(Point(100, 100)); // Thêm phần tử mới cho rắn ban đầu
    body.push_back(Point(80, 100));
    body.push_back(Point(60, 100));

    currentDirection = RIGHT;
    hasEaten = false;
}
