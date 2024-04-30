#include<stdio.h>
#include<sys/types.h>
#include<wait.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>

#define MIN_PID 300
#define MAX_PID 5000

#define char_size 8
char *bitmap; // 进程号是否空闲

int allocate_map(){
	bitmap = (char*)malloc((MAX_PID-MIN_PID)/char_size*char_size);
	if(bitmap){
		memset(bitmap,0,(MAX_PID-MIN_PID)/char_size*char_size);
		return 1;
	}
	return 0;
}

/*
int getArandom(){
	srand((unsigned)time(NULL));
	int ran = rand();
	return ran >= 300 ? ran%5000 : ran+300;
}
*/

int allocate_pid(){
	int i;
	int mark = 0;
	for(i=0; i <= MAX_PID-MIN_PID;i++){
		int index = i/8;// 定位字节
		int offset = i%8; // 定位偏移
		int dest = 1 << offset;
		if( !(bitmap[index] & dest)){
			bitmap[index] |= dest;
			mark = 1;
			break;
		}
	}
	if(mark == 0) return 1;
	else return i + MIN_PID;
}

void release_pid(int pid){
	pid -= 300;
	int index = pid/8;
	int offset = pid%8;
	char dest = 1<< offset;
	bitmap[index] &= (~dest);
}



int main(){
	int mark = allocate_map();
	if(!mark){
		puts("allocate_map failed");
	}
	int pid = allocate_pid();
	if(pid == 1){
		puts("no extra process space");
	}else{
		printf("allocate_pid:%d\n",pid);
	}
	/*
	printf("allocate_pid:%d\n",pid);
	pid = allocate_pid();
	printf("allocate_pid:%d\n",pid);
	*/
	
	release_pid(pid);
	printf("remove process :%d\n",pid);

	free(bitmap);
	return 0;
}

