#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(){
	pid_t pid;
	pid = fork();
	if(pid == 0){
		//sleep(100);// ms
		exit(0);
	}else if(pid > 0){
		// nothing
		sleep(100);
		exit(0);
	}else{
		puts("failed to fork");
	}
}
