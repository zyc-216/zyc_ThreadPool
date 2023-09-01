#pragma once
#ifndef _ZYC_THREAD_POOL_h
#define _ZYC_THREAD_POOL_h

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
//#include <future>
#include <functional>	
#include <queue>	
#include <vector>	
#include <exception>
#include <atomic>
#define	DEFAULT_THREADS_NUM 8
namespace zyc 
{
	class CThrpool
	{
	public:

		CThrpool();
		~CThrpool();    //����������ʹ��join�ȴ����߳̽���

		void initPool();			//��ʼ���̳߳أ��߳�����
		void StopFetchTask();			//�̳߳�ȫ������
		void StartFetchTask();			//����ȫ���̳߳�


		template<typename F, typename... Args>
		void pushTask(F&& f, Args&&...args) //������������ύ���񣬲������߳�ȡ����ִ��
		{
			std::function<decltype(f(args...))()> funcOfZeroArg = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
			//	auto task = std::make_shared<decltype(funcOfZeroArg)>(funcOfZeroArg);

			std::function<void()> func = [funcOfZeroArg]() { funcOfZeroArg(); };
			//	std::function<void()> func = [task]() {(*task)(); };
			{
				std::unique_lock<std::mutex> lk(m_mutex);

				m_queTask.emplace(std::move(func));
			}
			m_cv.notify_one();
		}
	private:
		void singleThreadWorking();   //�����߳�ѭ���ȴ�������

	public:
		std::queue<std::function<void()>>	m_queTask;
		std::vector<std::thread>			m_vecThread;
		std::mutex							m_mutex;
		std::condition_variable				m_cv;
		std::atomic<bool>					m_atoBoolShutDown;
		std::atomic<bool>					m_atoBoolStopFetch;
		//bool m_shutdown = false;    //����ԭ����������Ҫ�����ʹ��

	};
}

#endif //!_ZYC_THREAD_POOL_h

