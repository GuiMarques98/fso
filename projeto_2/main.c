#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE (2<<8)
#define PAGE_SIZE (2<<8)
#define TLB_SIZE  (2<<8)
#define FRAME_SIZE (2<<8)
#define FRAMES (2<<8)
#define MEMORY (2<<8)
#define NOT_FOUND -1

// Data Estructures

struct Page{
	int pageNumber;
	char value;
};

struct Frame{
	char value;
};

struct PageTable{
	struct Page pages[PAGE_SIZE];
};

struct FrameTable{
	struct Frame frames[FRAME_SIZE];
};

struct Message{
	unsigned short frame;
	unsigned short offset;
};



// FUNCTIONS
int searchInPageTable(int key);
void viewBinary(unsigned short);
unsigned int convertString(char*);
struct Message convertToMessage(unsigned short);
void clearMemory(void);
void clearPageTable(void);
int getEmptyPhisicalMemory(void);


// === GLOBAL === 

struct PageTable pageTable;
struct FrameTable memory;

int main(int argc, char *argv[]){

	
	FILE *file;
	char *string = NULL;
	size_t len = 0;
	size_t taked;

	file = fopen(argv[1], "r");

	while(getdelim(&string, &len, ' ', file)!=EOF){
		
		struct Message input;
		unsigned int inputString;

		printf("%s\n", string);
		inputString = convertString(string);
		printf("%d\n", inputString);	
		input = convertToMessage(inputString);
		viewBinary(inputString);

		if(searchInPageTable(input.frame) == NOT_FOUND){
		
			int local = getEmptyPhisicalMemory();;
			viewBinary(input.frame);
		
			if(local == -1){
				printf("Memory FULL\n");
			}else{
				FILE *backing = fopen("BACKING_STORE.bin", "rb");
				fseek(backing, input.offset*256, SEEK_SET);
				getdelim(&string, &len, ' ', backing);
				printf("%x\n", convertString(string));
			}
		}
		printf("NAO\n");
	}

 return 0;
}

void clearMemory(){
	int i;
	for(i=0; i<MEMORY; ++i){
		memory.frames[i].value = 0;
	}
}

int getEmptyPhisicalMemory(){
	int i;
	for(i=0; i<MEMORY; ++i)
		if(memory.frames[i].value == 0)
			return 0;
	return -1;
}

void clearPageTable(){
	int i;
	for(i=0; i<PAGE_SIZE; ++i)
		pageTable.pages[i].pageNumber = -1;
}

int searchInPageTable(int key){
	int i;

	for(i=0; i<PAGE_SIZE; ++i)
		if(pageTable.pages->pageNumber == key)
			return i;

	return -1;
}

struct Message convertToMessage(unsigned short number){
	struct Message aux;

	aux.frame = (number & 0xFF00)>>8;
	aux.offset = number & 0x00FF;

	return aux;
}

unsigned int convertString(char *s){
	unsigned int number = 0;
	int idx;

	for(idx = 0; s[idx]!= '\0'; ++idx){
		number*= 10;
		number+=s[idx] - '0';
	}

	return number;

}

void viewBinary(unsigned short number){
    int i;

	for(i=15; i>=0; --i){
		if(i==7)
			printf("|");
		printf("%d", (number & (1<<i)? 1 : 0));
	}
	printf("\n");
}
