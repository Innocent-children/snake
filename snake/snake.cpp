#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <string>

using namespace std;

bool gameOver;
int width, height;
int x, y, fruitX, fruitY, score;
vector<int> tailX, tailY;
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

int difficulty = 1; // 1 = Easy, 2 = Normal, 3 = Hard

HANDLE console;
COORD CursorPosition;

void gotoxy(int x, int y) {
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

void GetWindowSize() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(console, &csbi);
    width = csbi.srWindow.Right - csbi.srWindow.Left - 2;
    height = csbi.srWindow.Bottom - csbi.srWindow.Top - 4;
}

void Setup() {
    console = GetStdHandle(STD_OUTPUT_HANDLE);
    GetWindowSize();
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    tailX.clear();
    tailY.clear();
    nTail = 0;
}

void DrawBorder() {
    system("cls");
    for (int i = 0; i < width + 2; i++) {
        gotoxy(i, 0);
        cout << "#";
        gotoxy(i, height + 1);
        cout << "#";
    }
    for (int i = 0; i <= height + 1; i++) {
        gotoxy(0, i);
        cout << "#";
        gotoxy(width + 1, i);
        cout << "#";
    }
}

void Draw() {
    vector<string> buffer(height, string(width, ' '));

    buffer[y][x] = 'O';
    buffer[fruitY][fruitX] = 'F';

    for (int k = 0; k < nTail; k++) {
        if (tailX[k] >= 0 && tailX[k] < width && tailY[k] >= 0 && tailY[k] < height) {
            buffer[tailY[k]][tailX[k]] = 'o';
        }
    }

    for (int i = 0; i < height; i++) {
        gotoxy(1, i + 1);
        cout << buffer[i];
    }

    gotoxy(0, height + 2);
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

    if (x >= width || x < 0 || y >= height || y < 0)
        gameOver = true;

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

void ShowInstructions() {
    system("cls");
    cout << "Welcome to the Snake Game!" << endl << endl;
    cout << "Instructions:" << endl;
    cout << "1. Use WASD keys to control the snake:" << endl;
    cout << "   W - Up" << endl;
    cout << "   S - Down" << endl;
    cout << "   A - Left" << endl;
    cout << "   D - Right" << endl;
    cout << "2. Eat fruits (F) to grow and earn points" << endl;
    cout << "3. Avoid hitting the walls or your own tail" << endl;
    cout << "4. Press X at any time to exit the game" << endl << endl;
    cout << "Press any key to start the game...";
    _getch();
}

int main() {
    srand(time(0));
    console = GetStdHandle(STD_OUTPUT_HANDLE);

    ShowInstructions();

    cout << "\nChoose difficulty (1-Easy, 2-Normal, 3-Hard): ";
    cin >> difficulty;

    Setup();
    DrawBorder();

    while (!gameOver) {
        Draw();
        Input();
        Logic();
        // Adjusted sleep times for different difficulty levels
        Sleep(difficulty == 1 ? 200 : (difficulty == 2 ? 100 : 50));

        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(console, &csbi)) {
            int newWidth = csbi.srWindow.Right - csbi.srWindow.Left - 2;
            int newHeight = csbi.srWindow.Bottom - csbi.srWindow.Top - 4;
            if (newWidth != width || newHeight != height) {
                width = newWidth;
                height = newHeight;
                Setup();
                DrawBorder();
            }
        }
    }

    gotoxy(width / 2 - 5, height / 2);
    cout << "Game Over!";
    gotoxy(width / 2 - 8, height / 2 + 1);
    cout << "Your Score: " << score;
    gotoxy(0, height + 3);

    return 0;
}