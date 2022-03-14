#pragma once
#include <windows.h>
#include <string>
#include "EnumClass.h"
/// <summary>
/// �׽�Ʈ�� �ӽ� ����� Ŭ����
/// </summary>
class KeyinputManager;
class DebugManager
{
public:
	~DebugManager();
	DebugManager();

	enum class MSG_TYPE
	{
		MSG_LOAD,
		MSG_CREATE,
		MSG_GET,
		MSG_DELETE,
		MSG_PUSH,
		MSG_SYSTEM
	};
	void Initialize(KeyinputManager* mkeyManager,bool mDebugOn);
	static void Print(MSG_TYPE type, std::string typeData,std::string msg,bool Error);
	static void Print(std::string str);
	static void Line(std::string ObjType = "");
	void Update();
	void Delete();
private:
	static HANDLE hConsole;
	static bool DebugON;
};