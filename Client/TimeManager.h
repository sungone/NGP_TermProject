class TimeManager
{

public:
	static TimeManager* GetInstance()
	{
		static TimeManager instance;
		return &instance;
	}


	void Init();
	void Update();

	UINT32 GetFps() { return _fps; }
	float GetDeltaTime() { return _deltaTime; }

private:
	UINT64	_frequency = 0;
	UINT64	_prevCount = 0;
	float	_deltaTime = 0.f;

private:

	HWND	_hwnd;
	UINT32	_frameCount = 0;
	float	_frameTime = 0.f;
	UINT32	_fps = 0;

};

