#include <cstdio>
#include <iostream>
#include <vector>
#include <pthread.h>
#include <semaphore.h>

#define QUEUE_MAX 5
class RingQueue{
	public:
		RingQueue(int maxq=QUEUE_MAX):_queue(maxq),_capacity(maxq),_step_read(0),_step_write(0){
			//sem_init(信号量 ，进程/线程，信号量初始值)
			sem_init(&_lock,0,1);
			sem_init(&_sem_data,0,0);
			sem_init(&_sem_idle,0,maxq);
		}
		~RingQueue(){
			sem_destroy(&_lock);
			sem_destroy(&_sem_data);
			sem_destroy(&_sem_idle);
		}
		bool Push(int data){
			//1.是否能够访问资源，不能则阻塞
			sem_wait(&_sem_idle);//空间空闲计数判断，空闲空间计数-1
			//2.能访问，则加锁，保护访问过程
			sem_wait(&_lock);
			//3.资源访问
			_queue[_step_write]=data;
			_step_write = (_step_write+1)%_capacity;
			//4.解锁
			sem_post(&_lock);
			//5.入队数据，数据空间+1，唤醒消费者
			sem_post(&_sem_data);
			return true;
		}
		bool Pop(int *data){
			sem_wait(&_sem_data);
			sem_wait(&_lock);
			*data=_queue[_step_read];
			_step_read = (_step_read +1)%_capacity;
			sem_post(&_lock);
			sem_post(&_sem_idle);
			return true;
		}
	private:
		std::vector<int> _queue;//数组
		int _capacity;//这是队列的容量
		int _step_read;//获取数据的位置下标
		int _step_write;//写入数据的位置下标
		sem_t _lock;//这个信号量用于实现互斥
		sem_t _sem_idle;//这个信号量用于对空闲空间进行计数，对于生产者来说空闲空间计数>0才能写入 ---初始为节点个数
		sem_t _sem_data;//这个信号量用于对具有数据的空间进行计数--对于消费者来说有数据的空间计数>0时候才能取出数据--初始为0
};

void *thr_pro(void *arg)
{
	RingQueue *queue=(RingQueue*)arg;
	int i = 0;
	while(1){
		//生产者生成数据
		queue->Push(i);
		printf("pro push data:%d\n",i++);
	}
	return NULL;
}

void *thr_cus(void *arg)
{
	RingQueue *queue= (RingQueue*)arg;
	while(1){
		int data;
		queue->Pop(&data);
		printf("cus pop data:%d\n",data);
	}
	return NULL;
}

int main()
{
	int ret,i;
	pthread_t ptid[4],ctid[4];
	RingQueue queue;
	for(i = 0;i <4;i++){
		ret = pthread_create(&ptid[i],NULL,thr_pro,(void*)&queue);
		if(ret != 0){
			printf("create pro error\n");
			return -1;
		}
	}
	for(i = 0;i <4;i++){
		ret = pthread_create(&ctid[i],NULL,thr_cus,(void*)&queue);
		if(ret != 0){
			printf("create cus error\n");
			return -1;
		}
	}
	for(i = 0;i<4;i++){
		pthread_join(ptid[i],NULL);
		pthread_join(ctid[i],NULL);
	}
}
