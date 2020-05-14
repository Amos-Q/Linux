#include <cstdio>
#include <iostream>
/*class single_instance
  {
  public:
  int *get_instance(){
  return &_data;
  }
  private:
  static int _data;
  };
  int single_instance:: _data = 10;//静态成员变量需要在类外初始化
  */
#include <thread>
#include <mutex>
std::mutex g_mutex;
class single_instance
{
	public:
		volatile int *get_instance(){
			if(_data == NULL){//4.二次判断，避免每次加锁解锁
				g_mutex.lock();//3.实现线程安全
				if(_data==NULL){
					_data=new int;
					*_data = 10;

				}
				g_mutex.unlock();
			}
			return _data;
		}
	private:
		//1.statci保证所有对象使用同一份资源
		//2.volatile防止编译器过度优化
		volatile static int *_data;
};
volatile int *single_instance::_data = NULL;
int main()
{
	single_instance a1;
	single_instance b1;
	printf("a1:%d-%p;b1:%d-%p\n",*(a1.get_instance()),a1.get_instance(),*(b1.get_instance()),b1.get_instance());
	return 0;
}
