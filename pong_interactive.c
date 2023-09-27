#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#define MAX_X 80
#define MAX_Y 25
#define SIZE_R 3
#define MAX_SCORE 21
#define RACKET_START_POSITION 12
#define LEFT_RACKET_POSITION 1
#define RIGHT_RACKET_POSITION 78
#define CLR                   \
    while (getchar() != '\n') \
        ;

void print_hello_screen();
short is_ball(int ball_x, int ball_y, int axis_x, int axis_y);
short input_racket_move();
void print_game_screen(int left_racket_pos, int right_racket_pos, int ball_x, int ball_y, int score_left,
                       int score_right);
int ball_move_y(int y_dir, int x, int y, int left_racket_pos, int right_racket_pos);
int ball_move_x(int x_dir, int x, int y, int left_racket_pos, int right_racket_pos);

int main() {
    srand(time(NULL));
    initscr();
    noecho();
    nodelay(stdscr, true);
    curs_set(0);
    keypad(stdscr, 1);
    int score_left = 0, score_right = 0;
    char button = 0;
    int left_racket_pos = RACKET_START_POSITION, right_racket_pos = RACKET_START_POSITION;
    int ball_x = (rand() % 2) + 39, ball_y = (rand() % 18) + 3;
    int y_dir = ((rand() % 2) - 1) == 0 ? 1 : -1, x_dir = (ball_x == 39 ? -1 : 1);
    print_hello_screen();
    button = getch();
    while (((score_left < MAX_SCORE && score_right < MAX_SCORE) || abs(score_right - score_left) < 2) &&
           button != 27) {
        if (ball_x == LEFT_RACKET_POSITION) {
            score_right++;
            ball_x = (rand() % 2) + 39;
            ball_y = (rand() % 18) + 3;
        } else if (ball_x == RIGHT_RACKET_POSITION) {
            score_left++;
            ball_x = (rand() % 2) + 39;
            ball_y = (rand() % 18) + 3;
        }
        if ((button == 'A' || button == 'a') && left_racket_pos > 2)
            left_racket_pos--;
        else if ((button == 'Z' || button == 'z') && left_racket_pos < MAX_Y - 3)
            left_racket_pos++;
        else if ((button == 'K' || button == 'k') && right_racket_pos > 2)
            right_racket_pos--;
        else if ((button == 'M' || button == 'm') && right_racket_pos < MAX_Y - 3)
            right_racket_pos++;
        y_dir = ball_move_y(y_dir, ball_x, ball_y, left_racket_pos, right_racket_pos);
        x_dir = ball_move_x(x_dir, ball_x, ball_y, left_racket_pos, right_racket_pos);
        ball_x += x_dir;
        ball_y += y_dir;
        button = getch();
        print_game_screen(left_racket_pos, right_racket_pos, ball_x, ball_y, score_left, score_right);
    }
    if (score_left > score_right)
        mvaddstr(MAX_Y + 1, 0, "                    Congratulations to the right!");
    else
        mvaddstr(MAX_Y + 1, 0, "                    Congratulations to the left!");
    getch();
    endwin();
    return 0;
}

void print_hello_screen() {
    mvaddstr(0, 0, "                       Welcome to pong interactive game!");
    mvaddstr(1, 0, "             PLAYER 1: A/Z       SPACEBAR - SKIP      PLAYER 2: K/M");
    mvaddstr(1, 0, "                     Reach 21 points in order to WIN!");
    mvaddstr(2, 0, "                            PRESS ANY KEY...\n");
}

short is_ball(int ball_x, int ball_y, int axis_x, int axis_y) {
    return ball_x == axis_x && ball_y == axis_y ? 1 : 0;
}

void print_game_screen(int left_racket_pos, int right_racket_pos, int ball_x, int ball_y, int score_left,
                       int score_right) {
    for (int axis_y = 0; axis_y < MAX_Y; axis_y++) {
        for (int axis_x = 0; axis_x < MAX_X; axis_x++) {
            if (axis_x == 0 || axis_x == MAX_X - 1) {
                mvaddch(axis_y, axis_x, '|');
            } else if (axis_y == 0 || axis_y == MAX_Y - 1) {
                mvaddch(axis_y, axis_x, '=');
            } else if (axis_x == LEFT_RACKET_POSITION && abs(left_racket_pos - axis_y) < 2) {
                mvaddch(axis_y, axis_x, ']');
            } else if (axis_x == RIGHT_RACKET_POSITION && abs(right_racket_pos - axis_y) < 2) {
                mvaddch(axis_y, axis_x, '[');
            } else if (axis_y != MAX_Y - 1 && (axis_x == 39 || axis_x == 40) &&
                       !is_ball(ball_x, ball_y, axis_x, axis_y)) {
                mvaddch(axis_y, axis_x, '|');
            } else if (axis_x == ball_x && axis_y == ball_y) {
                mvaddch(axis_y, axis_x, '*');
            } else {
                mvaddch(axis_y, axis_x, ' ');
            }
        }
        mvaddch(axis_y, MAX_X, '\n');
    }
    usleep(100000);
}

int ball_move_y(int y_dir, int x, int y, int left_racket_pos, int right_racket_pos) {
    if (y_dir == -1 && x > LEFT_RACKET_POSITION + 1 && x < RIGHT_RACKET_POSITION - 1) {
        if (y == 1) y_dir = 1;
    } else if (y_dir == 1 && x > LEFT_RACKET_POSITION + 1 && x < RIGHT_RACKET_POSITION - 1) {
        if (y == MAX_Y - 2) y_dir = -1;
    } else if (x == LEFT_RACKET_POSITION + 1) {
        if (y == left_racket_pos)
            y_dir = 0;
        else if (y == (left_racket_pos + 1))
            y_dir = 1;
        else if (y == (left_racket_pos - 1))
            y_dir = -1;
    } else if (x == RIGHT_RACKET_POSITION - 1) {
        if (y == right_racket_pos)
            y_dir = 0;
        else if (y == (right_racket_pos + 1))
            y_dir = 1;
        else if (y == (right_racket_pos - 1))
            y_dir = -1;
    }
    return y_dir;
}

int ball_move_x(int x_dir, int x, int y, int left_racket_pos, int right_racket_pos) {
    if (x == LEFT_RACKET_POSITION + 1 && abs(left_racket_pos - y) < 2)
        x_dir = 1;
    else if (x == RIGHT_RACKET_POSITION - 1 && abs(right_racket_pos - y) < 2)
        x_dir = -1;
    return x_dir;
}
