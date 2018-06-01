/*
Problema 2
Integrantes:
    Guilherme Marques Moreira da Silva 16/0029503
    Felipe Borges
    Filipe Toyoshima Silva 16/0049971

*/

// Includes
#include <stdio.h>
#include<unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

// Defines
#define STUDENTS 6
#define QUEUE_MAX (STUDENTS)/2
#define return_back() usleep(rand() % 5)
#define help_student() usleep(rand() % 5)

// Structs
typedef struct c{
    pthread_cond_t cond;
    pthread_mutex_t block;
} chair_t;

typedef struct f {
    int chair[QUEUE_MAX];
    chair_t chair_action[QUEUE_MAX];
    int start;
    int number_of_elements;
} queue_t;

// Functions
void* student(void*);
void* AE(void*);
void AE_help(int);
int check_AE(int);
int init_queue();
int check_queue(int);
int is_queue_available();
int is_queue_empty();
int add_in_queue(int);
int remove_from_queue();
int get_queue_end();
void destroy_queue();


// Global variables
int helped_students = 0, CONTADOR_GLOBAL = 0;
sem_t teacher;
pthread_mutex_t lock;
queue_t queue;

//  MAIN
int main(int argc, char const *argv[]) {

    // Inicializations
    srand(time(NULL));
    sem_init(&teacher, 0, 1);
    pthread_t students[STUDENTS];
    pthread_t ae_helper;
    pthread_mutex_init(&lock, NULL);
    init_queue();
    printf("%d - Começa pelo menos\n", CONTADOR_GLOBAL);
    CONTADOR_GLOBAL++;

    int i = 0;
    // Creating threads
    pthread_create(&ae_helper, NULL, AE, NULL);
    for(i=0; i < STUDENTS; ++i) {
        pthread_create(&students[i], NULL, student, (void *)i);
    }

    // Join threads
    pthread_join(ae_helper, NULL);
    for (size_t i = 0; i < STUDENTS; i++) {
        pthread_join(students[i], NULL);
    }
    printf("Terminou TUDO \n");
    // Ending
    destroy_queue();
    sem_destroy(&teacher);
    pthread_mutex_destroy(&lock);
    return 0;
}

// Functions implementantion

void* student(void* data){
    int help = 0, pos_queue;
    int code = (int) data;
    do {
        if(check_AE(code) && is_queue_empty()){
            AE_help(code);
            help++;
        }
        // Wait in queue until this thread is not call
        else if((pos_queue = add_in_queue(code)) >= 0){
            pthread_mutex_lock(&queue.chair_action[pos_queue].block);
            pthread_cond_wait(&queue.chair_action[pos_queue].cond, &queue.chair_action[pos_queue].block);
            pthread_mutex_unlock(&queue.chair_action[pos_queue].block);
        } else {
            return_back();
        }
    } while(help < 3);
}

void* AE(void* data) {
    int sem_status = 0, chais_block = 0, code = 0;
    do {
        pthread_mutex_lock(&lock);
        sem_getvalue(&teacher, &sem_status);
        if(sem_status && !is_queue_empty()) {
            chais_block = queue.start;
            code = remove_from_queue();
            AE_help(code);
            pthread_cond_signal(&queue.chair_action[chais_block].cond);
        }
        pthread_mutex_unlock(&lock);
    } while(helped_students < (STUDENTS * 3));
}

int check_AE(int code){
    pthread_mutex_lock(&lock);
    int teach_available = 0;
    sem_getvalue(&teacher, &teach_available);
    if(teach_available) {
        pthread_mutex_unlock(&lock);
        return 1;
    }
    pthread_mutex_unlock(&lock);
    return 0;
}

void AE_help(int student){
    sem_wait(&teacher);
    printf("%d - AE ajudando o estudante %d.\n", CONTADOR_GLOBAL, student+1);
    CONTADOR_GLOBAL++;
    helped_students++;
    help_student();
    sem_post(&teacher);
}


int is_queue_available () {
    if (queue.number_of_elements >= QUEUE_MAX) {
        // printf("Full queue, cannot add one more element\n");
        return 0;
    }
    // printf("Queue ready to use!\n");
    return 1;
}

int init_queue () {
    for (size_t i = 0; i < QUEUE_MAX; i++) {
        pthread_cond_init(&queue.chair_action[i].cond, NULL);
        pthread_mutex_init(&queue.chair_action[i].block, NULL);
    }
    queue.start = 0;
    queue.number_of_elements = 0;
    return 0;
}

int add_in_queue (int code) {
    /*Push code into Queue end
      Return inserted position if success
      Return -1 if fails*/
    pthread_mutex_lock(&lock);
    if (is_queue_available()) {
        queue.chair[get_queue_end()] = code;
        queue.number_of_elements++;
        // printf("Queue %dº position now holds %d code\n", get_queue_end(), queue.chair[get_queue_end()]+1);
        printf("%d - Estudante %d está na fila.\n", CONTADOR_GLOBAL, code+1);
        CONTADOR_GLOBAL++;
        int end = get_queue_end();
        pthread_mutex_unlock(&lock);
        return end;
    }
    pthread_mutex_unlock(&lock);
    return -1;
}

int is_queue_empty () {
    if (queue.number_of_elements < 1) {
        return 1;
    }
    return 0;
}

int remove_from_queue (){
    /*Move the queue start forward
      Return removed code if success
      Return -1 if fails*/
    if (!is_queue_empty()) {
        int code = queue.chair[queue.start];
        queue.start = (queue.start + 1) % QUEUE_MAX;
        queue.number_of_elements--;
        return code;
    }
    return -1;
}

int get_queue_end() {
    return (queue.start + queue.number_of_elements) % QUEUE_MAX;
}

void destroy_queue(){
    for (size_t i = 0; i < QUEUE_MAX; i++) {
        pthread_cond_destroy(&queue.chair_action[i].cond);
        pthread_mutex_destroy(&queue.chair_action[i].block);
    }
}
