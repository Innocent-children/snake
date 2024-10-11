#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
using namespace std;

bool gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
vector<int> tailX, tailY;
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;
int difficulty;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

void gotoxy(int x, int y) {
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

void Setup() {
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 0;
    tailX.clear();
    tailY.clear();
}

void Draw() {
    vector<string> buffer(height, string(width, ' '));

    buffer[y][x] = 'O';
    if (fruitX >= 0 && fruitX < width && fruitY >= 0 && fruitY < height) {
        buffer[fruitY][fruitX] = 'F';
    }

    for (int k = 0; k < nTail; k++) {
        if (tailX[k] >= 0 && tailX[k] < width && tailY[k] >= 0 && tailY[k] < height) {
            buffer[tailY[k]][tailX[k]] = 'o';
        }
    }

    for (int i = 0; i < height; i++) {
        gotoxy(0, i);
        cout << string(width + 2, '#');
        gotoxy(1, i);
        cout << buffer[i];
        gotoxy(width + 1, i);
        cout << '#';
    }

    gotoxy(0, height);
    cout << string(width + 2, '#');

    gotoxy(0, height + 1);
    cout << "Score: " << score << "    ";
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a':
            if (dir != RIGHT) dir = LEFT;
            break;
        case 'd':
            if (dir != LEFT) dir = RIGHT;
            break;
        case 'w':
            if (dir != DOWN) dir = UP;
            break;
        case 's':
            if (dir != UP) dir = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
        }
    }
}

void Logic() {
    int prevX = tailX.empty() ? x : tailX[0];
    int prevY = tailY.empty() ? y : tailY[0];
    int prev2X, prev2Y;
    if (!tailX.empty()) tailX[0] = x;
    if (!tailY.empty()) tailY[0] = y;
    for (int i = 1; i < nTail; i++) {
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

    // 检查是否撞墙
    if (x >= width || x < 0 || y >= height || y < 0)
        gameOver = true;

    // 检查是否撞到自己
    for (int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;

    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
        tailX.push_back(0);
        tailY.push_back(0);
    }
}

int main() {
    cout << "Choose difficulty (1-Easy, 2-Medium, 3-Hard): ";
    cin >> difficulty;
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(difficulty == 1 ? 200 : (difficulty == 2 ? 100 : 50));
    }
    gotoxy(width / 2 - 4, height / 2);
    cout << "Game Over!";
    gotoxy(width / 2 - 8, height / 2 + 1);
    cout << "Your Score: " << score;
    gotoxy(0, height + 2);
    return 0;
}