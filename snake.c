#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <stdbool.h>
#include <windows.h>

#define EDGE 25 // EDGE * EDGE playground
#define DELAY 0.1 // in seconds

void create_table(char (*table)[EDGE + 1]);
bool game(char (*table)[EDGE + 1]);
void print_table(char (*table)[EDGE + 1], int);
void delay(double);
void generate_apple(char (*table)[EDGE + 1]);
bool choice(char (*table)[EDGE + 1]);
void ClearScreen(); // for update output
void ShowConsoleCursor(bool); // for hiding underscore cursor

int main() {
	ShowConsoleCursor(false);
	char table[EDGE][EDGE + 1];
	srand(time(NULL));
	do {
		system("cls");
		create_table(table);
		print_table(table, 0);
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
    while (flag) {
		char ch = getch();
		do {
			switch ((int) ch) {
				case 27: exit(0);
				case 97:
				{
					if (table[x][y - 1] != '%') {
						if (table[x][y - 1] == '*') {
							result++;
							generate_apple(table);
						}
						table[x][y - 1] = '0';
						table[x][y] = ' ';
						y--;
					}
					else
						flag = false;
					break;
				}
				case 100:
				{
					if (table[x][y + 1] != '%') {
						if (table[x][y + 1] == '*') {
							result++;
							generate_apple(table);
						}
						table[x][y + 1] = '0';
						table[x][y] = ' ';
						y++;
					}
					else
						flag = false;
					break;
				}
				case 119:
				{
					if (table[x - 1][y] != '%') {
						if (table[x - 1][y] == '*') {
							result++;
							generate_apple(table);
						}
						table[x - 1][y] = '0';
						table[x][y] = ' ';
						x--;
					}
					else
						flag = false;
					break;
				}
				case 115:
				{
					if (table[x + 1][y] != '%') {
						if (table[x + 1][y] == '*') {
							result++;
							generate_apple(table);
						}
						table[x + 1][y] = '0';
						table[x][y] = ' ';
						x++;
					}
					else
						flag = false;
					break;
				}
				default: printf("The game has stopped!\n"); break;
			}
			printf("%*s", 21, " ");
			print_table(table, result);
			delay(DELAY - result * 0.005);
		} while(!kbhit() && flag);
	}
	return choice(table);
}

void print_table(char (*table)[EDGE + 1], int result) {
	ClearScreen();
	printf("Canetizen Kojima Proudly Presents...\n");
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
    int milli_seconds = 1000 * number_of_seconds;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds);
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

void ClearScreen() {	
	COORD cursorPosition;	
	cursorPosition.X = 0;	
	cursorPosition.Y = 0;	
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}

void ShowConsoleCursor(bool showFlag) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}
