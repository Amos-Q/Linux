#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


void *thr_start(void *arg)
{
	pthread_detach(pthread_self());//pthread_self()返回调用的线程id
	char *ptr = "thread我退出了";//字符串在常量区
	//char ptr[] = "thread我退出了";//ptr有一块空间在栈中，字符串赋值进去,等到线程退出，栈区释放
	while(1){
		printf("thread\n");
		sleep(1);
		pthread_exit((void*)ptr);
	}
	return NULL;
}
int main()
{
	pthread_t tid;
	int ret = pthread_create(&tid,NULL,thr_start,NULL);
	if(ret != 0){
		printf("create thread failed\n");
		return -1;
	}
	pthread_detach(tid);
	//char *ptr;
	//pthread_join(tid,(void**)&ptr);
	//printf("%s\n",ptr);
	printf("main exit\n");
	pthread_exit(NULL);
	return 0;
}
