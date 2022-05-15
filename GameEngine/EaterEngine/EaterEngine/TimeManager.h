#pragma once

/// <summary>
/// Ÿ�̸� �Ŵ��� Ŭ����
/// ���߿� �����ҿ���
/// </summary>

class TimeManager
{
public:
	TimeManager();
	~TimeManager();


	void Initialize();
	void Update();
	float TotalTime()const;  // in seconds
	float DeltaTime()const; // in seconds
	void ResetTime(); // in seconds

	void Reset(); // Call before message loop.
	void Start(); // Call when unpaused.
	void Stop();  // Call when paused.
	void Tick();  // Call every frame.

private:
	double mSecondsPerCount;
	double mDeltaTime;

	__int64 mBaseTime;
	__int64 mPausedTime;
	__int64 mStopTime;
	__int64 mPrevTime;
	__int64 mCurrTime;

	bool mStopped;

	float debugTime = 0;
};

