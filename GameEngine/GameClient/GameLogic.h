#pragma once

/// <summary>
/// 클라이언트 쪽 모든 매니저를 관리하는 로직 매니저
/// </summary>
#include <vector>

class GameObject;
class ObjectFactory;
class MonsterBase;
class MessageManager;

class GameLogic
{
public:
	GameLogic();
	~GameLogic();
public:
	void Initialize();
	void Release();
	void Update();
private:
	ObjectFactory*			FactoryGM;	//오브젝트 생성 클래스
	MessageManager*			ObjectGM;	//오브젝트 관리 클래스
private:
	bool IsStart = false;
};

