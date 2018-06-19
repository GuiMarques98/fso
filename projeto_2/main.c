/*
Integrantes:
    Felipe Borges                      16/0049733
    Filipe Toyoshima Silva             16/0049971
    Guilherme Marques Moreira da Silva 16/0029503
*/
#include <stdio.h>
#include <stdlib.h>

#define RAM_SIZE 256
#define SIZE_SIZE 256
#define FRAME_SIZE 256
#define PAGE_SIZE 256
#define NOT_FOUND -1
#define LRU_MAX 16

// Structs
typedef struct a {
    int frame;
    int offset;
} adress_t;

typedef struct r {
    unsigned char *frame[RAM_SIZE];
    char validation[RAM_SIZE];
} ram_t;

typedef struct p {
    unsigned short page[SIZE_SIZE];
    signed validation[SIZE_SIZE];
}page_table_t;

// VARIAVEL GLOBAL
ram_t ram;
page_table_t page_table;
FILE *hd;
int not_found = 0;
int tlb_hit = 0;
int pt_miss = 0;
int count = 0;

// Functions
int getPosition (int vector[], int size, int value);
void swap (int* adress1, int* adress2);
void raiseValue(int vector[], int size, int position);
void addInLRU(int vector[], int size, int value);
int callLRUTable (int lru[], int value);
void init_ram();
void init_table();
int isRAMFull();
adress_t getAdress (number);
unsigned char* getValueInMemory(adress_t adress);
void printValueInMemory(adress_t adress);
int putInRam(adress_t adress);

// Code
int main (int argc, char *argv[]) {

    FILE *adressFile = fopen(argv[1], "r");
    if (adressFile == NULL) {
        printf("Arquivo adresses não encontrado\n");
    }
    hd = fopen("BACKING_STORE.bin", "rb");
    if (adressFile == NULL) {
        printf("Arquivo HD não encontrado\n");
    }

    // int page_table[256] = {0};
    int adress_int;
    adress_t adress;
    int lru[LRU_MAX] = {-1};

    init_ram();
    init_table();

    while(fscanf(adressFile, "%d", &adress_int) != EOF) {
        // if (isRAMFull()) {
        //     printf("FULL RAM, ABORT PROGRAM\n");
        //     return -1;
        // }
        adress = getAdress(adress_int);
        int wasInTLB = callLRUTable(lru, adress.frame);

        if (wasInTLB) {
            printf("HIT! : ");
            printValueInMemory(adress);
            tlb_hit++;
            printf("PAGE : %x\n", page_table.page[adress.frame]);
        } else if (page_table.validation[adress.frame]) {
            printf("HIT! : ");
            printValueInMemory(adress);
            printf("PAGE : %x\n", page_table.page[adress.frame]);
        } else {
            printf("MISS : ");
            page_table.page[adress.frame] = putInRam(adress);
            page_table.validation[adress.frame] = 1;
            printf("PAGE : %x\n", page_table.page[adress.frame]);
            pt_miss++;
        }

        count++;
    }

    printf ("TLB SUCCESS: %d / %d = %.3lf%%\n", tlb_hit, count, 100 * (double)tlb_hit/count);
    printf ("MISS RATE  : %d / %d = %.3lf%%\n", pt_miss, count, 100 * (double)pt_miss/count);

    fclose(adressFile);
    fclose(hd);
    return 0;
}

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

void init_ram() {
    for (size_t i = 0; i < RAM_SIZE; i++) {
        ram.validation[i] = 0;
    }
}


void init_table() {
    for (size_t i = 0; i < SIZE_SIZE; i++) {
        page_table.validation[i] = 0;
    }
}


int isRAMFull() {
    for (int i=0; i<RAM_SIZE; i++) {
        if (ram.validation[i] == 0) {
            return 0;
        }
    }
    return 1;
}

adress_t getAdress(number) {
    adress_t adress;
    adress.frame = (number & 0xFF00)>>8;
	adress.offset = number & 0x00FF;
    printf("READ : ADRESS frame %x : offset %x\n", adress.frame, adress.offset);
    return adress;
}

unsigned char* getValueInMemory(adress_t adress){
	unsigned char *value = (char*)malloc(sizeof(char)*256);
	fseek(hd, adress.frame*256, SEEK_SET);
	fread(value, 256, 1, hd);
    printf("HD VALUE : %x\n", value[adress.offset]);
	return value;
}

void printValueInMemory(adress_t adress){
    printf("HD VALUE : %x\n", ram.frame[page_table.page[adress.frame]][adress.offset]);
}

int putInRam(adress_t adress) {
    unsigned char *value = getValueInMemory(adress);
    for (size_t i = 0; i < FRAME_SIZE; i++) {
        if(!ram.validation[i]) {
            ram.frame[i] = value;
            ram.validation[i] = 1;
            return (int)i;
        }
    }
}
