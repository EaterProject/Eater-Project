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

class Phys_Collider_Capsule;
class Phys_Collider_Sphere;
class Phys_Collider_Triangle;
class Phys_Collider_Box;


///�ϳ��� ��ü�� ����� ����ü ������������ ��������� �Ʒ��ʿ� ����
///������� �ʴ� ���� �׳� ����Ʈ������ ���θ�ȴ�
class  PHYS_ENGINEDLL PhysData
{
public:
	 PhysData();
	 ~PhysData();
	 void* EaterObj = nullptr;

	Vector3 WorldPosition;	//��ġ
	Vector4 Rotation;		//ȸ��
	Vector3 CenterPoint;	//�߽���
	//������� ��ġ
	 void SetWorldPosition(float x, float y, float z);
	 void SetLocalPosition(float x, float y, float z);

	//������� ȸ��
	 void SetRotation(float x, float y, float z);
	 void SetRotate(float x, float y, float z);
	 void AddForce(float x, float y, float z);
	 void SetVelocity(float x, float y, float z);
	 void SetTrigger(bool mTrigger);
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
	bool isGravity = true;
	//�������� �ʴµ� �浹 �Ұ�����
	bool isKinematic;

	int TriggerCount = 0;


	int Enter_Count		= 0;
	int Stay_Count		= 0;
	int Exit_Count		= 0;
	PhysData* TriggerEnter_List[10];
	PhysData* TriggerStay_List[10];
	PhysData* TriggerExit_List[10];
	//PhysData* TriggerList[10];
public:
	//�̵��� ���� �ຯȯ�� ���´�
	void SetLockAxis_Position(bool x, bool y, bool z);
	Vector3 GetLockAxis_Position();
	//ȸ���� ���� �ຯȯ�� ���´�
	void SetLockAxis_Rotation(bool x, bool y, bool z);
	Vector3 GetLockAxis_Rotation();

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
	
	void PushTriggerEnter_Data(PhysData* Data);
	void PushTriggerExit_Data(PhysData* Data);

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


