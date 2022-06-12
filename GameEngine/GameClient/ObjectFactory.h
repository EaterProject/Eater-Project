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
class ClientObjectManager;
class Potal;
class FontImage;

class ObjectFactory
{
public:
	ObjectFactory();
	~ObjectFactory();
public:
	void Initialize(ClientObjectManager* mGM);
	void Release();
		
	static GameObject*		CreatePlayer();									//플레이어 객체를 생성
	static Bullet*			CreateBullet(float x, float y, float z);		//총알 객체를 생성
	static MonsterA*		CreateMonsterA(float x, float y, float z);		//몬스터 객체를 생성
	static MonsterB*		CreateMonsterB(float x, float y, float z);		//몬스터 객체를 생성
	static ManaStone*		CreateManaStone(float x, float y, float z);		//마나석 객체를 생성

	static FontImage*		CreateFontImage(float x, float y);
private:
	//게임상에 한개만 존재하는 오브젝트를 따로 보관
	GameObject* PlayerObject;
	GameObject* PlayerMainCamera;
	ClientObjectManager* mOBJ_GM;
};

