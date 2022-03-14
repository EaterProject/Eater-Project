#pragma once
/// <summary>
/// Global Data ������ ���� ���� Ŭ����
/// </summary>
/// 
/// - �ش� BaseManager�� ��ӹ��� Ŭ������ Global Data�� ������ �� �� �ִ� Manager�鿡�� ����Ѵ�.
/// - Global Data ���õ� Manager���� ���������� GlobalData�� ���� ������Ʈ.

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

