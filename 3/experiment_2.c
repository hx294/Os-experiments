#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include<wait.h>
#include <sys/wait.h>


#define READ_END 0
#define WRITE_END 1
#define lenth_h 12
#define MAX_LINE 80

char history[lenth_h][MAX_LINE] = {0};
int his_id;

int split_cmd(char cmd[],char* args[]);
void eval(char cmd[],char* args[]);
int builtin_cmd(char* args[],char []);
extern char** environ; /* 环境变量 */

int main(void){
	
	char *args[MAX_LINE/2 + 1];
	int should_run = 1;
	char cmd[MAX_LINE];

	while(should_run){
		printf("osh>");
		fflush(stdout);
		/*reading user input*/
		if(fgets(cmd,MAX_LINE,stdin) == NULL && ferror(stdin)){
			puts("fgets error");
		}

		/*
		if(feof(stdin)){ // ctrl-d 
			fflush(stdout);
			exit(0);
		}	
		*/
		/* eval the cmd line*/
		eval(cmd,args);
		fflush(stdout);
	//	fflush(stdout);
	}
	return 0;
}
int same_file(int fd1, int fd2) {
    struct stat stat1, stat2;
    if(fstat(fd1, &stat1) < 0) return -1;
    if(fstat(fd2, &stat2) < 0) return -1;
    return (stat1.st_dev == stat2.st_dev) && (stat1.st_ino == stat2.st_ino);
}
void execution(char * args[],char* cmd){
/* check the cmd is built-in or not */
	if(!builtin_cmd(args,cmd))
	{
			if(execvp(args[0],args) == -1){
				perror("execve");
				exit(0);
			}
			puts("never get here");
	}
}


/* return 0 if not call execvp */
void exe_intru(char* args[],char cmd[],int mark){


	pid_t pid = 0;
	pid = fork();
	if(pid < 0){
		perror("fork");
		exit(1);
	}

	if(!strcmp(args[0],"exit")){
		exit(0);
	}

	if(pid == 0){
		int fd[2];
		if(mark){
			char ** args1;
			char * cmd1;

			args1 = args+mark+1;
			for(int i=0; cmd[i]; i++)
			{
				if(cmd[i] == '|'){
					cmd1 = cmd+i+1;
					cmd[i] = '\0';
					break;
				}
			}


			if(pipe(fd) == -1){
				perror("pipe");
				exit(1);
			}
			
			pid = fork();
			//puts("mark");

			if(pid < 0){
				perror("fork");
				exit(1);
			}

			if(pid > 0){
				//printf("%s\n",cmd1);
				close(fd[WRITE_END]);
				dup2(fd[READ_END],STDIN_FILENO);
				execution(args1,cmd1);
				wait(NULL);
				exit(0);
				puts("never get here");
			}else{
				//printf("%s\n",cmd);
				close(fd[READ_END]);
				dup2(fd[WRITE_END],STDOUT_FILENO);
				execution(args,cmd);
				exit(0);
				puts("never get here");
			}
		}
		execution(args,cmd);
		exit(0);
	}
}

