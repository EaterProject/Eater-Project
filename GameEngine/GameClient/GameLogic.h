#pragma once

/// <summary>
/// 클라이언트 쪽 모든 매니저를 관리하는 로직 매니저
/// </summary>
#include <vector>

class ObjectFactory;

class GameLogic
{
public:
	GameLogic();
	~GameLogic();
	void Initialize();
	void Release();
private:
	ObjectFactory* Factory;

};

