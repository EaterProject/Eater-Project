#pragma once

#include "EaterEngineDLL.h"
#include "SimpleMath.h"
#include <DirectXMath.h>
#include <vector>
#include "Component.h"
class Rigidbody;
class Collider;
class Transform :public Component
{
public:
	EATER_ENGINEDLL Transform();
	virtual EATER_ENGINEDLL ~Transform();

	//������� ����Ǵ� ������Ʈ
	void Start();
	void TransformUpdate();

	//�����浹�� ��ǥ�� �����ϰ�����
	bool isRigid;

public:
	//���� ������ ���͸� �����´�
	EATER_ENGINEDLL DirectX::SimpleMath::Vector3 GetLocalPosition_UP();
	//���� �����ʹ��� ���͸� �����´�
	EATER_ENGINEDLL DirectX::SimpleMath::Vector3 GetLocalPosition_Right();
	//���� �չ��� ���͸� �����´�
	EATER_ENGINEDLL DirectX::SimpleMath::Vector3 GetLocalPosition_Look();

	EATER_ENGINEDLL void LookAt(GameObject* Target);
	EATER_ENGINEDLL void LookAt_X(GameObject* Target);
	EATER_ENGINEDLL void LookAt_Y(Vector3 Pos);
	EATER_ENGINEDLL void LookAt_Z(GameObject* Target);

	//���� ������ġ���� ���� ������
	EATER_ENGINEDLL void SetLocalPosition(float X, float Y, float Z);
	EATER_ENGINEDLL void AddLocalPosition(float X, float Y, float Z);

	//���� ��ġ���� ��������
	EATER_ENGINEDLL void SetPosition_X(float pos);
	EATER_ENGINEDLL void SetPosition_Y(float pos);
	EATER_ENGINEDLL void SetPosition_Z(float pos);
	EATER_ENGINEDLL void SetPosition(float X, float Y, float Z);
	EATER_ENGINEDLL void SetPosition(DirectX::SimpleMath::Vector3& mPos);
	EATER_ENGINEDLL void SetPosition(DirectX::SimpleMath::Vector3&& mPos);
	//���� ��ġ���� ���� ������
	EATER_ENGINEDLL void AddPosition_X(float pos);
	EATER_ENGINEDLL void AddPosition_Y(float pos);
	EATER_ENGINEDLL void AddPosition_Z(float pos);
	EATER_ENGINEDLL void AddPosition(float X, float Y, float Z);
	EATER_ENGINEDLL void AddPosition(DirectX::SimpleMath::Vector3& mPos);
	EATER_ENGINEDLL void AddPosition(DirectX::SimpleMath::Vector3&& mPos);

	//���� ȸ������ ��������
	EATER_ENGINEDLL void SetRotate_X(float rot);
	EATER_ENGINEDLL void SetRotate_Y(float rot);
	EATER_ENGINEDLL void SetRotate_Z(float rot);
	EATER_ENGINEDLL void SetRotate(float X, float Y, float Z);
	EATER_ENGINEDLL void SetRotate(DirectX::SimpleMath::Vector3& mRot);
	EATER_ENGINEDLL void SetRotate(DirectX::SimpleMath::Vector3&& mRot);
	EATER_ENGINEDLL void SetRotate(DirectX::SimpleMath::Quaternion& mRot);
	EATER_ENGINEDLL void SetRotate(DirectX::SimpleMath::Quaternion&& mRot);
	//���� ȸ������ ���� ������
	EATER_ENGINEDLL void AddRotate_X(float rot);
	EATER_ENGINEDLL void AddRotate_Y(float rot);
	EATER_ENGINEDLL void AddRotate_Z(float rot);
	EATER_ENGINEDLL void AddRotate(float X, float Y, float Z);
	EATER_ENGINEDLL void AddRotate(DirectX::SimpleMath::Vector3& mRot);
	EATER_ENGINEDLL void AddRotate(DirectX::SimpleMath::Vector3&& mRot);
	EATER_ENGINEDLL void AddRotate(DirectX::SimpleMath::Quaternion& mRot);
	EATER_ENGINEDLL void AddRotate(DirectX::SimpleMath::Quaternion&& mRot);

	//���� ũ�Ⱚ�� ��������
	EATER_ENGINEDLL void SetScale_X(float scale);
	EATER_ENGINEDLL void SetScale_Y(float scale);
	EATER_ENGINEDLL void SetScale_Z(float scale);
	EATER_ENGINEDLL void SetScale(float scale);
	EATER_ENGINEDLL void SetScale(float X, float Y, float Z);
	EATER_ENGINEDLL void SetScale(DirectX::SimpleMath::Vector3& mScl);
	EATER_ENGINEDLL void SetScale(DirectX::SimpleMath::Vector3&& mScl);
	//���� ũ�Ⱚ�� ���� ������
	EATER_ENGINEDLL void AddScale_X(float scale);
	EATER_ENGINEDLL void AddScale_Y(float scale);
	EATER_ENGINEDLL void AddScale_Z(float scale);
	EATER_ENGINEDLL void AddScale(float scale);
	EATER_ENGINEDLL void AddScale(float X, float Y, float Z);
	EATER_ENGINEDLL void AddScale(DirectX::SimpleMath::Vector3& mScl);
	EATER_ENGINEDLL void AddScale(DirectX::SimpleMath::Vector3&& mScl);

