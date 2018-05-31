#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

int cels[9];
int row[9];
int col[9];
int sudoku[9][9];

void debug(){ // remove this function
	int i,j; 
	for(i=0; i<9; ++i){
		for(j=0; j<9; ++j)
			printf("[%2d]", sudoku[i][j]);
		printf("\n");
	}
}


void *sub_grade(void * input){
	/// Evaluate a 3x3 subgrade
	
	input = (int*)input;
	a = input[0];
	b = input[1];
	
	int conf[9];
	int result = 1, i, j;

	memset(conf, 0, sizeof(conf));
	

	for(i=a-1; i<=a+1; ++i){
		for(j=b-1; j<=b+1; ++j){
			conf[sudoku[i][j]-1]=1;
		}
	}

	for(i=0; i<9; ++i)
		result = result && conf[i];	

	return (void*) result;
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

	int i,j;
	
	for(i=0; i<9; ++i)
		for(j=0; j<9; ++j)
			scanf("%d", &sudoku[i][j]);

	int bool_cols=1, bool_rows=1, bool_cels=1;

	pthread_t grades[9];

	for(i=1; i<9; i+=3){
		for(j=1; j<9; j+=3){
			int vet = {i,j};
			pthread_create(&grades[i], NULL, sub_grade, arg);
			bool_cels = bool_cels && sub_grade(i,j);
		}
	}

	for(i=0; i<9; ++i){
		bool_rows = bool_rows && confirm_row(i);
		bool_cols = bool_cols && confirm_col(i);
	}

	int result = bool_cels && bool_cols && bool_rows;

	if(result)
		printf("YES");
	else
		printf("NO");

	printf("\n");

 return 0;
}
