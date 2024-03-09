#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <stdbool.h>

// Playground settings
#define EDGE 25 // EDGE * EDGE playground
#define DELAY 65 // in milliseconds
#define EDGE_TYPE '%'
#define SNAKE_HEAD 'X'
#define SNAKE_BODY 'o'
#define BAIT '*'

// Control settings
#define ESC 27
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'

typedef struct Body {
    char symbol;
    int x;
    int y;
    struct Body* next;
} Body;

typedef struct Snake {
    struct Body* snake_head;
} Snake;

void create_table(char (*table)[EDGE + 1], Snake*);
bool game(char (*table)[EDGE + 1], Snake*, int*);
void print_table(char (*table)[EDGE + 1], int*, int*);
void delay(double);
void generate_apple(char (*table)[EDGE + 1]);
bool choice(char (*table)[EDGE + 1]);
void shift(int, int, char (*table)[EDGE + 1], Snake*);
void eat(int, int, char (*table)[EDGE + 1], Snake*, int*);
void destructor(Snake*);
Body* constructor(int, int, char);
void ncurses_config();

int main() {
	ncurses_config();	
    Snake* snake = (Snake*)malloc(sizeof(Snake));
    snake->snake_head = NULL;
    char table[EDGE][EDGE + 1];
    srand(time(NULL));
    int best = 0;
    do {
        clear();
        if (snake->snake_head != NULL)
            destructor(snake);
        snake->snake_head = constructor(EDGE / 2, EDGE / 2, SNAKE_HEAD);
        create_table(table, snake);
        int initial = 0;
        print_table(table, &initial, &best);
    } while (game(table, snake, &best));
    destructor(snake);
    free(snake);
    snake = NULL;
	endwin();
    return EXIT_SUCCESS;
}

void create_table(char (*table)[EDGE + 1], Snake* snake) {
    for (int i = 0; i < EDGE; i++)
        for (int j = 0; j < EDGE; j++)
            table[i][j] = ' ';

    for (int i = 0; i < EDGE; i++) {
        table[0][i] = EDGE_TYPE;
        table[i][0] = EDGE_TYPE;
        table[EDGE - 1][i] = EDGE_TYPE;
        table[i][EDGE - 1] = EDGE_TYPE;
    }
    table[EDGE / 2][EDGE / 2] = snake->snake_head->symbol;
}

bool game(char (*table)[EDGE + 1], Snake* snake, int* best) {
    int result = 0;
    generate_apple(table);
    bool flag = true;
    int previous_direction = -1;
	int ch; // Current movement
	int movement_check = ERR;
    while (flag) {
		if (previous_direction == -1) {
			ch = getch(); // First movement
			nodelay(stdscr, TRUE);
		}
		
		switch (ch) {
			case ESC:
				return false;
			case LEFT:
				if (previous_direction != RIGHT) {
					if (table[snake->snake_head->x][snake->snake_head->y - 1] != EDGE_TYPE &&
						table[snake->snake_head->x][snake->snake_head->y - 1] != SNAKE_BODY) {
						if (table[snake->snake_head->x][snake->snake_head->y - 1] == BAIT) {
							eat(snake->snake_head->x, snake->snake_head->y - 1, table, snake, &result);
						} else {
							shift(snake->snake_head->x, snake->snake_head->y - 1, table, snake);
						}
						previous_direction = LEFT;
					} else
						flag = false;
					break;
				} else {
					ch = (int) RIGHT;
					continue;
				}
			case RIGHT:
				if (previous_direction != LEFT) {
					if (table[snake->snake_head->x][snake->snake_head->y + 1] != EDGE_TYPE &&
						table[snake->snake_head->x][snake->snake_head->y + 1] != SNAKE_BODY) {
						if (table[snake->snake_head->x][snake->snake_head->y + 1] == BAIT) {
							eat(snake->snake_head->x, snake->snake_head->y + 1, table, snake, &result);
						} else {
							shift(snake->snake_head->x, snake->snake_head->y + 1, table, snake);
						}
						previous_direction = RIGHT;
					} else
						flag = false;
					break;
				} else {
					ch = (int) DOWN;
					continue;
				}
			case UP:
				if (previous_direction != DOWN) {
					if (table[snake->snake_head->x - 1][snake->snake_head->y] != EDGE_TYPE &&
						table[snake->snake_head->x - 1][snake->snake_head->y] != SNAKE_BODY) {
						if (table[snake->snake_head->x - 1][snake->snake_head->y] == BAIT) {
							eat(snake->snake_head->x - 1, snake->snake_head->y, table, snake, &result);
						} else {
							shift(snake->snake_head->x - 1, snake->snake_head->y, table, snake);
						}
						previous_direction = UP;
					} else
						flag = false;
					break;
				} else {
					ch = (int) DOWN;
					continue;
				}
			case DOWN:
				if (previous_direction != UP) {
					if (table[snake->snake_head->x + 1][snake->snake_head->y] != EDGE_TYPE &&
						table[snake->snake_head->x + 1][snake->snake_head->y] != SNAKE_BODY) {
						if (table[snake->snake_head->x + 1][snake->snake_head->y] == BAIT) {
							eat(snake->snake_head->x + 1, snake->snake_head->y, table, snake, &result);
						} else {
							shift(snake->snake_head->x + 1, snake->snake_head->y, table, snake);
						}
						previous_direction = DOWN;
					} else
						flag = false;
					break;
				} else {
					ch = (int) UP;
					continue;
				}
			default:
				printw("\nThe game has stopped!\n");
				nodelay(stdscr, FALSE);
				ch = getch();
				nodelay(stdscr, TRUE);
				continue;
		}
		if (result > *best)
			*best = result;
		print_table(table, &result, best);
		delay((DELAY - result * 0.05) * 1000);

		movement_check = getch(); // If any user input is given
		if (movement_check != ERR) { // No user input
			ch = movement_check;
		}
    }
    return choice(table);
}

