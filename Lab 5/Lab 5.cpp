#include <iostream>
#include "SDL.h"
#include "SDL_mixer.h"
#include <sstream>
#include <chrono>
#include <thread>
#include <conio.h>
#include <random>

#define MUSIC_PATH "hotline_miami.mp3"
#define SOUND_WIN "win.wav"
#define SOUND_BOOM "small-explosion.wav"
#define SOUND_DEAD "dead.wav"

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

// level info 
int lives = 3;
int level = 1;

// main aim to blow up
int main_door_x = -1;
int main_door_y = -1;

// minus 1, because indexes is starting from zero
int star_x = 5 - 1;
int star_y = 5 - 1;

#undef main
int main()
{
    int result = 0;
    int flags = MIX_INIT_MP3;

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("Failed to init SDL\n");
        exit(1);
    }

    if (flags != (result = Mix_Init(flags))) {
        printf("Could not initialize mixer (result: %d).\n", result);
        printf("Mix_Init: %s\n", Mix_GetError());
        exit(1);
    }

    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
    Mix_Music* music = Mix_LoadMUS(MUSIC_PATH);
    Mix_PlayMusic(music, 1);

    while (!SDL_QuitRequested()) {
        SDL_Delay(250);
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
                SDL_Quit();
                return(0);
            }
        }
    }
    Mix_FreeMusic(music);
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
    else if (ch == (char) DONE) {
        win();
    }
}

void down() {
    char ch = matrix[star_y + 1][star_x];
    if (ch == '.') {
        if (matrix[star_y][star_x] != (char)BOMB) {
            matrix[star_y][star_x] = '.';
        }
        decrease_bomb_time();
        star_y++;
        matrix[star_y][star_x] = HERO;
        draw();
    }
    else if (ch == (char) DONE) {
        win();
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
    else if (ch == (char) DONE) {
        win();
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
    else if (ch == (char) DONE) {
        win();
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

    int x = distrX(gen);
    int y = distrY(gen);
    main_door_x = x;
    main_door_y = y;
    matrix[y][x] = AIM;
    for (int i = 1; i < level * 7; i++) {
        x = distrX(gen);
        int y = distrY(gen);
        matrix[y][x] = AIM;
    }
}

void init() {
    if (lives <= 0) {
        level = 1;
        lives = 3;
    }
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
        if (x == main_door_x && y == main_door_y) {
            matrix[y][x] = DONE;
        }
        else {
            matrix[y][x] = BOMB;
        }
    }
    else if (ch == HERO) {
        lives--;
        if (lives == 0) lose();
    }
}

void blow_up(int x, int y) {
    matrix[y][x] = BOMB;
    Bomb bomb;
    bomb.x = x;
    bomb.y = y;
    bombs.push_back(bomb);
    blow_up(bomb);
}

void blow_up(Bomb bomb) {
    int x = bomb.x;
    int y = bomb.y;
    matrix[y][x] = BOOM;

    //todo: check here to don't blow up
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
    if (level == 10) {
        cout << "You win! :)" << endl;
        exit(0);
    }
    else {
        cout << "Next level -_-" << endl;
        lives = 3;
        level++;
        main();
    }
}

void lose() {
    system("cls");
    cout << "You lose! :(" << endl;
    cout << "\n\n\n";
    cout << "Do you want again? [yes - 1|no = 0]: ";
    char ch;
    cin >> ch;
    if (ch == '1') {
        main();
    }
    else {
        exit(0);
    }
}