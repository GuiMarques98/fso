#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

int cels[9];
int row[9];
int col[9];
int sudoku[9][9];

pthread_mutex_t mutex;

typedef struct MessageSubGrade{
	int x_axis;
	int y_axis;
	int* boolean_result;
}MessageSubGrade;

void debug(){ // remove this function
	int i,j; 
	for(i=0; i<9; ++i){
		for(j=0; j<9; ++j)
			printf("[%2d]", sudoku[i][j]);
		printf("\n");
	}
}


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
	 *inputMessage->boolean_result = boolean_result;
	pthread_mutex_unlock(&mutex);


}

int confirm_col(int index){

	int conf[9];
	int i;
	int result=1;

	memset(conf, 0, sizeof(conf));

	for(i=0; i<9; ++i)
		conf[sudoku[i][index]-1]=1;

	for(i=0; i<9; ++i)
		result = result && conf[i];

	return result;
}
int confirm_row(int index){
	int conf[9];
	int i;
	int result=1;

	memset(conf, 0, sizeof(conf));

	for(i=0; i<9; ++i)
		conf[sudoku[index][i]-1]=1;

	for(i=0; i<9; ++i){
		result = result && conf[i];
	}


	return result;
}

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
	
	for(i=1; i<9; i+=3){
		for(j=1; j<9; j+=3){
			MessageSubGrade message;
			message.x_axis = i;
			message.y_axis = j;
			message.boolean_result = &bool_cels;
			
			pthread_create(&grades[i], NULL, sub_grade, (void*)&message);
		}
	}

	for(i=0; i<9; ++i){
		bool_rows = bool_rows && confirm_row(i);
		bool_cols = bool_cols && confirm_col(i);
	}

	int result = bool_cels && bool_cols && bool_rows;

	printf("[%d]\n", bool_cels);
	if(result)
		printf("YES");
	else
		printf("NO");

	printf("\n");

	pthread_mutex_destroy(&mutex);
 return 0;
}
