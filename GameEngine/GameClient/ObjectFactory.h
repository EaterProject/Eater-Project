#pragma once

/// <summary>
/// 오브젝트들 생성을 관리하는 오브젝트 펙토리
/// </summary>
#include <vector>

class GameObject;
class ObjectFactory
{
public:
	ObjectFactory();
	~ObjectFactory();

	void Initialize();
	void Release();

	GameObject* CreateCameraPlayer();	//플레이어 카메라 객체를 생성
	GameObject* CreatePlayer();			//플레이어 객체를 생성
	GameObject* CreateMonster(float x, float y, float z);		//몬스터 객체를 생성
	GameObject* CreateMana();			//마나석 객체를 생성
	GameObject* CreateDrone();			//드론 객체를 생성
	GameObject* CreatePortal();			//몬스터객체가 나오는 포탈 생성
private:
	std::vector<GameObject*> ObjectManager;

	//게임상에 한개만 존재하는 오브젝트를 따로 보관
	GameObject* PlayerObject;
	GameObject* PlayerMainCamera;
};

