#pragma once
#include "PhysEngineDLL.h"
#include "SimpleMath.h"
#include <vector>


#pragma warning(disable : 4251)

using namespace DirectX;
using namespace SimpleMath;

 enum class SHAPE_TYPE
{
	BOX,
	SPHERE,
	CAPSULE,
	TRIANGLE
};

///���̽� �������� �ݶ��̴��� �����ϰ������ �ʿ��� ������
struct  TriangleMeshData
{
	Vector3* VertexList = nullptr;
	UINT* IndexList		= nullptr;
	int VertexListSize	= 0;
	int IndexListSize	= 0;
};

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
	 Vector3 GetVelocity();
public:
	///����(���׸���) ������
	//���� ������
	float MT_StaticFriction;
	//� ������
	float MT_DynamicFriction;
	//������
	float MT_Restitution;
	//����
	float MT_Mass;
public:
	///��ü ���� ������
	//�����̴� ��ü���� (Dinamic , Static)
	bool isDinamic = true;
	//�߷� �ۿ� ����
	bool isGrvity = true;
	//�������� �ʴµ� �浹 �Ұ�����
	bool isKinematic;
public:
	//���� ���� ����
	void CreateBoxCollider(float x, float y, float z);
	//������
	void CreateBoxCollider(float Radius);
	//������
	void CreateSphereCollider(float Radius);
	//������,����
	void CreateCapsuleCollider(float Radius, float Height);
	//���̽��������� �ݶ��̴��� ���鶧
	void CreateTriangleCollider(TriangleMeshData* Triangle, int VertexSize,int IndexSize);
	
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
	///�ݶ��̴� ������
	Vector3 Shape_Size;			//������
	SHAPE_TYPE	Shape_type; //Ÿ��
private:
	bool isMove;
	bool isForce;
	bool isVelocity;
	bool isPosition;
private:
	void* ActorObj = nullptr;
	TriangleMeshData* Triangle = nullptr;


	friend class Factory;
	friend class PhysEngine;
};

//Phys ������ ���� ������
struct PhysSceneData
{
	

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

/*
*
 ������ (Restitution)
	�籸���� ���� ���ڸ� �����̴� ���� �������ִ� ���� �浹������ �����̴� ���� ���߰�
	�����ִ� ���� �����̰� �ȴ� �̶� �ۿ�Ǵ°��� ������ 1�ϼ��� �浹�Ͽ����� �ӵ��� �����ȴ�
	0�̶�� �浹 ���ڸ��� ����� �� 1 = ���� ź��  0 = ��ź�� �浹

 ���� ������ (StaticFriction) , � ������ (DynamicFriction)
	��ü�� �����̱� ���� �������� �ǹ� ���� ���ڸ� ������ �ִ� �ڵ����� ó�������̱�� ��������
	�����̱� �����ϸ� ū���� �����ʴ´� ��ü�� �����̱� �����ϸ� � �������� ����ȴ� ���� ��쿡
	� �������� ���� �����º��� �۱⶧���� 
	
	F(��) = u(�������) * m(����) * g(�߷�) 

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
