#pragma once

/*---------------------------------------------------
	ThreadManager 쓰레드 들을 관리하기위한 클래스이다.
------------------------------------------------------*/

class ThreadManager
{
public:
	ThreadManager() = delete;
	~ThreadManager();

	static void	Launch(function<void(void)> callback);
	static void	Join();

private:
	static mutex _lock;
	static vector<thread>	_threads;
};