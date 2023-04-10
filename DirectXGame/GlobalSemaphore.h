#pragma once
#include <semaphore>
#include <mutex>

class GlobalSemaphore
{
public:
	GlobalSemaphore(int keys);
	void acquire(int keys);
	void release(int keys);

private:
	std::counting_semaphore<10>* semaphore;
};