/*
Filipe Toyoshima Silva - 16/0049971
Guilherme Marques Moreira da Silva 16/0029503
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define NUM_THREADS 27

int sudoku[9][9];

void *checkColumn (void *i);
void *checkLine (void *j);
void *checkSubsection (void *i);

void readSudoku (char *filename);



int main (int argc, char *argv[]) {
    printf("initing program...\n");
    pthread_t workers[NUM_THREADS];
    pthread_attr_t attr[NUM_THREADS];
    printf("toma no cu\n");
    printf("toma no cu 2.0\n");

    printf("nome recebido: %s\n", argv[1]);
    readSudoku(argv[1]);
    int *local = (int*) malloc(9*sizeof(int));

    for (int i=0; i<9; i++) {
        local[i] = i;
        pthread_attr_init(&attr[i]);
        pthread_create(&workers[i], &attr[i], checkColumn,(void *) local[i]);
        pthread_create(&workers[i+9], &attr[i+9], checkLine,(void *) local[i]);
        pthread_create(&workers[i+18], &attr[i+18], checkSubsection,(void *) local[i]);
    }

    for (int i=0; i<NUM_THREADS; i++) {
        pthread_join(workers[i], NULL);
    }

    printf("Valid solution!");
    return 0;
}


void readSudoku(char *filename) {
    printf("abrindo arquivo %s", filename);
    FILE * matrix = fopen(filename, "r");
    printf("arquivo aberto");
    if (matrix) {
        for (int i=0; i<9; i++) {
            for (int j=0; j<9; j++) {
                fscanf(matrix, "%d", &sudoku[i][j]);
            }
        }
    }
}


void *checkColumn(void *param) {
    int i = *(int*)param;
    int line[9] = {0};
    for (int j=0; j<9; j++) {
        int number = sudoku[i][j];
        if (line[number] != 0) {
            printf("fail fount at column %d\n", i);
            exit(0);
        }
        line[number] = 1;
    };
    return 0;
}


void *checkLine(void *param) {
    int j = *(int*)param;
    int line[9] = {0};
    for (int i=0; i<9; i++) {
        int number = sudoku[i][j];
        if (line[number] != 0) {
            printf("fail found at line %d\n", j);
            exit(0);
        }
        line[number] = 1;
    };
    return 0;
}

void *checkSubsection(void *param) {
    int number = *(int*)param;
    // number to subsection ->
    // 0 1 2
    // 3 4 5
    // 5 7 8
    int columnStart = (number % 3) * 3;
    int lineStart = (number / 3) * 3;
    int line[9] = {0};
    for (int i=columnStart; i<columnStart+3; i++) {
        for (int j=lineStart; j<lineStart+3; j++) {
            int number = sudoku[i][j];
            if (line[number] != 0) {
                printf("fail found at subsection %d,%d\n", columnStart/3, lineStart/3);
                exit(0);
            }
            line[number] = 1;
        }
    }
}