	//���� ��İ��� ��������
	EATER_ENGINEDLL void SetLoadLocal(DirectX::SimpleMath::Matrix& Local);
	EATER_ENGINEDLL void SetLoadLocal(DirectX::SimpleMath::Matrix&& Local);
	EATER_ENGINEDLL void SetLoadWorld(DirectX::SimpleMath::Matrix& World);
	EATER_ENGINEDLL void SetLoadWorld(DirectX::SimpleMath::Matrix&& World);


	//��ġ,ȸ��,ũ�� ��ĵ��� ���� �����´�
	EATER_ENGINEDLL DirectX::SimpleMath::Matrix GetPositionXM();
	EATER_ENGINEDLL DirectX::SimpleMath::Matrix GetRotationXM();
	EATER_ENGINEDLL DirectX::SimpleMath::Matrix GetScaleXM();

	//�����Ʈ������ ������
	EATER_ENGINEDLL DirectX::SimpleMath::Matrix* GetWorld();
	EATER_ENGINEDLL DirectX::SimpleMath::Matrix* GetLocal();

	//��ġ, ȸ��, ũ�� ���� ������
	EATER_ENGINEDLL DirectX::SimpleMath::Vector3 GetPosition();
	EATER_ENGINEDLL DirectX::SimpleMath::Vector3 GetRotation();
	EATER_ENGINEDLL DirectX::SimpleMath::Quaternion GetRotation_Q();
	EATER_ENGINEDLL DirectX::SimpleMath::Vector3 GetScale();

	//�ڽİ�ü �ֱ�
	EATER_ENGINEDLL void SetChild(Transform* mChild);
	//�θ� ��ü �ֱ�
	EATER_ENGINEDLL void SetParent(Transform* mParent);

	EATER_ENGINEDLL Transform* GetParent();

	//�ڽİ�ü ���� ������Ʈ
	void Child_Local_Update();

	//Y�� �������� ���¹������� õõ�� ȸ���Ѵ�
	EATER_ENGINEDLL void Slow_Y_Rotation(Vector3 Dir, float RotationSpeed, bool Z_Front = false);
	EATER_ENGINEDLL float GetDistance(Vector3 Pos);

private:
	//���� ��ġ ȸ�� ũ�Ⱚ�� ������ ����� ���Ѵ�
	DirectX::SimpleMath::Matrix CreateXMPos4x4();
	DirectX::SimpleMath::Matrix CreateXMRot4x4();
	DirectX::SimpleMath::Matrix CreateXMRot4x4_Q();
	DirectX::SimpleMath::Matrix CreateXMScl4x4();

	//��ġ
	DirectX::SimpleMath::Vector3 Position;
	//ȸ��
	DirectX::SimpleMath::Vector3 Rotation;
	//ũ��
	DirectX::SimpleMath::Vector3 Scale;

	DirectX::SimpleMath::Quaternion Q_Rotation;

	DirectX::SimpleMath::Matrix Load_World; //�ε�� ����
	DirectX::SimpleMath::Matrix Load_Local; //�ε�� ����

	///���� ��ǥ�� ������ġ���� ���ù������� 1��ŭ ������ ��ǥ��
	///���� Ŭ���ʿ��� �ٲ��ʿ䰡���⶧���� �������°͸� ����
	//�ڱ� �������� ���������� 1��ŭ ������ ��ǥ
	DirectX::SimpleMath::Vector3 Local_UP;
	//�ڱ� �������� ������ �������� 1��ŭ ������ ��ǥ
	DirectX::SimpleMath::Vector3 Local_Right;
	//�ڱ� �������� �չ������� 1��ŭ ������ ��ǥ
	DirectX::SimpleMath::Vector3 Local_Look;

	//������ ��ĵ��� ���κ���
	DirectX::SimpleMath::Matrix PositionXM;
	DirectX::SimpleMath::Matrix RotationXM;
	DirectX::SimpleMath::Matrix ScaleXM;

	///��� ������Ʈ �Լ���
	//������ǥ�� �����Ӹ��� ���Ұ��ΰ� ����
	bool LocalUpdate;

	//������Ʈ���� �Ź� �������, ���� ����
	void UpdateTransform();

	//���� ������Ʈ�� ��ġ ȸ�� ũ�Ⱚ�� ��ΰ��� ���� ���
	DirectX::SimpleMath::Matrix World_M;
	DirectX::SimpleMath::Matrix Local_M;
private:
	const float ConvertPI = 180 / 3.141592f;

	bool IsUpdate = false;

	///���� �������� �θ� ��ü �ڽİ�ü
	Transform* Parent;
	std::vector<Transform*> ChildList;


	//physX
	Rigidbody* mRigidbody;
	Collider* mCollider;
};