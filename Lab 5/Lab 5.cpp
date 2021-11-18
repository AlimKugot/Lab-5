#include <iostream>
#include <queue>
#include <sstream>
#include <chrono>
#include <thread>
#include <conio.h>
#include <random>

using namespace std;

void init();
void draw();
void up();
void down();
void left();
void right();

#define HERO 24
#define BOMB 15
#define BOOM 43
#define DONE 251
#define BORDER 177
#define AIM 176

#define MAX_X 40
#define MAX_Y 10
char matrix[MAX_Y][MAX_X];

// main aim to blow up
int main_door_x = -1;
int main_door_y = -1;

// minus 1, because indexes is starting from zero
int star_x = 5 - 1;
int star_y = 5 - 1;

struct Bomb {
    int x;
    int y;
    int time = 3;
};

queue<Bomb> bombs;

int main()
{
    /*
    for (int i = 0; i < 300; i++) {
        cout << i << " " << (char)i << endl;
    }
    */

    // queue<>
    // todo: plant_bomb()
    // todo: boom in 2 steps
    // bomb or done

    init();
    draw();
    while (1) {
        char input = _getch();
        if (_kbhit()) {
            if (input == -32) {
                input = _getch();
            }
        }
//        this_thread::sleep_for(chrono::milliseconds((speed - 10) * 10));
        switch (input) {
        case 72:
            up();
            break;
        case 75:
            left();
            break;
        case 77:
            right();
            break;
        case 80:
            down();
            break;
        case 27: // esc
            return(0);
        }
    }
}

void up() {
    char ch = matrix[star_y - 1][star_x];
    if (star_y > 1 && ch != (char) AIM) {
        matrix[star_y][star_x] = '.';
        star_y--;
        matrix[star_y][star_x] = HERO;
        draw();
    }
}

void down() {
    char ch = matrix[star_y + 1][star_x];
    if (star_y + 2 < MAX_Y && ch != (char) AIM) {
        matrix[star_y][star_x] = '.';
        star_y++;
        matrix[star_y][star_x] = HERO;
        draw();
    }
}

void left() {
    char ch = matrix[star_y][star_x - 1];
    if (star_x > 1 && ch != (char) AIM) {
        matrix[star_y][star_x] = '.';
        star_x--;
        matrix[star_y][star_x] = HERO;
        draw();
    }
}

void right() {
    char ch = matrix[star_y][star_x + 1];
    if (star_x + 2 < MAX_X && ch != (char) AIM) {
        matrix[star_y][star_x] = '.';
        star_x++;
        matrix[star_y][star_x] = HERO;
        draw();
    }
}

void init_hero() {
    matrix[star_x][star_y] = HERO;
}

void init_borders() {
    for (int i = 0; i < MAX_Y; i++) {
        matrix[i][0] = BORDER;
        matrix[i][MAX_X - 1] = BORDER;
    }
    for (int j = 0; j < MAX_X; j++) {
        matrix[0][j] = BORDER;
        matrix[MAX_Y - 1][j] = BORDER;
    }
}

void init_aims() {
    std:random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distrX(2, MAX_X - 2); // define the range
    std::uniform_int_distribution<> distrY(2, MAX_Y - 2); // define the range

    for (int i = 0; i < 40; i++) {
        const int x = distrX(gen);
        const int y = distrY(gen);
        if (i == 20) {
            main_door_x = x;
            main_door_y = y;
        }
        matrix[y][x] = AIM;
    }
}


void init() {
    for (int i = 0; i < MAX_Y; i++) {
        for (int j = 0; j < MAX_X; j++) {
           matrix[i][j] = '.';
        }
    }
    init_hero();
    init_borders();
    init_aims();
}

void draw() {
    system("cls");

    for (int i = 0; i < MAX_Y; i++) {
        for (int j = 0; j < MAX_X; j++) {
            cout << matrix[i][j];
        }
        cout << endl;
    }

    cout << "\n\n\n";
    cout << (char) 24 << " \t " << "up" << endl;
    cout << (char) 26 << " \t " << "right" << endl;
    cout << (char) 25 << " \t " << "down" << endl;
    cout << "←" << " \t " << "left" << endl;

    cout << "Esc \t exit" << endl;
    cout << "Space \t Plant bomb" << endl;
}