/*
Problema 2
Integrantes:
    Guilherme Marques Moreira da Silva 16/0029503
    Felipe Borges
    Filipe Toyoshima

*/

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

// Defines
#define STUDENTS 3
#define return_back() sleep(srand() % 5)
#define help_student() sleep(srand() % 5)

// Structs
typedef struct  queue_t{
    int *queue;
    int size;
    int full;
    int head, tail;
}queue_t;

// Functions
void* student(void*);
void* AE(void*);
void AE_help(int);
int check_AE(int);
int check_queue(int);
int in_q(int);
int out_q();


// Global variables
sem_t teacher;
pthread_mutex_t lock;
queue_t queue;

//  MAIN
int main(int argc, char const *argv[]) {
    srand(time(NULL));
    sem_init(&teacher, 0, 1);
    pthread_t students[STUDENTS];
    pthread_mutex_init(&lock, NULL);
    queue.queue = (int*) malloc((STUDENTS/2)*sizeof(int));

    int i = 0;
    // Creating threads
    for(i=0; i < STUDENTS; ++i) {
        pthread_create(&students[i], NULL, student, (void *)i);
    }

    // Join threads
    for (size_t i = 0; i < STUDENTS; i++) {
        pthread_join(students[i], NULL);
    }


    sem_destroy(&teacher);
    pthread_mutex_destroy(&lock);
    return 0;
}

// Functions implementantion

void* student(void* data){
    int help = 0;
    int code = (int) data;
    printf("Estudante %d\n", code);
    do {
        if(!check_AE(code)){
            help++;
        }
        else if(!check_queue(code)){

        } else {
            sleep()
        }

    } while(help < 3);
}

void* AE(void* data) {}

int check_AE(int code){
    if(sem_trywait(&teacher)) {
        AE_help(code);
        return 1;
    }
    return 0;
}

void AE_help(int student){
    printf("AE ajudando o estudante %d.\n", student+1);
    help_student();
}

int check_queue(int){

}

int in_q(int){}

int out_q(){}
