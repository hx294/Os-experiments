#include<stdio.h>
#include<sys/types.h>
#include<wait.h>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/mman.h>
#include<sys/shm.h>
#include<sys/stat.h>

int main(int argc ,char* argv[]){
	if(argv[1] == NULL){
		puts("Please execute 3_22 like this :\n ./3_22 num");
		return 1;
	}
	int num = atoi(argv[1]);
	while(num <= 0){
		puts("Please enter a positive number");
		scanf("%d",&num);
	}
	const int SIZE = 4096;
	const char *name = "OS";
	int shm_fd;
	int* ptr;
	
	/* open shared memory object */
	shm_fd = shm_open(name,O_CREAT | O_RDWR,0666);

	/* configure the size */
	ftruncate(shm_fd,SIZE);

	/* memory map */
	ptr = (int *)mmap(0,SIZE,PROT_READ | PROT_WRITE,MAP_SHARED,shm_fd,0);
	if( ptr == MAP_FAILED ){
		perror("mmap");
		return 1;
	}

	pid_t pid = fork();

	if(pid < 0){
		perror("fork:");
	}
	else if( !pid  ){
		/*child */
		/* write to sh-me */
		while(num != 1)
		{
			*ptr = num;
			if(num%2){
				num = 3*num + 1;
			}else{
				num = num/2;
			}
			ptr ++;
		}
		*ptr = num;
	}
	else{/* parent */

		wait(NULL);
		puts("the sequence is:\t");
		while(*ptr != 1){
			printf("%d ",*ptr);
			ptr ++;
		}
		printf("%d\n",*ptr);
		shm_unlink(name);
	}

	return 0;
}
