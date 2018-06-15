#include <stdio.h>

#define TLB_MAX 4

/*
FUNNY STORY:
When I was debuggin this code, something weird has come:
The getPosition() method return -1. Originally, this was to mean
False to the if where the method was called (which does not exist
anymore). Now I know it dont work, but surprise, IT WORKED MOTHERF*CKER!
Strangely, the searched valued was saved at vector[-1]. I still dunno
why. So, when the raiseValue() was called, it raises the -1º position,
from which the value would be raised utill the vector final.

The bug even let the code less complex. Notice that the function that
should adds in vector isn't even called.
*/


// This method checks if value is in vector and return its position
// If its not in, return -1;
int getPosition (int vector[], int size, int value) {
    for (int i=0; i<size; i++) {
        if (vector[i] == value) return i;
    }
    return -1;
}


void printVector (int vector[], int size) {
    for (int i=0; i<size; i++) {
        printf("%d ", vector[i]);
    }
    printf("\n-------\n");
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
        printVector(vector, TLB_MAX);
    }
}


// // Add the given value at vector's end and shift every value to the previous position
// void addInLRU(int vector[], int size, int value) {
//     raiseValue(vector, size, 0);
//     vector[size-1] = value;
// }


// This method calls the TLB
int callTLB (int tlb[], int value) {
    int position = getPosition(tlb, TLB_MAX, value);
    raiseValue(tlb, TLB_MAX, position);
    return 0;
}


// TEST MAIN, THE FUNCTIONS MAY BE COPIED TO ANOTHER FILE
int main () {
    int tlb[TLB_MAX] = {0};
    int call;

    printf("\n\
    This is a Last Recently Used (LRU) strucure\n\
    Call a number and it will be rised till the end of vector\n\
    The LRU value will be excluded\n\
    Call 0 to exit the program\n\n");

    printf("Call: ");
    scanf("%d", &call);
    while (call != 0) {
        callTLB(tlb, call);
        //printVector(tlb, TLB_MAX);
        printf("Call: ");
        scanf("%d", &call);
    }
    return 0;
}
