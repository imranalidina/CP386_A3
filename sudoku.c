#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 9
#define NUM_THREADS 11

typedef struct {
    int row;
    int column;
} parameters;

int sudokuVar[SIZE][SIZE];
int validationResults[NUM_THREADS] = {0};

void *checkRow(void *param) {
    parameters *data = (parameters *) param;
    int row = data->row;
    int controlDigit[SIZE] = {0};
    for (int j = 0; j < SIZE; j++) {
        int num = sudokuVar[row][j];
        if (num < 1 || num > SIZE || controlDigit[num - 1] == 1) {
            validationResults[row] = 0;
            pthread_exit(NULL);
        }
        controlDigit[num - 1] = 1;
    }
    validationResults[row] = 1;
    pthread_exit(NULL);
}

void *checkColumn(void *param) {
    parameters *data = (parameters *) param;
    int col = data->column;
    int controlDigit[SIZE] = {0};
    for (int i = 0; i < SIZE; i++) {
        int num = sudokuVar[i][col];
        if (num < 1 || num > SIZE || controlDigit[num - 1] == 1) {
            validationResults[SIZE] = 0;
            pthread_exit(NULL);
        }
        controlDigit[num - 1] = 1;
    }
    validationResults[SIZE] = 1;
    pthread_exit(NULL);
}

void *checkSubgrid(void *param) {
    parameters *data = (parameters *) param;
    int rowStart = data->row;
    int colStart = data->column;
    int controlDigit[SIZE] = {0};
    for (int i = rowStart; i < rowStart + 3; i++) {
        for (int j = colStart; j < colStart + 3; j++) {
            int num = sudokuVar[i][j];
            if (num < 1 || num > SIZE || controlDigit[num - 1] == 1) {
                validationResults[SIZE + 1 + (rowStart / 3) * 3 + (colStart / 3)] = 0;
                pthread_exit(NULL);
            }
            controlDigit[num - 1] = 1;
        }
    }
    validationResults[SIZE + 1 + (rowStart / 3) * 3 + (colStart / 3)] = 1;
    pthread_exit(NULL);
}

int main() {
    FILE *file = fopen("sample_in_sudoku.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (fscanf(file, "%d", &sudokuVar[i][j]) != 1) {
                printf("Error reading sudoku puzzle\n");
                fclose(file);
                return 1;
            }
        }
    }
    fclose(file);

    printf("Sudoku Puzzle Solution is:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", sudokuVar[i][j]);
        }
        printf("\n");
    }

    pthread_t threads[NUM_THREADS];
    int threadIndex = 0;

    for (int i = 0; i < SIZE; i++) {
        parameters *dataForRow = malloc(sizeof(parameters));
        dataForRow->row = i;
        dataForRow->column = 0;
        pthread_create(&threads[threadIndex++], NULL, checkRow, dataForRow);
    }

    parameters *dataForCol = malloc(sizeof(parameters));
    dataForCol->row = 0;
    dataForCol->column = 0;
    pthread_create(&threads[threadIndex++], NULL, checkColumn, dataForCol);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            parameters *dataForSubgrid = malloc(sizeof(parameters));
            dataForSubgrid->row = i * 3;
            dataForSubgrid->column = j * 3;
            pthread_create(&threads[threadIndex++], NULL, checkSubgrid, dataForSubgrid);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        if (validationResults[i] == 0) {
            printf("Sudoku puzzle is invalid\n");
            return 1;
        }
    }

    printf("Sudoku puzzle is valid\n");
    return 0;
}
