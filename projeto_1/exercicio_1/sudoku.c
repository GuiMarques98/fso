#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

// ======== GLOBAL =======

int cels[9];
int row[9];
int col[9];
int sudoku[9][9];
pthread_mutex_t mutex;

// ======== SIGNATURES ====

void* sub_grade(void *);
void* confirm_row(void*);
void* confirm_col(void*);

// ======== STRUCTS ======= 

typedef struct MessageSubGrade{
	int x_axis;
	int y_axis;
	int* boolean_result;
}MessageSubGrade;

typedef struct MessageLine{
	int line;
	int * boolean_result;
}MessageLine;


int main(){
	
	memset(cels, 0, sizeof(cels));
	memset(row, 0, sizeof(row));
	memset(col, 0, sizeof(col));
	pthread_mutex_init(&mutex, NULL);

	int i,j;
	
	for(i=0; i<9; ++i)
		for(j=0; j<9; ++j)
			scanf("%d", &sudoku[i][j]);

	int bool_cols=1, bool_rows=1, bool_cels=1;


	pthread_t grades[9];
	int thread_cont=0;
	for(i=1; i<9; i+=3){
		for(j=1; j<9; j+=3){
			MessageSubGrade message;
			message.x_axis = i;
			message.y_axis = j;
			message.boolean_result = &bool_cels;
			
			pthread_create(&grades[thread_cont++], NULL, sub_grade, (void*)&message);
		}
	}
	

	for(i=0; i<9; ++i)
			pthread_join(grades[i], NULL);

	
	for(i=0; i<9; ++i){
		MessageLine message;

		message.line = i;
		message.boolean_result = &bool_cels;
		pthread_create(&grades[i], NULL, confirm_row,(void*)&message);
	}

	for(i=0; i<9; ++i)
			pthread_join(grades[i], NULL);

	for(i=0; i<9; ++i){
		MessageLine message;
		message.line = i;
		message.boolean_result = &bool_cels;
		pthread_create(&grades[i], NULL, confirm_col,(void*)&message);
	}

	for(i=0; i<9; ++i)
			pthread_join(grades[i], NULL);


	int result = bool_cels && bool_cols && bool_rows;

	if(result)
		printf("YES");
	else
		printf("NO");

	printf("\n");

	pthread_mutex_destroy(&mutex);
 return 0;
}

// ======= FUNCTIONS =========
void* sub_grade(void* message){
	
	MessageSubGrade* inputMessage;

	inputMessage = (MessageSubGrade*) message;

	int map[9];
	int boolean_result = 1;
	int	x_axis, y_axis;

	memset(map, 0, sizeof(map));

	for(x_axis = inputMessage->x_axis-1; x_axis <= inputMessage->x_axis+1; ++x_axis){
		for(y_axis = inputMessage->y_axis-1; y_axis <= inputMessage->y_axis+1; ++y_axis){
			map[sudoku[x_axis][y_axis]-1]=1;
		}
	}

	for(x_axis=0; x_axis<9; ++x_axis){
		boolean_result = boolean_result && map[x_axis];
	}

	pthread_mutex_lock(&mutex);
	 *inputMessage->boolean_result = *inputMessage->boolean_result && boolean_result;
	pthread_mutex_unlock(&mutex);


}

void* confirm_col(void *message){

	MessageLine *inputMessage;
	inputMessage = (MessageLine*)message;

	int conf[9];
	int i;
	int result=1;

	memset(conf, 0, sizeof(conf));

	for(i=0; i<9; ++i)
		conf[sudoku[i][inputMessage->line]-1]=1;

	for(i=0; i<9; ++i)
		result = result && conf[i];

	pthread_mutex_lock(&mutex);
	*inputMessage->boolean_result = *inputMessage->boolean_result && result;
	pthread_mutex_unlock(&mutex);
}

void* confirm_row(void* message){

	MessageLine *inputMessage;
	inputMessage = (MessageLine*)message;

	int conf[9];
	int i;
	int result=1;

	memset(conf, 0, sizeof(conf));

	for(i=0; i<9; ++i)
		conf[sudoku[inputMessage->line][i]-1]=1;

	for(i=0; i<9; ++i){
		result = result && conf[i];
	}

	pthread_mutex_lock(&mutex);
	*inputMessage->boolean_result = *inputMessage->boolean_result && result;
	pthread_mutex_unlock(&mutex);

}


