#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <stdbool.h>

#define EDGE 25 // EDGE * EDGE playground
#define DELAY 50 // in miliseconds
#define ESC 27
#define UP 100
#define DOWN 97
#define LEFT 119
#define RIGHT 115

void create_table(char (*table)[EDGE + 1]);
bool game(char (*table)[EDGE + 1]);
void print_table(char (*table)[EDGE + 1], int&);
void delay(double);
void generate_apple(char (*table)[EDGE + 1]);
bool choice(char (*table)[EDGE + 1]);
void insert(int&, int&, char (*table)[EDGE + 1], int&);
void move_to_left(int&, int&, char (*table)[EDGE + 1], int&, int&);
void move_to_right(int&, int&, char (*table)[EDGE + 1], int&, int&);
void move_to_up(int&, int&, char (*table)[EDGE + 1], int&, int&);
void move_to_down(int&, int&, char (*table)[EDGE + 1], int&, int&);


int main() {
	char table[EDGE][EDGE + 1];
	srand(time(NULL));
	do {
		system("clear");
		create_table(table);
		int initial = 0;
		print_table(table, initial);
	} while(game(table));
	
	return EXIT_SUCCESS;
} 

void create_table(char (*table)[EDGE + 1]) {
	for (int i = 0; i < EDGE; i++)
		for (int j = 0; j < EDGE; j++)
			table[i][j] = ' ';
	
	for (int i = 0; i < EDGE; i++) {
		table[0][i] = '%';
		table[i][0] = '%';
		table[EDGE - 1][i] = '%';
		table[i][EDGE - 1] = '%';
	}
	table[EDGE / 2][EDGE / 2] = '0';	
}

bool game(char (*table)[EDGE + 1]) {
	int x = EDGE / 2, y = EDGE / 2;
	int result = 0;
	generate_apple(table);
	bool flag = true;
	int previous_direction = -1;
    while (flag) {
		int ch = (int) getch();
		do {
			switch (ch) {
				case ESC: exit(0);
				case DOWN:
				{
					if (previous_direction != UP) {
						if (table[x][y - 1] != '%' && previous_direction != UP) {
							move_to_down(x, y, table, result, previous_direction);
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
						if (table[x][y + 1] != '%' && previous_direction != DOWN) {
							move_to_up(x, y, table, result, previous_direction);
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
						if (table[x - 1][y] != '%' && previous_direction != RIGHT) {
							move_to_left(x, y, table, result, previous_direction);
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
						if (table[x + 1][y] != '%') {
							move_to_right(x, y, table, result, previous_direction);
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
			delay(DELAY - result * 0.01);
		} while(!kbhit() && flag);
	}
	return choice(table);
}

void print_table(char (*table)[EDGE + 1], int &result) {
	system("clear");
	fflush(stdin);
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
	} while (table[x_axis][y_axis] == 0);
	table[x_axis][y_axis] = '*';
}

bool choice(char (*table)[EDGE + 1]) {
	char flag;
	do {
		printf("You lost. Try again -> [y], Quit -> [n]\n");
		flag = getch();
	} while (flag != 'y' && flag != 'n');
	if (flag == 'n'|| (int) flag == 27)
		return false;
	return true;
}

void insert(int &x, int &y, char (*table)[EDGE + 1], int &result) {
	if (table[x][y] == '*') {
		result++;
		generate_apple(table);
	}
	table[x][y] = '0';
}

void move_to_left(int &x, int &y, char (*table)[EDGE + 1], int &result, int &previous_direction) {
	int new_value = x - 1;
	insert(new_value, y, table, result);
	table[x][y] = ' ';
	x--;
	previous_direction = LEFT;
}

void move_to_right(int &x, int &y, char (*table)[EDGE + 1], int &result, int &previous_direction) {
	int new_value = x + 1;
	insert(new_value, y, table, result);
	table[x][y] = ' ';
	x++;
	previous_direction = RIGHT;
}

void move_to_up(int &x, int &y, char (*table)[EDGE + 1], int &result, int &previous_direction) {
	int new_value = y + 1;
	insert(x, new_value, table, result);
	table[x][y] = ' ';
	y++;
	previous_direction = UP;
}

void move_to_down(int &x, int &y, char (*table)[EDGE + 1], int &result, int &previous_direction) {
	int new_value = y - 1;
	insert(x, new_value, table, result);
	table[x][y] = ' ';
	y--;
	previous_direction = DOWN;
}