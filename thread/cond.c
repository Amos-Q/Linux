#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

int bowl = 0;//默认碗中没饭

pthread_cond_t cond_cook; // 实现线程间对bowl变量访问的同步操作
pthread_cond_t cond_customer;
pthread_mutex_t mutex;//保护bowl变量的访问

void *thr_cook(void *arg)
{
	while(1){
		//加锁
		pthread_mutex_lock(&mutex);
		while(bowl != 0){//表示有饭
			//让厨师线程等待,等待之前先解锁，被唤醒之后再加锁
			//pthread_cond_wait 接口中就完成了解锁，休眠，被唤醒后加锁操作 
			//并且解锁和休眠操作时一步完成，保证原子操作
			pthread_cond_wait(&cond_cook,&mutex);
		}
		bowl= 1;//能走下来白哦是没饭，bowl==0，则做饭将bowl修改为1
		printf("I made a bowl\n");
		//解锁
		pthread_mutex_unlock(&mutex);		
		//唤醒顾客吃饭
		pthread_cond_signal(&cond_customer);
	}
	return NULL;
}

void *thr_customer(void *arg)
{
	while(1){
		//加锁
		pthread_mutex_lock(&mutex);
		if(bowl != 1){//没有饭，则等待
			//没有饭则等待，被唤醒后加锁
			pthread_cond_wait(&cond_customer,&mutex);
		}
		bowl = 0;//能走下来表示有饭  bowl==1，吃完饭将bowl修改为0
		printf("I eat\n");
		//解锁
		pthread_mutex_unlock(&mutex);		
		//唤醒厨师做饭
		pthread_cond_signal(&cond_cook);
	}
	return NULL;
}

int main()
{
	pthread_t cook_tid[4],customer_tid[4];
	int ret,i;
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cond_cook,NULL);
	pthread_cond_init(&cond_customer,NULL);
	for(i = 0;i < 4;i++){
		ret = pthread_create(&cook_tid[i],NULL,thr_cook,NULL);
		if(ret != 0){
			printf("pthread_cook error\n");
			return -1;
		}
		ret = pthread_create(&customer_tid[i],NULL,thr_customer,NULL);
		if(ret != 0){
			printf("pthread_customer error\n");
			return -1;
		}
	}
	pthread_join(cook_tid[0],NULL);
	pthread_join(customer_tid[0],NULL);
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond_cook);
	pthread_cond_destroy(&cond_customer);
	return 0;
}
