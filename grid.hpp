#include <vector>
#include <random>
#pragma once

using namespace std;

namespace grid {
    enum DIRECTION {
        NORTH,
        SOUTH,
        EAST,
        WEST
    };
    struct point {
        char val = ' ';
        char prop = 'E';
        int pos[2] = {0};
    };
    // Snake Grid
    point grid[32][64];
    int head_start[2] = {31, 15};
    int score = 0;

    point& getP(int x, int y) {
        return (grid[y][x]);
    }
    bool isEmpty(int x, int y) {
        if (grid[y][x].prop == 'E') {
            return true;
        } else {
            return false;
        }
    }
    class snake {
    public:
        static std::vector<point*> all;
        static int head_pos_x; static int head_pos_y;
        static DIRECTION facing;
        static void move(DIRECTION dir, DIRECTION newdir, int x, int y);
        static void runNorth() {move(SOUTH, NORTH, snake::head_pos_x, snake::head_pos_y - 1);}
        static void runSouth() {move(NORTH, SOUTH, snake::head_pos_x, snake::head_pos_y + 1);}
        static void runEast() {move(WEST, EAST, snake::head_pos_x + 1, snake::head_pos_y);}
        static void runWest() {move(EAST, WEST, snake::head_pos_x - 1, snake::head_pos_y);}
        static void addPart(int t);
    };
    std::vector<point*> snake::all = {};
    DIRECTION snake::facing = NORTH;
    int snake::head_pos_x = head_start[0]; int snake::head_pos_y = head_start[1];
    bool alive = true;
    auto nextAction = &(snake::runNorth);
    // Random Numbers
    random_device tg;
    uniform_int_distribution<int> rng_x(0,63);
    uniform_int_distribution<int> rng_y(0,31); // Explanation: auto rng_y = uniform_int_distribution<int>(0,63);
    int random_x() {return (rng_x(tg));}
    int random_y() {return (rng_y(tg));}

    void newHead(int x, int y) {
        snake::all[0]->val = 'O'; snake::all[0]->prop = 'B'; // sets head to be O and body part of snake
        point& new_head = getP(x, y); // gets position of new head of snake
        new_head.prop = 'H'; new_head.val = '@'; // sets new head of snake to be H for head and the @ char
        snake::all.insert(snake::all.begin(), &new_head); // inserts pointer to new head into all vector
        point *tail = snake::all[snake::all.size() - 1]; // gets tail of snake
        (*tail).val = ' '; // sets snake tail to be nothing
        (*tail).prop = 'E'; // marks tail as absent point
        snake::all.pop_back(); // chops it off the snake from all vector
        snake::head_pos_x = snake::all[0]->pos[0]; // sets new head X position
        snake::head_pos_y = snake::all[0]->pos[1]; // sets new head Y position
    }
    bool isAvailable(int x, int y) {
        return (grid[y][x].prop == 'E');
    }
    point& lastPSnake() {
        return *(snake::all[snake::all.size() - 1]);
    }
    // add directional checking for adding snake part
    void snake::addPart(int t = 1) {
        int modify_x = 0; int modify_y = 0;
        switch(snake::facing) {
            case NORTH: modify_y = 1; break;
            case SOUTH: modify_y = -1; break;
            case EAST: modify_x = -1; break;
            case WEST: modify_x = 1; break;
        }
        for (int i = 0; i < t; i++) {
            if (isAvailable(lastPSnake().pos[0] + modify_x, lastPSnake().pos[1] + modify_y)) {
                point& newPart = getP(lastPSnake().pos[0] + modify_x, lastPSnake().pos[1] + modify_y);
                newPart.val = 'O'; newPart.prop = 'B';
                snake::all.push_back(&newPart);
            }
        }
    }
    void newApple() {
        int x = random_x(); int y = random_y();
        point& apple = getP(x, y);
        if (apple.prop != 'E') {
            newApple();
        } else {
            apple.prop = 'A';
            apple.val = '%';
        }
    }
    void eatApple(int x, int y) {
        newHead(x, y);
        snake::addPart(1);
        newApple();
        score++;
    }
    bool isApple(int x, int y) {
        point& apple = getP(x, y);
        return (apple.prop == 'A');
    }
    void snake::move(DIRECTION dir, DIRECTION newdir, int x, int y) {
        if (snake::facing != dir) {
            if (isAvailable(x, y)) {
                newHead(x, y); // Add new head in appropriate direction
                snake::facing = newdir;
            } else if (isApple(x, y)) {
                eatApple(x, y);
            } else alive = false;
        }
    }
    void printGrid() { // prints out the grid
        int i;
        for (i = 0; i < 32; i++) {
            int j;
            for (j = 0; j < 64; j++) {
                printw("%c", grid[i][j].val);
            }
            printw("\n");
        }
    }
    void initGrid() { // sets borders on the grid to dashes
        int y;
        for (y = 0; y < 32; y++) {
            int x;
            for (x = 0; x < 64; x++) {
                grid[y][x].pos[0] = x;
                grid[y][x].pos[1] = y;
                if (y == 0 || y == 31 || x == 0 || x == 63) {
                    grid[y][x].val = '-'; grid[y][x].prop = 'R';
                }
            }
        }
    }
}