void print_table(char (*table)[EDGE + 1], int* result, int* best) {
	clear();
    printw("Canetizen Proudly Presents...\n");
    printw("Press [w] - [a] - [s] - [d] to play. Press [ESC] to quit.\n");
    printw("\n");
    for (int i = 0; i < EDGE; i++) {
        for (int j = 0; j < EDGE; j++) {
            if (table[i][j] == SNAKE_HEAD || table[i][j] == SNAKE_BODY)
                printw("%c", table[i][j]);
            else
                printw("%c", table[i][j]);
        }
        printw("\n");
    }
    printw("Best Score: %d\n", *best);
    printw("Score: %d\n", *result);
    refresh();
}

void delay(double n_of_milliseconds) {
    clock_t start_time = clock();
    while (clock() < start_time + n_of_milliseconds);
}

void generate_apple(char (*table)[EDGE + 1]) {
    int x_axis, y_axis;
    do {
        x_axis = (rand() % (EDGE - 2)) + 1;
        y_axis = (rand() % (EDGE - 2)) + 1;
    } while (table[x_axis][y_axis] == SNAKE_HEAD || table[x_axis][y_axis] == SNAKE_BODY);
    table[x_axis][y_axis] = BAIT;
}

bool choice(char (*table)[EDGE + 1]) {
    char flag;
    do {
        printw("\nYou lost. Try again -> [y], Quit -> [n]\n");
		nodelay(stdscr, FALSE);
        flag = getch();
    } while (flag != 'y' && flag != 'n');
    if (flag == 'n' || (int)flag == 27) {
        return false;
    }
    return true;
}

void destructor(Snake* snake) {
    Body* temp = snake->snake_head;
    Body* temp2;
    while (temp != NULL) {
        temp2 = temp->next;
        free(temp);
        temp = temp2;
    }
    snake->snake_head = NULL;
}

Body* constructor(int x, int y, char symbol) {
    Body* new_body = (Body*)malloc(sizeof(Body));
    new_body->next = NULL;
    new_body->symbol = symbol;
    new_body->x = x;
    new_body->y = y;
    return new_body;
}

void shift(int x, int y, char (*table)[EDGE + 1], Snake* snake) {
    Body* new_body = constructor(x, y, SNAKE_HEAD);
    Body* temp = snake->snake_head;
    if (snake->snake_head->next != NULL) {
        while (temp->next->next != NULL)
            temp = temp->next;
        table[temp->next->x][temp->next->y] = ' ';
        free(temp->next);
        temp->next = NULL;
        new_body->next = snake->snake_head;
        snake->snake_head->symbol = SNAKE_BODY;
        table[snake->snake_head->x][snake->snake_head->y] = snake->snake_head->symbol;
        snake->snake_head = new_body;
    } else {
        Body* prev_head = snake->snake_head;
        table[snake->snake_head->x][snake->snake_head->y] = ' ';
        snake->snake_head = new_body;
        free(prev_head);
        prev_head = NULL;
    }
    table[new_body->x][new_body->y] = new_body->symbol;
}

void eat(int x, int y, char (*table)[EDGE + 1], Snake* snake, int* result) {
    Body* new_body = constructor(snake->snake_head->x, snake->snake_head->y, SNAKE_BODY);
    new_body->next = snake->snake_head->next;
    snake->snake_head->next = new_body;
    snake->snake_head->x = x;
    snake->snake_head->y = y;
    table[new_body->x][new_body->y] = new_body->symbol;
    table[snake->snake_head->x][snake->snake_head->y] = snake->snake_head->symbol;
    generate_apple(table);
    (*result)++;
}

void ncurses_config() {
	initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
}