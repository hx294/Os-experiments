#include<stdio.h>
#include<sys/types.h>
#include<wait.h>
#include<time.h>
#include<stdlib.h>
#include <unistd.h>
#include<sys/types.h>


int main(int argc, char* argv[]){
	if(argv[1] == NULL){
		puts("please exec the program like this:\n ./3_21 num");
		return 1;
	}
	int num = atoi(argv[1]);
	while(num <= 0){
		puts("\nfailed to run ,please enter a positive number:\n");
		scanf("%d",&num);
	}
	
	pid_t pid = fork();
	if(pid < 0){
		fprintf(stderr,"Fork failed");
		return 1;
	}
	else if( pid > 0 )
	{/* parent process */
		wait(NULL);
	}
	else
	{
		while(num != 1)
		{
			printf("%d ",num);
			if(num%2){
				num = 3*num + 1;
			}else{
				num = num/2;
			}
		}
		printf("1\n");
	}
	return 0;
}
