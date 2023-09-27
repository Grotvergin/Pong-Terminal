#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
    int score_left = 0, score_right = 0;
    int left_racket_pos = RACKET_START_POSITION, right_racket_pos = RACKET_START_POSITION;
    int ball_x = (rand() % 2) + 39, ball_y = (rand() % 24) + 3;
    int y_dir = ((rand() % 2) - 1) == 0 ? 1 : -1, x_dir = (ball_x == 39 ? -1 : 1);
    print_hello_screen();
    while ((score_left < MAX_SCORE && score_right < MAX_SCORE) || abs(score_right - score_left) < 2) {
        if (ball_x == LEFT_RACKET_POSITION) {
            score_right++;
            ball_x = (rand() % 2) + 39;
            ball_y = (rand() % 24) + 1;
        } else if (ball_x == RIGHT_RACKET_POSITION) {
            score_left++;
            ball_x = (rand() % 2) + 39;
            ball_y = (rand() % 24) + 1;
        }
        CLR;
        short button = input_racket_move();
        system("clear");
        if (button == 1 && left_racket_pos > 2)
            left_racket_pos -= 1;
        else if (button == 2 && left_racket_pos < MAX_Y - 3)
            left_racket_pos += 1;
        else if (button == 3 && right_racket_pos > 2)
            right_racket_pos -= 1;
        else if (button == 4 && right_racket_pos < MAX_Y - 3)
            right_racket_pos += 1;
        y_dir = ball_move_y(y_dir, ball_x, ball_y, left_racket_pos, right_racket_pos);
        x_dir = ball_move_x(x_dir, ball_x, ball_y, left_racket_pos, right_racket_pos);
        ball_x += x_dir;
        ball_y += y_dir;
        print_game_screen(left_racket_pos, right_racket_pos, ball_x, ball_y, score_left, score_right);
    }
    if (score_left > score_right)
        printf("\n                      ПОЗДРАВЛЯЕМ С ПОБЕДОЙ ИГРОКА СЛЕВА!");
    else
        printf("\n                      ПОЗДРАВЛЯЕМ С ПОБЕДОЙ ИГРОКА СПРАВА!");
    return 0;
}

short input_racket_move() {
    char button;
    if (scanf("%c", &button)) {
        if (button == 'A' || button == 'a')
            return 1;
        else if (button == 'Z' || button == 'z')
            return 2;
        else if (button == 'K' || button == 'k')
            return 3;
        else if (button == 'M' || button == 'm')
            return 4;
    }
    return 5;
}

void print_hello_screen() {
    printf("                         Добро пожаловать в пинг-понг!\n");
    printf("             ИГРОК 1: A/Z     ПРОБЕЛ - пропуск хода    ИГРОК 2: K/M\n");
    printf("      Для победы необходимо набрать %d очко (действует правило отрыва в два очка)\n", MAX_SCORE);
    printf("                            Нажмите любую клавишу для начала...\n");
}

short is_ball(int ball_x, int ball_y, int axis_x, int axis_y) {
    return ball_x == axis_x && ball_y == axis_y ? 1 : 0;
}

void print_game_screen(int left_racket_pos, int right_racket_pos, int ball_x, int ball_y, int score_left,
                       int score_right) {
    for (int axis_y = 0; axis_y < MAX_Y; axis_y++) {
        for (int axis_x = 0; axis_x < MAX_X; axis_x++) {
            if (axis_x == 0 || axis_x == MAX_X - 1) {
                printf("|");
            } else if (axis_y == 0 || axis_y == MAX_Y - 1) {
                printf("=");
            } else if (axis_x == LEFT_RACKET_POSITION && abs(left_racket_pos - axis_y) < 2) {
                printf("]");
            } else if (axis_x == RIGHT_RACKET_POSITION && abs(right_racket_pos - axis_y) < 2) {
                printf("[");
            } else if (axis_y != MAX_Y - 1 && (axis_x == 39 || axis_x == 40) &&
                       !is_ball(ball_x, ball_y, axis_x, axis_y)) {
                printf("|");
            } else if (axis_x == ball_x && axis_y == ball_y) {
                printf("*");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
    printf("                   %d                  СЧЁТ                  %d\n", score_left, score_right);
    printf("                                     ХОДИТЕ:");
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
