#include "threadpool.h"

void zyc::CThrpool::initPool()
{
	for (int i = 0; i < DEFAULT_THREADS_NUM; ++i) 
	{
		m_vecThread.emplace_back(&zyc::CThrpool::singleThreadWorking, this);
	}
	std::cout << "thread pool initializes successfully" << std::endl;
}

void zyc::CThrpool::singleThreadWorking()
{
	std::function<void()> task;
	while (1)
	{
		{
			std::unique_lock<std::mutex> lk(m_mutex);

			/// 
			/// 三种方法避免虚假唤醒
			//1
			m_cv.wait(lk, [this]() 
				{
					if (m_atoBoolStopFetch.load())
						return false;
					return !this->m_queTask.empty() || m_atoBoolShutDown.load(); 
				}); //wait的第二个参数lambda表达式避免了虚假唤醒
			//2
			//while (this->m_queTask.empty() && !this->m_shutdown)
			//{
			//	m_cv.wait(lk);
			//}
			//3
			//m_cv.wait(lk);
			//if (this->m_queTask.empty() && !this->m_shutdown)
			//{
			//	m_cv.wait(lk);
			//}
			///


			//关闭线程池的判断
			if (m_atoBoolShutDown && this->m_queTask.empty()) 
				return;

			task = std::move(m_queTask.front());	//????move
			m_queTask.pop();
		}

		try 
		{
			task();
		}
		catch (const std::bad_function_call& e) 
		{
			std::cout <<"error!" << e.what() << std::endl;
		}
	}
}

void zyc::CThrpool::StopFetchTask()
{
	m_atoBoolStopFetch.store(true);
}

void zyc::CThrpool::StartFetchTask()
{
	m_atoBoolStopFetch.store(false);
	m_cv.notify_all();
}

zyc::CThrpool::CThrpool()
{
	m_atoBoolShutDown.store(false);
	m_atoBoolStopFetch.store(false);
}

zyc::CThrpool::~CThrpool()
{
	//{
	//	std::unique_lock<std::mutex> lk(m_mutex);
	//	m_shutdown = true; 
	//}
	m_atoBoolStopFetch.store(false);
	m_atoBoolShutDown.store(true);
	m_cv.notify_all();

	for (auto& thr : m_vecThread) 
	{
		if (thr.joinable()) 
		{
			thr.join();
		}
	}
	std::cout << "thread pool exits successfully" << std::endl;
}


