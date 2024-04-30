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

#define READ_END 0
#define WRITE_END 1

int main(int argc,char * argv[]){

	if(argc != 3){
		puts("Usage: ./3_27 sourc_file_path destination_file_path");
		return 1;
	}

	int fd[2];
	if(pipe(fd) == -1){
		perror("pipe");
		return 1;
	}

	
	pid_t pid = fork();

	if(pid < 0){
		perror("fork");
		return 1;
	}
	else if(pid > 0){
		/* parent */
		close(fd[READ_END]);
		struct stat st;
		int fd_src = open(argv[1],O_RDONLY);
		
		if(fd_src == -1){
			perror("src open");
			return 1;
		}

		fstat(fd_src,&st);

		char* buf = (char*)malloc(st.st_size);
		read(fd_src,buf,st.st_size);

		/* firstly write the size */
		write(fd[WRITE_END],&st.st_size,sizeof(st.st_size));

		/* secondly write the data */
		write(fd[WRITE_END],buf,st.st_size);

		free(buf);
		printf("read from %s successfully!!\n",argv[1]);
		close(fd[WRITE_END]);
	}else{
		/*child */
		close(fd[WRITE_END]);
		int fd_des = open(argv[2],O_CREAT | O_RDWR ,0666);
		if(fd_des == -1){
			perror("dest open");
			return 1;
		}
		
		long int size;

		/* firstly read the size */
		read(fd[READ_END],&size,sizeof(size));

		char* buf = (char*)malloc(size);

		/* secondly read the data */
		read(fd[READ_END],buf,size);
		
		write(fd_des,buf,size)	;

		free(buf);
		printf("write to %s successfully!!\n",argv[2]);
		close(fd[READ_END]);
	}

}
