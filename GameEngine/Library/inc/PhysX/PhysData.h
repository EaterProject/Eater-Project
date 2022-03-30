#pragma once
#include "PhysEngineDLL.h"
#include "SimpleMath.h"
#include <vector>

#include "PhysScene.h"
#include "PhysMaterial.h"
#include "PhysCollider.h"

#pragma warning(disable : 4251)

using namespace DirectX;
using namespace SimpleMath;

/*
Dinamic ��ü
	���� ���ؼ� ������ ���� �ְ� �浹�� ������ ��ü

Knematic ��ü
	�� ��ġ���� �ٸ� ��ġ���� �̵��� ���������� ���� ���ؼ�
	������ �� �ִ� ��ü�� �ƴϴ�

	���� ������ ���� ��ü�� �����ϸ� moveGlobal �Լ��� �����ϼ� �ִ�
	�Ϲ����� Dynamic ���͸� �м��� �ִ� ������ static �� �ٸ� Knematic ��ü�� �浹��������
Static ��ü
	�̰�ü�� ��� ����� ������ ������ �� ���� ��ü�̴�
	������ ��ü�� �ǹ��Ѵ�
*/

///�ϳ��� ��ü�� ����� ����ü ������������ ��������� �Ʒ��ʿ� ����
///������� �ʴ� ���� �׳� ����Ʈ������ ���θ�ȴ�
class  PHYS_ENGINEDLL PhysData
{
public:
	 PhysData();
	 ~PhysData();
	//���� ��ġ
	Vector3 WorldPosition;
	//ȸ��
	Vector4 Rotation;
	//�̰�ü�� ���� �߽���
	Vector3 CenterPoint;
	//������� ��ġ
	 void SetWorldPosition(float x, float y, float z);
	 void SetLocalPosition(float x, float y, float z);

	//������� ȸ��
	 void SetRotation(float x, float y, float z);
	 void SetRotate(float x, float y, float z);
	 void AddForce(float x, float y, float z);
	 void SetVelocity(float x, float y, float z);
public:
	 Vector3 GetVelocity();
	 bool GetTriggerEnter();
	 bool GetTriggerStay();
	 bool GetTriggerExit();
public:
	//����
	float MT_Mass;
	PhysMaterial* mMeterial;
	PhysCollider* mCollider;
public:
	///��ü ���� ������
	//�����̴� ��ü���� (Dinamic , Static)
	bool isDinamic = true;
	//�߷� �ۿ� ����
	bool isGrvity = true;
	//�������� �ʴµ� �浹 �Ұ�����
	bool isKinematic;

	int TriggerCount = 0;
	std::vector<PhysData*> TriggerList;
	std::vector<PhysData*> CollisionList;
public:
	//�̵��� ���� �ຯȯ�� ���´�
	void SetLockAxis_Position(bool x, bool y, bool z);
	//ȸ���� ���� �ຯȯ�� ���´�
	void SetLockAxis_Rotation(bool x, bool y, bool z);
private:
	///��ġ ������
	//���� ��ġ (���� �����϶�)
	Vector3 LocalPosition;
	//��� �� �̵��� ���� ��ų�� ��� 
	Vector3 FreezePositon;
	//��� �� ȸ���� ���� ��ų�� ���
	Vector3 FreezeRotaticon;
	//�̰�ü�� �̵��ӷ��� �־��ش�
	Vector3 Velocity;
	//PhysX���� ������ �����´�
	Vector3 PhysX_Velocity;
	//���� ���� �������� ���� �ٶ� ���
	Vector3 Force;
private:
	bool isMove;
	bool isForce;
	bool isVelocity;
	bool isPosition;
	bool isTrigger;

	bool OnTriggerEnter;
	bool OnTriggerStay;
	bool OnTriggerExit;
private:
	void* ActorObj = nullptr;

	friend class Factory;
	friend class PhysEngine;
	friend class BaseEventCallBack;
};

struct FilterGroup
{
	enum Enum
	{
		eWall = (1 << 0),
		ePlayer = (1 << 0),
	};
};

class PHYS_ENGINEDLL PhysRayCast
{
public:
	struct RaycastHit
	{
		int HitCount; //�浹�� ������Ʈ ����
	};

	PhysRayCast();
	~PhysRayCast();
	Vector3 Origin;			//�������� ��ġ
	Vector3 Direction;		//��� ����
	float MaxDistance;		//�ִ� �Ÿ�

	RaycastHit Hit;
};


