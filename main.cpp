#include <iostream>
#include <conio.h>
#include <windows.h>

// Constants
const int width = 40;
const int height = 20;

// Variables
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
bool gameOver;
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction dir;

// Handle to the console
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void Setup() {
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
}

void Draw() {
    COORD coord;
    coord.X = 0;
    coord.Y = 0;
    SetConsoleCursorPosition(hConsole, coord);

    for (int i = 0; i < width + 2; ++i) std::cout << "#";
    std::cout << std::endl;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (j == 0) std::cout << "#"; // Left border
            if (i == y && j == x)
                std::cout << "O"; // Snake head
            else if (i == fruitY && j == fruitX)
                std::cout << "X"; // Fruit
            else {
                bool print = false;
                for (int k = 0; k < nTail; ++k) {
                    if (tailX[k] == j && tailY[k] == i) {
                        std::cout << "o";
                        print = true;
                    }
                }
                if (!print) std::cout << " ";
            }
            if (j == width - 1) std::cout << "#"; // Right border
        }
        std::cout << std::endl;
    }

    for (int i = 0; i < width + 2; ++i) std::cout << "#";
    std::cout << std::endl;
    std::cout << "Score: " << score << std::endl;
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a':
            dir = LEFT;
            break;
        case 'd':
            dir = RIGHT;
            break;
        case 'w':
            dir = UP;
            break;
        case 's':
            dir = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
        }
    }
}

void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; ++i) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }

    // Check for collision with walls
    if (x >= width || x < 0 || y >= height || y < 0) {
        gameOver = true;
    }

    // Check for collision with itself
    for (int i = 0; i < nTail; ++i) {
        if (tailX[i] == x && tailY[i] == y) {
            gameOver = true;
        }
    }

    // Check for fruit
    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }
}

int main() {
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(100); // Sleep for 100 milliseconds
    }
    std::cout << "Game Over!" << std::endl;
    return 0;
}
