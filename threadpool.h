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
		~CThrpool();    //析构函数里使用join等待子线程结束

		void initPool();			//初始化线程池，线程启动
		void StopFetchTask();			//线程池全部休眠
		void StartFetchTask();			//唤醒全部线程池


		template<typename F, typename... Args>
		void pushTask(F&& f, Args&&...args) //向任务队列里提交任务，并唤醒线程取任务执行
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
		void singleThreadWorking();   //单条线程循环等待任务唤醒

	public:
		std::queue<std::function<void()>>	m_queTask;
		std::vector<std::thread>			m_vecThread;
		std::mutex							m_mutex;
		std::condition_variable				m_cv;
		std::atomic<bool>					m_atoBoolShutDown;
		std::atomic<bool>					m_atoBoolStopFetch;
		//bool m_shutdown = false;    //改用原子量，不需要配合锁使用

	};
}

#endif //!_ZYC_THREAD_POOL_h

