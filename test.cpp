#include "threadpool.h"
#include <chrono>

void taskAdd(const int a, const int b) {
	//std::cout << "task add begin" << std::endl;
	std::cout << a + b << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(2));
	//std::cout << "task add end" << std::endl;
}


void taskAddThree(const int a, const int b, const int c) {
	//std::cout << "task addthree begin" << std::endl;
	std::cout << a + b + c << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(2));
	//std::cout << "task addthree end" << std::endl;
}

int main()
{
	//�����̳߳ض���
	zyc::CThrpool pool;
	pool.initPool();
	//����������������������
	for (int i = 0; i < 3; ++i) 
	{
		pool.pushTask(taskAdd, i, 0);
	}
	std::this_thread::sleep_for(std::chrono::seconds(5)); //�ȴ�ǰ��������ȡ����ִ����
	pool.StopFetchTask();
	for (int i = 0; i < 2; ++i) 
	{
		pool.pushTask(taskAddThree, i, 0, 10);
	}
	std::cout << "before restart" << std::endl;
	pool.StartFetchTask();
	std::cout << "after restart" << std::endl;

}