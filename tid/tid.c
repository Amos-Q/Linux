#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

int a = 100;

void* thr_start(void* arg)
{
	strcpy(arg,"我修改了你");
	a = 200;
	while(1){
		printf("i am thread-%s-%d\n",(char*)arg,a);
		sleep(1);
	}
	return NULL;
}
int main()
{
	pthread_t tid;
	char ptr[] = "chilema";
	//pthread_create(获取线程id，线程属性，线程入口函数，参数)
	int ret = pthread_create(&tid,NULL,thr_start,(void*)ptr);
	if(ret != 0){
		printf("create thread failed\n");
		return -1;
	}
	printf("create thread success!\n");
	while(1){
		printf("hello-%s-%d\n",ptr,a);
		sleep(1);
	}
	return 0;
}
