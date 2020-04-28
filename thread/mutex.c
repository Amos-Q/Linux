//以黄牛抢票为例子

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int ticket = 100;
pthread_mutex_t mutex;

void *thr_scalpers(void *arg)
{
	while(1){
		//加锁一定是只保护临界资源的访问
		pthread_mutex_lock(&mutex);
		if(ticket > 0){
			usleep(1000);
			printf("I got ticket:%d\n",ticket);
			ticket--;
			pthread_mutex_unlock(&mutex);
		}
		else{
			//加锁后在任意有可能退出线程的地方都要解锁
			pthread_mutex_unlock(&mutex);
			pthread_exit(NULL);
		}
	}
	return NULL;
}

int main()
{
	pthread_t tid[4];
	int i = 0;
	//互斥锁的初始化要放在线程创建之前
	pthread_mutex_init(&mutex,NULL);
	for(i = 0; i < 4; i++){
		int ret = pthread_create(&tid[i],NULL,thr_scalpers,NULL);
		if(ret != 0){
			printf("thread create error");
			return -1;
		}	
	}
	for(i = 0;i<4;i++){
		pthread_join(tid[i],NULL);
	}
	pthread_mutex_destroy(&mutex);
	return 0;
}
