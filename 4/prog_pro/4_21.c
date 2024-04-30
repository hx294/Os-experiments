#include<stdio.h>
#include<sys/types.h>
#include<wait.h>
#include<time.h>
#include<pthread.h>
#include<stdlib.h>

void * avg(void* argv[]){
	int sum = 0;
	int i = 0;
	while(argv[i] != NULL){
		sum += atoi(argv[i]);
		//printf("sum = %d\n",sum);
		i++;
	}	
	return (void*) (sum/i);
}

void * min(void* argv[]){
	int mi = 1e5 ;
	int i=0;
	while(argv[i] != NULL){
		int x = atoi(argv[i]);
		if(mi > x){
			mi = x;
		}
		i++;
	}
	return (void*) mi;
}

void * max(void* argv[]){
	int ma = -1e5;
	int i=0;
	while(argv[i] != NULL){
		int x = atoi(argv[i]);
		if(ma < x){
			ma = x;
		}
		i++;
	}
	return (void*) ma;
}

int main(int argc,char* argv[]){
	void* ouput;
	pthread_t tid[3];
	void* func[3]={&avg,&min,&max};
	char* format[3]={"The average value is ","The minimum value is ", "The maximum value is "};
	pthread_attr_t attr; /* set of thread attributes */
	
	pthread_attr_init(&attr);

	for(int i=0; i<3; i++)
		pthread_create(&tid[i],&attr,func[i],&argv[1]);


	for(int i=0; i<3; i++)
	{
		pthread_join(tid[i],&ouput);
		printf("%s%d\n",format[i],ouput);
	}
	//printf("aaaaaaa\n");

}
