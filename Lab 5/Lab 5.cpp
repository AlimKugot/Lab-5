#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <conio.h>

using namespace std;

void init();
void draw();
void up();
void down();
void left();
void right();

#define HERO 24
#define BOMB 15
#define BOOM 253
#define DONE 251
#define BORDER 177

#define MAX_X 40
#define MAX_Y 10
char matrix[MAX_Y][MAX_X];

// minus 1, because indexes is starting from zero
int star_x = 5 - 1;
int star_y = 5 - 1;

int main()
{
    /*
    for (int i = 0; i < 300; i++) {
        cout << i << " " << (char)i << endl;
    }
    */

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
            printf("Exit");
            return(0);
        }
    }
}

void up() {
    if (star_y > 1) {
        matrix[star_y][star_x] = '.';
        star_y--;
        matrix[star_y][star_x] = HERO;
        draw();
    }
}

void down() {
    if (star_y + 2 < MAX_Y) {
        matrix[star_y][star_x] = '.';
        star_y++;
        matrix[star_y][star_x] = HERO;
        draw();
    }
}

void left() {
    if (star_x > 1) {
        matrix[star_y][star_x] = '.';
        star_x--;
        matrix[star_y][star_x] = HERO;
        draw();
    }
}

void right() {
    if (star_x + 2 < MAX_X) {
        matrix[star_y][star_x] = '.';
        star_x++;
        matrix[star_y][star_x] = HERO;
        draw();
    }
}


void init() {
    for (int i = 0; i < MAX_Y; i++) {
        for (int j = 0; j < MAX_X; j++) {
           matrix[i][j] = '.';
        }
    }
    matrix[star_x][star_y] = HERO;

    for (int i = 0; i < MAX_Y; i++) {
        matrix[i][0] = (char)BORDER;
        matrix[i][MAX_X - 1] = (char)BORDER;
    }
    for (int j = 0; j < MAX_X; j++) {
        matrix[0][j] = BORDER;
        matrix[MAX_Y - 1][j] = BORDER;
    }
}

void draw() {
    system("cls");

    for (int i = 0; i < MAX_Y; i++) {
        for (int j = 0; j < MAX_X; j++) {
            cout << matrix[i][j];
        }
        cout << endl;
    }
}