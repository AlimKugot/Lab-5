#include <iostream>
#include <queue>
#include <sstream>
#include <chrono>
#include <thread>
#include <conio.h>
#include <random>

using namespace std;

struct Bomb {
    int x = -1;
    int y = -1;
    int time = 2;
};

vector<Bomb> bombs;

void init();
void draw();
void plant_bomb();
void decrease_bomb_time();
void blow_up(int x, int y);
void blow_up(Bomb bomb);
void reload();
void up();
void down();
void left();
void right();
void win();
void lose();

#define HERO 24
#define BOMB 15
#define BOOM 43
#define DONE 251
#define BORDER 177
#define AIM 176
#define HEART 3

#define MAX_X 40
#define MAX_Y 10
char matrix[MAX_Y][MAX_X];

int lives = 3;
int level = 1;

// main aim to blow up
int main_door_x = -1;
int main_door_y = -1;

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
        if (_kbhit() && input == -32) {
           input = _getch();
        }
        switch (input) {
        case 82: // R
        case 114: // r
            reload();
            break;
        case 32: // space
            plant_bomb();
            break;
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
    if (ch == '.') {
        if (matrix[star_y][star_x] != (char) BOMB) {
            matrix[star_y][star_x] = '.';
        }
        decrease_bomb_time();
        star_y--;
        matrix[star_y][star_x] = HERO;
        draw();
    }
}

void down() {
    char ch = matrix[star_y + 1][star_x];
    if (ch == '.') {
        if (matrix[star_y][star_x] != (char) BOMB) {
            matrix[star_y][star_x] = '.';
        }
        decrease_bomb_time();
        star_y++;
        matrix[star_y][star_x] = HERO;
        draw();
    }
}

void left() {
    char ch = matrix[star_y][star_x - 1];
    if (ch == '.') {
        if (matrix[star_y][star_x] != (char)BOMB) {
            matrix[star_y][star_x] = '.';
        }
        decrease_bomb_time();
        star_x--;
        matrix[star_y][star_x] = HERO;
        draw();
    }
}

void right() {
    char ch = matrix[star_y][star_x + 1];
    if (ch == '.') {
        if (matrix[star_y][star_x] != (char)BOMB) {
            matrix[star_y][star_x] = '.';
        }
        decrease_bomb_time();
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
    std::random_device rd; // obtain a random number from hardware
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
    
    cout << "Level: " << level << "\t\t\t     " << string(lives, (char)HEART) << endl;
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
    cout << "R \t reload hero position (minus life)" << endl;
    cout << "Space \t plant bomb" << endl;
}

void decrease_bomb_time() {
    for (auto &b : bombs) {
        b.time--;
        if (b.time == 0) {
            blow_up(b);
        }
    }
}

void check_to_blow_up(int x, int y) {
    char ch = matrix[y][x];
    if (ch == '.') {
        matrix[y][x] = BOOM;
    }
    else if (ch == (char)BOMB) {
        blow_up(x, y);
    }
    else if (ch == (char) AIM) {
        matrix[y][x] = BOMB;
    }
    else if (ch == HERO) {
        lives--;
        if (lives == 0) lose();
    }
}

void blow_up(int x, int y) {
    for (auto& bomb : bombs) {
        if (bomb.x = x && bomb.y == y) {
            blow_up(bomb);
        }
    }
}

void blow_up(Bomb bomb) {
    int x = bomb.x;
    int y = bomb.y;
    matrix[y][x] = BOOM;
    for (int i = 0; i < 2; i++) {
        if (x - i > 0) check_to_blow_up(x - i, y);
        if (x + i < MAX_X - 1) check_to_blow_up(x + i, y);
        if (y - i > 0) check_to_blow_up(x, y - i);
        if (y + i < MAX_Y - 1) check_to_blow_up(x, y + i);
    }
}



void plant_bomb() {
    matrix[star_y][star_x] = BOMB;
    Bomb bomb;
    bomb.x = star_x;
    bomb.y = star_y;
    bombs.push_back(bomb);
    draw();
}

void reload() {
    lives--;
    if (lives == 0) lose();
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distrX(1, MAX_X - 2); // define the range
    std::uniform_int_distribution<> distrY(1, MAX_Y - 2); // define the range

    while (1) {
        int x = distrX(gen);
        int y = distrY(gen);
        
        if (matrix[y][x] == '.') {
            matrix[star_y][star_x] = '.';
            star_x = x;
            star_y = y;
            matrix[star_y][star_x] = HERO;
            draw();
            break;
        }
    }
}

void win() {
    system("cls");
    cout << "You win! :)" << endl;
    exit(0);
}

void lose() {
    system("cls");
    cout << "You lose! :(" << endl;
    exit(0);
}