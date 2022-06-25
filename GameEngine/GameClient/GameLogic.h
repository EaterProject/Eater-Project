#pragma once

/// <summary>
/// Ŭ���̾�Ʈ �� ��� �Ŵ����� �����ϴ� ���� �Ŵ���
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
	ObjectFactory*			FactoryGM;	//������Ʈ ���� Ŭ����
	MessageManager*			ObjectGM;	//������Ʈ ���� Ŭ����
private:
	bool IsStart = false;
};

