#include<stdio.h>
#include<sys/types.h>
#include<wait.h>
#include<time.h>
#include <unistd.h>
#include<sys/types.h>
#include<fcntl.h>

int main(){

	int a;
	/* no such file or directory */
	int fd = open("a",O_RDONLY);
	/* bad address */
	int fd2 = open(NULL, O_WRONLY|O_EXCL|O_CREAT ,0644);
	perror("perror");


	return 0;
}
