#include <stdio.h>

#define LRU_MAX 4


// This method checks if value is in vector and return its position
// If its not in, return -1;
int getPosition (int vector[], int size, int value) {
    for (int i=0; i<size; i++) {
        if (vector[i] == value) return i;
    }
    return -1;
}


// Swap value at given adresses
void swap (int* adress1, int* adress2) {
    int temp = *adress1;
    *adress1 = *adress2;
    *adress2 = temp;
}


// This method carry the value in 'position' to the last position in vector
// Works pretty like bubble sort, in where a element "float" till top
void raiseValue(int vector[], int size, int position) {
    for (int i=position; i<size-1; i++) {
        swap(&vector[i], &vector[i+1]);
    }
}


// Add the given value at vector's end and shift every value to the previous position
void addInLRU(int vector[], int size, int value) {
    raiseValue(vector, size, 0);
    vector[size-1] = value;
}


// This method calls the value at LRU Table
int callLRUTable (int lru[], int value) {
    int position = getPosition(lru, LRU_MAX, value);
    if (position != -1) {
        raiseValue(lru, LRU_MAX, position);
        return 1;
    }
    addInLRU(lru, LRU_MAX, value);
    return 0;
}


void printVector (int vector[], int size) {
    for (int i=0; i<size; i++) {
        printf("%d ", vector[i]);
    }
    printf("\n-------\n");
}


// TEST MAIN, THE FUNCTIONS MAY BE COPIED TO ANOTHER FILE
int main () {
    int lru[LRU_MAX] = {0};
    int call;

    printf("\n\
    This is a Last Recently Used (LRU) strucure\n\
    Call a number and it will be rised till the end of vector\n\
    The LRU value will be excluded\n\
    Call 0 to exit the program\n\n");

    printf("Call: ");
    scanf("%d", &call);
    while (call != 0) {
        int wasInLRU = callLRUTable(lru, call);
        if (wasInLRU) {
            printf("HIT!\n");
        } else {
            printf("FAULT!\n");
        }
        printVector(lru, LRU_MAX);
        printf("Call: ");
        scanf("%d", &call);
    }
    return 0;
}
