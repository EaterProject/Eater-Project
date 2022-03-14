#pragma once
/// <summary>
/// Global Data 공유를 위한 상위 클래스
/// </summary>
/// 
/// - 해당 BaseManager를 상속받은 클래스는 Global Data에 변동을 줄 수 있는 Manager들에만 상속한다.
/// - Global Data 관련된 Manager들은 전역변수인 GlobalData에 각각 업데이트.

class GlobalData;

class BaseManager
{
public:
	BaseManager() = default;
	virtual ~BaseManager() = default;

protected:
	static GlobalData* Global;

public:
	static void Initialize();
	static void Reset();

	static void UpdateGlobalData(float dTime);

	static GlobalData* GetGlobalData();
};

