#pragma once
#ifndef SPINLOCK_H_
#define	SPINLOCK_H_

#include <mutex>
#include <atomic>

class spinLock
{
public:
	spinLock();
	spinLock(const spinLock&) = delete;
	spinLock(spinLock&&) = delete;
	spinLock& operator=(const spinLock&) = delete;
	spinLock& operator=(spinLock&&) = delete;

	void lock();
	void unlock();

private:
	std::atomic_flag af= ATOMIC_FLAG_INIT;
};

//spinLock��mutex������������
// spinLock���ò������Ļ�һֱ�ȴ�
// mutex���ò������Ļᱻ����ϵͳ���ȣ����sleep

#endif // !SPINLOCK_H_
