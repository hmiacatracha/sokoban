#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "domain.h"
#include "search.h"
/*Variable globa de columnas se modifica en el loadDomain y se necesita en el showOperator*/
int COLS, ROWS;

tState *loadDomain(char *fileName) {
	int *rows = (int*) malloc(sizeof(int));
	int *cols = (int*) malloc(sizeof(int));
	int *goals = (int*) malloc(sizeof(int));
	char ** matriz;
	tState *s = (tState*) malloc(sizeof(tState));
	matriz = loadFile(fileName, rows, cols, goals);
	numOperators = 4 * (*cols) * (*rows);
	s->array = matriz;
	s->cols = *cols;
	s->goals = *goals;
	s->rows = *rows;
	COLS = *cols;
	ROWS = *rows;
	return s;

}
void freeDomain() {
}

int goalTest(tState *s) {
	int i, j;
	for (i = 0; i < s->rows; i++) {
		for (j = 0; j < s->cols; j++) {
			if (s->array[i][j] == '.') {
				return 0;
			}
		}
	}
	return 1;
}
void floodFillAlgorithm(int **floodFill, tState *s, int fil, int col) {
	if (floodFill[fil][col] != 0) {

	} else {
		floodFill[fil][col] = 1;
		if (col > 0) {
			if ((s->array[fil][col - 1] != '#')
					|| (s->array[fil][col - 1] != '$')
					|| (s->array[fil][col - 1] != '*')) {
				floodFillAlgorithm(floodFill, s, fil, col - 1);
			}
		}
		if (col < s->cols - 1) {
			if ((s->array[fil][col + 1] != '#')
					|| (s->array[fil][col + 1] != '$')
					|| (s->array[fil][col + 1] != '*')) {
				floodFillAlgorithm(floodFill, s, fil, col + 1);
			}
		}
		if (fil < s->rows - 1) {
			if ((s->array[fil + 1][col] != '#')
					|| (s->array[fil + 1][col] != '$')
					|| (s->array[fil + 1][col] != '*')) {
				floodFillAlgorithm(floodFill, s, fil + 1, col);
			}
		}
		if (fil > 0) {
			if ((s->array[fil - 1][col] != '#')
					|| (s->array[fil - 1][col] != '$')
					|| (s->array[fil - 1][col] != '*')) {
				floodFillAlgorithm(floodFill, s, fil - 1, col);
			}
		}
	}
}
void obtenerPosicionOperario(tState *s, int *coordenadaX, int *coordenadaY) {
	int x, y;
	for (x = 0; x < s->rows; x++) {
		for (y = 0; y < s->cols; y++) {
			if ((s->array[x][y] == '@') || (s->array[x][y] == '+')) {
				*coordenadaX = x;
				*coordenadaY = y;
			}
		}

	}
}

