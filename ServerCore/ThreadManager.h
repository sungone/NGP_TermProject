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
	static vector<thread>	_threads;
};