#include "spinlock.h"



void spinLock::lock()
{
	while(af.test_and_set()){}     // while��test_and_set�϶�Ϊһ��ԭ�Ӳ��������Ϊһ��ָ���Ȼ���ᱻ���

}

void spinLock::unlock()
{
	af.clear();
}
