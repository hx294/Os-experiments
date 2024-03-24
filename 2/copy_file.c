#include<stdio.h>

#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#define lenth 1000

char input[lenth],output[lenth];
char screen[lenth];

int main(){
	printf("please input the source file:\n");
	scanf("%s",input);
	printf("please input the target file:\n");
	scanf("%s",output);
	int fd1 = open(input,O_RDONLY);
	if(fd1 == -1){
		puts("source file not exists");
		exit(0);
	}
	int fd2 = open(output, O_WRONLY|O_EXCL|O_CREAT ,0644);
	//printf("%d",fd2);
	if(fd2 == -1){
		puts("target file already exits");
		puts("input 1 to abort the program or input 2 to replace the existing file");
		while(1){
			int choice;
			scanf("%d",&choice);
			if(choice == 1){
				close(fd1);
				exit(0);
			}else if(choice == 2){
				fd2 = open(output,O_WRONLY|O_CREAT|O_TRUNC,0644);
				//printf("%d\n",fd2);
				break;
			}else{
				puts("invalid input...please input again");
				continue;
			}
		}
	}

	char x[2];
	int mark = 1;
	while(mark != -1){
		mark = read(fd1,&x,1);
		if(mark == 0){
			puts("the end of file");
			break;
		}else if(mark == -1){
			puts("input wrong");
			close(fd1);
			close(fd2);
			exit(0);
		}
		mark = write(fd2,&x,1);
		if(mark == -1){
			puts("no more disk space ");
			close(fd1);
			close(fd2);
			exit(0);
		}
	}
	close(fd2);

	mark = lseek(fd1, 0, SEEK_SET);
	if(mark == -1){
		puts("Error seeking file");
		exit(0);
	}
	
	mark = read(fd1,screen,lenth);
	if(mark == -1){
		puts("read to screen wrong");
		close(fd1);
		exit(0);
	}

	puts("the contents of the file:");
	mark = write(1,screen,lenth);
	if(mark == -1){
		puts("write to screen wrong");
		close(fd1);
		exit(0);
	}

	close(fd1);
	return 0;
}