int executable(unsigned op, tState *s) {
	int i, j;
	int **floodFill = (int**) malloc(sizeof(int*) * s->rows);
	for (i = 0; i < s->rows; i++) {
		floodFill[i] = (int *) malloc(sizeof(int) * s->cols);
	}
	for (i = 0; i < s->rows; i++) {
		for (j = 0; j < s->cols; j++) {
			floodFill[i][j] = 0;
		}
	}
	int x, y;
	obtenerPosicionOperario(s, &x, &y);
	floodFillAlgorithm(floodFill, s, x, y);

	int row, col, operator;
	row = op / (COLS * 4);
	col = (op - row * 4 * COLS) / 4;
	operator = op % 4;
	switch (operator) {
	case 0:
		if (row < ROWS - 1) {
			if (('.' == s->array[row + 1][col])
					|| (' ' == s->array[row + 1][col])) {
				if (floodFill[row - 1][col] == 1) {
					return 1;
				}
			}

		}
		break;
	case 1:
		if (col < COLS - 1) {
			if (('.' == s->array[row][col + 1])
					|| (' ' == s->array[row][col + 1])) {
				if (floodFill[row][col - 1] == 1) {
					return 1;
				}
			}
		}
		break;
	case 2:
		if (row >= 1) {
			if (('.' == s->array[row - 1][col])
					|| (' ' == s->array[row - 1][col])) {
				if (floodFill[row + 1][col] == 1) {
					return 1;
				}
			}
		}
		break;
	case 3:
		if (col >= 1) {
			if (('.' == s->array[row][col - 1])
					|| (' ' == s->array[row][col - 1])) {
				if (floodFill[row][col + 1] == 1) {
					return 1;
				}
			}
		}
		break;
	}
	return 0;
}
tState *successorState(unsigned op, tState *s) {
	tState *result;
	result = (tState*) malloc(sizeof(tState));
	result->array = (char**) malloc(s->rows * sizeof(char*));
	int i;
	for (i = 0; i < s->rows; i++) {
		result->array[i] = (char*) malloc(s->cols * sizeof(char));
	}
	memcpy(result->array,s->array,sizeof(char)*s->rows*s->cols);
	int row, col, operator;
	row = op / (COLS * 4);
	col = (op - row * 4 * COLS) / 4;
	operator = op % 4;
	switch (operator) {
	case 0:
		if (s->array[row][col] == '*') {
			result->array[row][col] = '.';
			if (s->array[row + 1][col] == '.') {
				result->array[row + 1][col] = '*';
			} else {
				result->array[row + 1][col] = '$';
			}
		} else {
			result->array[row][col] = ' ';
			if (s->array[row + 1][col] == '.') {
				result->array[row + 1][col] = '*';
			} else {
				result->array[row + 1][col] = '$';
			}
		}
		break;
	case 1:
		if (s->array[row][col] == '*') {
			result->array[row][col] = '.';
			if (s->array[row][col + 1] == '.') {
				result->array[row][col + 1] = '*';
			} else {
				result->array[row][col + 1] = '$';
			}
		} else {
			result->array[row][col] = ' ';
			if (s->array[row][col + 1] == '.') {
				result->array[row][col + 1] = '*';
			} else {
				result->array[row][col + 1] = '$';
			}
		}
		break;
	case 2:
		if (s->array[row][col] == '*') {
			result->array[row][col] = '.';
			if (s->array[row - 1][col] == '.') {
				result->array[row - 1][col] = '*';
			} else {
				result->array[row - 1][col] = '$';
			}
		} else {
			result->array[row][col] = ' ';
			if (s->array[row - 1][col] == '.') {
				result->array[row - 1][col] = '*';
			} else {
				result->array[row - 1][col] = '$';
			}
		}
		break;
	case 3:
		if (s->array[row][col] == '*') {
			result->array[row][col] = '.';
			if (s->array[row][col - 1] == '.') {
				result->array[row][col - 1] = '*';
			} else {
				result->array[row][col - 1] = '$';
			}
		} else {
			result->array[row][col] = ' ';
			if (s->array[row][col - 1] == '.') {
				result->array[row][col - 1] = '*';
			} else {
				result->array[row][col - 1] = '$';
			}
		}
		break;
	}
	return result;
}
void freeState(tState *s) {

}

int cost(unsigned op, tState *s) {
	return 1; //Coste de mover una caja
}
int h(tSearchNode *n) {
	return 0;
}
int stateEqual(tState *s1, tState *s2) {
	int i, j;
	if (s1->rows != s2->rows)
		return 0;
	if (s1->cols != s2->cols)
		return 0;
	for (i = 0; i < s1->rows; i++) {
		for (j = 0; j < s1->cols; j++) {
			if (s1->array[i][j] != s2->array[i][j]) {
				return 0;
			}
		}
	}
	return 1;
}
void showOperator(unsigned op) {
	int row, col, operator;
	row = op / (COLS * 4);
	col = (op - row * 4 * COLS) / 4;
	operator = op % 4;
	switch (operator) {
	case 0:
		printf("Fila: %d, Columna: %d,ARRIBA", row, col);
		break;
	case 1:
		printf("Fila: %d, Columna: %d,DERECHA", row, col);
		break;
	case 2:
		printf("Fila: %d, Columna: %d,ABAJO", row, col);
		break;
	case 3:
		printf("Fila: %d, Columna: %d,IZQUIERDA", row, col);
		break;
	}
}
void showState(tState *s) {
	int i, j;
	for (i = 0; i < s->rows; i++) {
		for (j = 0; j < s->cols; j++) {
			printf("%c", s->array[i][j]);
		}
		printf("\n");
	}
}

