#include "pch.h"
#include "ThreadManager.h"

/*------------------
	ThreadManager
-------------------*/

std::vector<std::thread> ThreadManager::_threads;
std::mutex ThreadManager::_lock;


ThreadManager::~ThreadManager()
{
	Join();
}

void ThreadManager::Launch(function<void(void)> callback)
{
	lock_guard<mutex> guard(_lock);

	_threads.push_back(thread([=]()
		{
			callback();
		}));
}

void ThreadManager::Join()
{
	for (thread& t : _threads)
	{
		if (t.joinable())
			t.join();
	}
	_threads.clear();
}