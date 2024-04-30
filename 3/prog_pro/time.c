#include<stdio.h>
#include<sys/types.h>
#include<wait.h>
#include <sys/time.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include <unistd.h>

int main(int argc,char* argv[]){
	pid_t pid;
	pid = fork();
	if(pid < 0){
		puts("failed to fork");
	}else if( pid == 0 ){ /* child process */
		const int SIZE = 4096;
		const char *name = "OS";
		int shm_fd;
		long int *ptr;
		struct timeval tv;
		int mark = gettimeofday(&tv,NULL);
		if(mark){
			puts("failed to gettimeofday");
		}
		// write to shared-memory
		// open shared memeory object
		shm_fd = shm_open(name,O_CREAT | O_RDWR,0666); /* without execution right */
		/* configure the size of the shared memory */
		ftruncate(shm_fd,SIZE);
		// memory map
		ptr = (long int*)mmap(0,SIZE,PROT_READ | PROT_WRITE,MAP_SHARED,shm_fd,0);
		if(ptr == MAP_FAILED) {
			perror("mmap");
			exit(1);
    	}
		/* write to sh-me */
		//printf("sec:%ld\n",tv.tv_sec);
		*ptr = tv.tv_sec;
		//printf("hello\n");
		*(ptr + 1) = tv.tv_usec;
		printf("%ld\n",*ptr);
		//sprintf(ptr,"%ld",tv.tv_usec);
		if(execlp(argv[1],"",NULL)){
			puts("failed to execlp");
		}
		exit(0);
	}else { /* parent process */
		wait(NULL);
		/* get time of now */
		struct timeval tv;
		int mark = gettimeofday(&tv,NULL);
		if(mark){
			puts("failed to gettimeofday");
		}

		/* get the time from child */
		const int SIZE = 4096;
		const char *name = "OS";
		int shm_fd;
		long int*ptr;
		
		shm_fd = shm_open(name,O_RDONLY,0666);
		//printf("%d\n",shm_fd);
		ptr = (long int *)mmap(0,SIZE,PROT_READ,MAP_SHARED,shm_fd,0);
		if (ptr == MAP_FAILED) {
			perror("mmap");
			exit(1);
		}
		long int old_sec,old_usec;
		//printf("aaaa\n");
		old_sec = *ptr;
		//printf("aaaa\n");
		old_usec = *(ptr+1);
		printf("\nElapsed time: %lf",(double)tv.tv_sec-old_sec+(double)(tv.tv_usec-old_usec)/1000000);
shm_unlink(name); }
	return 0;
}
