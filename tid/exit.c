#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


void *thr_start(void *arg)
{
	while(1){
		printf("thread\n");
		sleep(1);
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
	printf("main exit\n");
	pthread_cancel(tid);
	pthread_exit(NULL);
	return 0;
}
