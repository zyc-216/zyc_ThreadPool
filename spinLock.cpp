#include "spinlock.h"



void spinLock::lock()
{
	while(af.test_and_set()){}     // while和test_and_set合二为一是原子操作，理解为一条指令，自然不会被打断

}

void spinLock::unlock()
{
	af.clear();
}
