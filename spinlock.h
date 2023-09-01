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

//spinLock和mutex互斥锁的区别：
// spinLock：拿不到锁的会一直等待
// mutex：拿不到锁的会被操作系统调度，变成sleep

#endif // !SPINLOCK_H_