void eval(char cmd[],char* args[]){
	//puts("eval");
	
	int bg = split_cmd(cmd,args);

	/* check args is wheather NULL or not */
	if(args[0] == NULL){
		return;
	}

	int fd = -1;
	int old_std;
	/* check if "<" | ">" is in args */
	for(int i=1; args[i] ; i++){
		if(!strcmp(args[i],">")){
			fd = open(args[i+1],O_WRONLY|O_RDONLY|O_CREAT|O_TRUNC,0666);
			if(fd < 0){
				perror("open:");
			}
			old_std = dup(STDOUT_FILENO);
			dup2(fd,STDOUT_FILENO)	;
			args[i] = NULL;
			break;
		}else if(!strcmp(args[i],"<")){
			fd = open(args[i+1],O_RDONLY);
			if(fd < 0){
				perror("open:");
			}
			old_std = dup(STDIN_FILENO);
			dup2(fd,STDIN_FILENO);
			args[i] = NULL;
			break;
		}
	}
	/* check if '|' in args */
	int mark = 0;
	char ** args1;
	char *cmd1;
	for(int i=0; args[i];i++ ){
		if(!strcmp(args[i],"|"))
		{
			args1 = args+i+1;
			args[i] = NULL;
			mark = i;
		}
	}
	/*
	int i;
	for(i = 0; cmd[i]; i++){
		if(cmd[i] == '|'){
			cmd1 = cmd + i + 1;
			mark = 1;
			break;
		}
	}
	*/

	/* execution */
	pid_t pid;
	exe_intru(args,cmd,mark);

	/* forehead process should be waited */
	if(!bg){
		while ((pid = waitpid(-1, NULL, 0)) != -1) ;
	//	puts("forehead");
	}else{
		printf("[%d] %s",pid,cmd);
	}
	
		
		/* add cmd to the history */
	int num;
	if(strcmp(args[0],"!!") && strcmp(args[0],"!") && strcmp(history[his_id],cmd))
			strcpy(history[(++his_id)%10],cmd);
	else if(!strcmp(args[0],"!") && (num = atoi(args[1]) < his_id) && strcmp(history[his_id],history[num%10])) 
			strcpy(history[(++his_id)%10],history[num%10]);

	/* close file */
	if(fd >= 0){
		if( same_file(fd,1) )
		{
			dup2(old_std,STDOUT_FILENO);
		}else if( same_file(fd,0) ){
			dup2(old_std,STDIN_FILENO);
		}
		close(old_std);
		close(fd);
	}
	
}

void hist(char cmd[]){
	printf("history");
	for(int i=0; i<10; i++ ){
		if(history[(his_id-i)%10][0]){
			if(i == 0 && !strcmp(history[his_id%10],cmd)) continue;
			printf("\t %d %s",his_id-i,history[(his_id-i)%10]);
		}else{
			if(i == 0){
				puts("");
			}
			break;
		}
	}

}

int builtin_cmd(char* args[],char cmd[]){
	int builtin = 0;
	//printf("%s",args[0]);
	if( !strcmp(args[0],"history") ){
		//puts("history");
		hist(cmd);
		builtin = 1;
	}else if(!strcmp(args[0],"!!")){
		if(!history[(his_id)%10][0]){
			puts("No commands in history");
		}else {
			eval(history[(his_id)%10],args);
		}
		builtin = 1;
	}else if(!strcmp(args[0],"!")){
		int num = 0;
		//printf("args: %s\n",args[1]);
		if(args[1] && (num = atoi(args[1])) && (num > his_id - 10  && num <= his_id)){
			eval(history[num % 10],args);
		}else{
			printf("No such command in history,the num: %d",num);
		}
		builtin = 1;
	}else if(!strcmp(args[0],"exit")){
		exit(0);
		builtin = 1;/* never get here */
	}else{
		return builtin;
	}	

	return builtin;
}

int split_cmd(char cmd[],char* args[]){

	static char array[MAX_LINE]; // holds local copy of cmd 
	char* buf = array;
	strcpy(array,cmd);
	char* delim;
	int argc;
	int bg;

	//puts("split_cmd");

	argc = 0;
	buf[strlen(buf) -1] = ' ';// replace the '\n' with '
	//printf("buf: %s\n",buf)	;					  
	while(*buf && (*buf) == ' '){ /* ignore space */
		buf ++;	
	}	

	/* args maybe quotes with '' */
	if(*buf == '\''){
		buf++;
		delim = strchr(buf,'\'');
	}else{
		delim = strchr(buf,' ');
	}

	while(delim){
		args[argc++] = buf;
		*delim = '\0'; /* 这个很关键，这样才能把字符串割裂 */
		buf = delim + 1;
		while(*buf && (*buf) == ' '){
			buf++;
		}
		if(*buf == '\''){
			buf ++;
			delim = strchr(buf,'\'');
		}else{
			delim = strchr(buf,' ');
		}
	}

	args[argc] = NULL;

	if(argc == 0){
		return 1; /* ignore the blank line */
	}

	/*
	for(int i=0; args[i]; i++){
		printf("%s\n",args[i]);
	}
	*/
	
	if((bg = (*args[argc-1] == '&'))){
		args[--argc] = NULL;
	}
	
	return bg;
}
