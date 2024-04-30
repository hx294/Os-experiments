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

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main(){
	int fd[2];
	int fd1[2];

	if(pipe(fd) == -1 || pipe(fd1) == -1){// 一开始写成&& 结果就出不来
		perror("pipe");
		return 1;
	}

	pid_t pid = fork();

	if(pid < 0){
		perror("fork");
		return 1;
	}
	else if( pid == 0 ){
		/* close unused port */
		close(fd[READ_END]);
		close(fd1[WRITE_END]);
		/* read from screan */
		puts("Please enter the string you want:\n");
		char buffer[BUFFER_SIZE];
		fgets(buffer,BUFFER_SIZE,stdin);
		/* write to fd */
		write(fd[WRITE_END],buffer,strlen(buffer)+1);
		close(fd[WRITE_END]);
		/* read from fd1 */
		read(fd1[READ_END],buffer,BUFFER_SIZE);
		close(fd1[READ_END]);
		printf("Reversed string:%s",buffer);	
	}
	else{
		/* close unused port */
		close(fd[WRITE_END]);
		close(fd1[READ_END]);
		char buffer[BUFFER_SIZE];
		/* read from fd */
		read(fd[READ_END],buffer,BUFFER_SIZE);
		close(fd[READ_END]);
		printf("Original string:%s",buffer);
		/* write to pipe */
		int len = strlen(buffer);
		for(int i=0; i<len; i++){
			if(buffer[i] >= 'A' && buffer[i] <= 'Z')
				buffer[i] += 0x20;
			else if(buffer[i] >= 'a' && buffer[i] <= 'z')
				buffer[i] -= 0x20;
			else {
				continue;
			}
		}
		write(fd1[WRITE_END],buffer,strlen(buffer)+1);
		close(fd1[WRITE_END]);
		wait(NULL);
	}
	return 0;
}
