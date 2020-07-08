#include <cstdio>
#include <iostream>
#include <queue>
#include <pthread.h>
using namespace std;
//线程安全的阻塞队列--没有数据则消费者阻塞/数据满了则生产者阻塞

#define QUEUE_MAX 5
class BlockQueue
{
	public:
		BlockQueue(int maxq = QUEUE_MAX):_capacity(maxq){
			pthread_mutex_init(&mutex,NULL);
			pthread_cond_init(&pro_cond,NULL);
			pthread_cond_init(&cus_cond,NULL);
		}
		~BlockQueue(){
			pthread_mutex_destroy(&mutex);
			pthread_cond_destroy(&pro_cond);
			pthread_cond_destroy(&cus_cond);
		}
		bool Push(int data){
			//生产者才会入队数据，如果队列中数据满了则需要阻塞
			pthread_mutex_lock(&mutex);
			//queue.size()获取队列节点个数
			while(_queue.size()==_capacity){
				pthread_cond_wait(&pro_cond,&mutex);
			}
			_queue.push(data);//_queu.push()入队
			pthread_mutex_unlock(&mutex);
			pthread_cond_signal(&cus_cond);//唤醒消费者
			return true;
		}
		bool Pop(int *data)
		{
			//出队都是消费者，有数据才能出队，没有数据要阻塞
			pthread_mutex_lock(&mutex);
			//_queue.empty()若queue为NULL 返回true
			while(_queue.empty()){
				pthread_cond_wait(&cus_cond,&mutex);
			}
			*data=_queue.front();//获取队首节点
			_queue.pop();//出队
			pthread_mutex_unlock(&mutex);
			pthread_cond_signal(&pro_cond);
			return true;
		}
	private:
		queue<int> _queue;
		int _capacity;
		pthread_mutex_t mutex;
		pthread_cond_t pro_cond;
		pthread_cond_t cus_cond;
};

void *thr_pro(void *arg)
{
	BlockQueue *queue=(BlockQueue*)arg;
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
	BlockQueue *queue= (BlockQueue*)arg;
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
	BlockQueue queue;
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
