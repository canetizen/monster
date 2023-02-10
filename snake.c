#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <stdbool.h>

//playground settings
#define EDGE 25 // EDGE * EDGE playground
#define DELAY 50 // in miliseconds
#define EDGE_TYPE '%'
#define SNAKE_HEAD '0'
#define BAIT '*'

//control settings
#define ESC 27
#define UP 100
#define DOWN 97
#define LEFT 119
#define RIGHT 115

typedef struct Body{
	char symbol = ' ';
	int x = 0;
	int y = 0;
	struct Body* next = NULL;
} Body;

typedef struct {
	struct Body* snake_head = NULL;
} Snake;

void create_table(char (*table)[EDGE + 1], Snake*);
bool game(char (*table)[EDGE + 1], Snake*);
void print_table(char (*table)[EDGE + 1], int&);
void delay(double);
void generate_apple(char (*table)[EDGE + 1]);
bool choice(char (*table)[EDGE + 1], Snake*);
void shift(int, int, char (*table)[EDGE + 1], Snake*);
void destructor(Snake*);
Body* constructor (Snake*, int, int, char);

int main() {
	Snake* snake;
	int center = EDGE / 2;
	char table[EDGE][EDGE + 1];
	srand(time(NULL));
	do {
		snake = (Snake*) malloc(sizeof(Snake));
		snake->snake_head = constructor(snake, center, center, SNAKE_HEAD);
		system("clear");
		create_table(table, snake);
		int initial = 0;
		print_table(table, initial);
	} while(game(table, snake));

	destructor(snake);
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

bool game(char (*table)[EDGE + 1], Snake* snake) {
	int result = 0;
	generate_apple(table);
	bool flag = true;
	int previous_direction = -1;
    while (flag) {
		int ch = (int) getch();
		do {
			switch (ch) {
				case ESC: 
				{
					destructor(snake);
					exit(0);
				}
				case DOWN:
				{
					if (previous_direction != UP) {
						if (table[snake->snake_head->x][snake->snake_head->y - 1] != EDGE_TYPE) {
							if (table[snake->snake_head->x][snake->snake_head->y - 1] == '*') {
								shift(snake->snake_head->x, snake->snake_head->y - 1, table, snake);
								generate_apple(table);
								result++;
							}
							else {
								shift(snake->snake_head->x, snake->snake_head->y - 1, table, snake);
							}		
							previous_direction = DOWN;				
						}
						else
							flag = false;
						break;
					}
					else {
						ch = UP;
						continue;
					}
				}
				case UP:
				{
					if (previous_direction != DOWN) {
						if (table[snake->snake_head->x][snake->snake_head->y + 1] != EDGE_TYPE) {
							if (table[snake->snake_head->x][snake->snake_head->y + 1] == '*') {
								shift(snake->snake_head->x, snake->snake_head->y + 1, table, snake);
								generate_apple(table);
								result++;
							}
							else {
								shift(snake->snake_head->x, snake->snake_head->y + 1, table, snake);
							}
							previous_direction = UP;	
						}
						else
							flag = false;
						break;
					}
					else {
						ch = DOWN;
						continue;
					}
				}
				case LEFT:
				{
					if (previous_direction != RIGHT) {
						if (table[snake->snake_head->x - 1][snake->snake_head->y] != EDGE_TYPE) {
							if (table[snake->snake_head->x - 1][snake->snake_head->y] == '*') {
								shift(snake->snake_head->x - 1, snake->snake_head->y, table, snake);
								generate_apple(table);
								result++;
							}
							else {
								shift(snake->snake_head->x - 1, snake->snake_head->y, table, snake);
							}
							previous_direction = LEFT;					
						}
						else
							flag = false;
						break;
					}
					else {
						ch = RIGHT;
						continue;
					}
				}
				case RIGHT:
				{
					if (previous_direction != LEFT) {
						if (table[snake->snake_head->x + 1][snake->snake_head->y] != EDGE_TYPE) {
							if (table[snake->snake_head->x + 1][snake->snake_head->y] == '*') {
								shift(snake->snake_head->x + 1, snake->snake_head->y, table, snake);
								generate_apple(table);
								result++;
							}
							else {
								shift(snake->snake_head->x + 1, snake->snake_head->y, table, snake);
							}
							previous_direction = RIGHT;
						}
						else
							flag = false;
						break;
					}
					else {
						ch = LEFT;
						continue;
					}
				}
				default:
				{
					ch = previous_direction;
					continue;
				}
			}
			print_table(table, result);
			delay(DELAY - result * 0.1);
		} while(!kbhit() && flag);
	}
	return choice(table, snake);
}

void print_table(char (*table)[EDGE + 1], int &result) {
	system("clear");
	printf("Canetizen Proudly Presents...\n");
	printf("Press [w] - [a] - [s] - [d] to play. Press [ESC] to quit.\n");
	printf("\n");
	for (int i = 0; i < EDGE; i++) {
		for (int j = 0; j < EDGE; j++)
			printf("%c", table[i][j]);
		printf("\n");
	}
	printf("Score: %d\n", result);
}

void delay(double number_of_seconds) {
    int ms = 1000 * number_of_seconds;
    clock_t start_time = clock();
    while (clock() < start_time + ms);
}

void generate_apple(char (*table)[EDGE + 1]) {
	int x_axis, y_axis;
	do {
		x_axis = (rand() %(EDGE - 2)) + 1;
		y_axis = (rand() %(EDGE - 2)) + 1;
	} while (table[x_axis][y_axis] == SNAKE_HEAD || table[x_axis][y_axis] == '.');
	table[x_axis][y_axis] = BAIT;
}

bool choice(char (*table)[EDGE + 1], Snake* snake) {
	char flag;
	do {
		printf("You lost. Try again -> [y], Quit -> [n]\n");
		flag = getch();
	} while (flag != 'y' && flag != 'n');
	if (flag == 'n'|| (int) flag == 27)
		return false;
	destructor(snake);
	return true;
}

void destructor(Snake* snake) {
	Body* temp = snake->snake_head;
	while (temp != NULL) {
		Body* temp2 = temp->next;
		free(temp);
		temp = temp2;
	}
}

Body* constructor(Snake* snake, int x, int y, char symbol) {
	Body* new_body = (Body*) malloc(sizeof(Body));
	new_body->symbol = symbol;
	new_body->x = x;
	new_body->y = y;
	return new_body;
}

void shift(int x, int y, char (*table)[EDGE + 1], Snake* snake) {
	Body* temp = snake->snake_head;
	while(temp->next != NULL) {
		temp->next->x = temp->x;
		temp->next->y = temp->y;
		table[temp->next->x][temp->next->y] = temp->next->symbol;
		temp = temp->next;
	}
	table[temp->x][temp->y] = ' ';
	snake->snake_head->x = x;
	snake->snake_head->y = y;
	table[snake->snake_head->x][snake->snake_head->y] = snake->snake_head->symbol;
}