#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>

#define IPC_KEY 0X12345678
int main()
{
	//1.创建共享内存    int shmget(标识符，大小，标志位|权限)
	int shm_id = shmget(IPC_KEY,32,IPC_CREAT|0664);
	if(shm_id <0){
		perror("shmget error");
		return -1;
	}
	//2.建立映射关系
	//shmat(操作句柄，映射首地址，操作权限)
	void* shm_start=shmat(shm_id,NULL,0);
	if(shm_start == (void*)-1){
		perror("shmat error");
		return -1;
	}
	//3.进行内存操作
	int i = 0;
	while(1){
		sprintf(shm_start,"%s+%d","HELLO",i++);
			sleep(1);
	}
	//4.解除映射关系
	//shmdt(映射首地址)
	shmdt(shm_start);
	//5.删除共享内存
	//shmctl(操作句柄，要进行的操作 IPC_RMID,共享内存信息结构)
	shmctl(shm_id,IPC_RMID,NULL);
	return 0;
}
