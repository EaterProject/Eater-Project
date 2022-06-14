#pragma once

/// <summary>
/// 오브젝트들 생성을 관리하는 오브젝트 펙토리
/// </summary>
#include <vector>
enum class MONSTER_TYPE
{
	MONSTER_A, //물기공격 몬스터 
	MONSTER_B, //자폭공격 몬스터
};

class ClientComponent;
class GameObject;
class MonsterA;
class MonsterB;
class AttackDrone;
class HealingDrone;
class Bullet;
class ManaStone;
class MessageManager;
class Potal;
class FontImage;
class ComboFont;
class UICanvas;

class ObjectFactory
{
public:
	ObjectFactory();
	~ObjectFactory();
public:
	void Release();
		
	GameObject* CreatePlayer();			//플레이어 객체를 생성
	GameObject* CreateBullet();			//총알 객체를 생성
	GameObject* CreateMonsterA();		//몬스터 객체를 생성
	GameObject* CreateMonsterB();		//몬스터 객체를 생성
	GameObject* CreateManaStone();		//마나석 객체를 생성
	GameObject* CreateFontImage();		//폰트 이미지 생성
	GameObject* CreateUICanvas();	//플레이어 스테이트 UI생성
	GameObject* CreateGate_In();
	GameObject* CreateGate_Out();
	GameObject* CreateGate_Manager();
private:
	//게임상에 한개만 존재하는 오브젝트를 따로 보관
	GameObject* PlayerObject;
	GameObject* PlayerMainCamera;

	std::vector<GameObject*> ManaPoint_List;
	std::vector<MonsterA*> MonsterA_List;
	std::vector<MonsterB*> MonsterB_List;
	ComboFont* mComboFont;

	friend MessageManager;
};

