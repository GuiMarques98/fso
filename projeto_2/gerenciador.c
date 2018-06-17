/*
    Gerenciador de Memória Virtual

Integrantes:
    Guilherme Marques Moreira da Silva 16/0029503
    Felipe Borges 16/0049733
    Filipe Toyoshima Silva 16/0049971

*/
// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DEFINE
#ifndef GERENCIADOR__C
#define GERENCIADOR__C
#define FRAME_SIZE 256
#define PAGE_TABLE_SIZE 256
#define HD_SIZE 256
#define RAM_SIZE 256
#endif

// STRUCTS

typedef struct f {
    char c[FRAME_SIZE];
}frame_t;

typedef struct r {
    frame_t frame[RAM_SIZE];
    char validation[RAM_SIZE];
}ram_t;

// Input from file
typedef struct i_p {
    unsigned short shift;
    unsigned short page;
}in_p_t;

// After pass in page table
typedef struct i_f{
    unsigned short shift;
    unsigned short frame;
}in_f_t;

typedef struct p {
    unsigned short table[PAGE_TABLE_SIZE];
    char validation[PAGE_TABLE_SIZE];
}page_table_t;

// GLOBAL VARIABLES
ram_t ram;
page_table_t page_table;

// FUNCTIONS
in_p_t convert_to_page(int);
in_f_t seek_in_table(in_p_t);
unsigned short put_in_table(unsigned short);


// MAIN
int main(int argc, char const *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "O arquivo precisa ser executado como: \"./a.out address.txt\"\n");
        return -1;
    }

    FILE* read_file;
    read_file = fopen(argv[1], "r");
    if(!read_file) {
        fprintf(stderr, "Erro ao abrir o arquivo\n");
        return -1;
    }

    unsigned int x = 0;
    while(fscanf(read_file, "%d", &x) != EOF){
        printf("ADRESS = %x\n", x);
    }
    return 0;
}

// FUNCTIONS IMPLEMENTATIONS

in_p_t convert_to_page(int adress) {
    /*
    Receives the adress from file.txt
    Returns the struct with the number of the page and the shift
    */
    in_p_t in_page;
    adress = 0x0000FFFF & adress;
    in_page.shift = 0x000000FF & adress;
    in_page.page = (0x0000FF00 & adress) >> 8;
    return in_page;
}

in_f_t seek_in_table(in_p_t in_page) {
    /*
    Seek pages in the page table and if is not put it in the RAM
    Returns the frame
    */
    in_f_t in_frame;
    in_frame.shift = in_page.shift;

    if(ram.validation[page]) {
        in_frame.frame = page_table.table[in_page.page];
    } else {
        in_frame.frame = put_in_table(in_page.page);
    }
    return in_frame;
}
