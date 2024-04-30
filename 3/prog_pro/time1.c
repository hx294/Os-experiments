#include<stdio.h>
#include<sys/types.h>
#include<wait.h>
#include<sys/time.h>
#include<unistd.h>
#include<stdlib.h>

#define BUFFER_SIZE 25
#define READ_ENd 0
#define WRITER_END 1

int main(int argc,char* argv[]){
	struct time;
	//long int sec,usec;
	int fd[2];
	pid_t pid;
	struct timeval tv;
	if(pipe(fd) == -1){
		fprintf(stderr,"Pipe failed");
	}

	pid = fork();
	if(pid < 0){
		fprintf(stderr,"fork failed");
	}else if(pid == 0){
		int mark = gettimeofday(&tv,NULL);
		if(mark ){
			fprintf(stderr,"gettimeofday failed");
		}
		/* close read end of the pipe */
		close(fd[READ_ENd]);
		/* write to the pipe */
		write(fd[WRITER_END],&tv,sizeof(tv));
		/* close the write end of the pipe */
		close(fd[WRITER_END]);
		if(execlp(argv[1],"",NULL))
			fprintf(stdout,"execlp failed");
		exit(0);

	}else{
		struct timeval tv1;
		wait(NULL);
		int mark = gettimeofday(&tv1,NULL)	;
		if(mark )
			fprintf(stderr,"gettimeofday failed");
		/* close the write end of the piep */
		close(fd[WRITER_END]);

		read(fd[READ_ENd],&tv,sizeof(tv));
		
		close(fd[READ_ENd]);

		fprintf(stdout,"Elapsed time:%f",(double)tv1.tv_sec-tv.tv_sec+(double)(tv1.tv_usec-tv.tv_usec)/1000000);
	}

	return 0;
}
