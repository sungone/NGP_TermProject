#pragma once

/*---------------------------------------------------
	ThreadManager ������ ���� �����ϱ����� Ŭ�����̴�.
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