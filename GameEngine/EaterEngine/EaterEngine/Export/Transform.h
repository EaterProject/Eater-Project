#pragma once

#include "EaterEngineDLL.h"
#include "SimpleMath.h"
#include <DirectXMath.h>
#include <vector>
#include "Component.h"
class Rigidbody;
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
	//��ġ
	DirectX::SimpleMath::Vector3 Position;
	//ȸ��
	DirectX::SimpleMath::Vector3 Rotation;
	//ũ��
	DirectX::SimpleMath::Vector3 Scale;

	DirectX::SimpleMath::Quaternion Q_Rotation;
	

	//���� ������ ���͸� �����´�
	EATER_ENGINEDLL DirectX::SimpleMath::Vector3 GetLocalPosition_UP();
	//���� �����ʹ��� ���͸� �����´�
	EATER_ENGINEDLL DirectX::SimpleMath::Vector3 GetLocalPosition_Right();
	//���� �չ��� ���͸� �����´�
	EATER_ENGINEDLL DirectX::SimpleMath::Vector3 GetLocalPosition_Look();

	EATER_ENGINEDLL void LookAt(GameObject* Target);

	//������ġ���� ���� ������
	EATER_ENGINEDLL void SetLocalPosition(float X, float Y, float Z);
	EATER_ENGINEDLL void SetTranlate(float X, float Y, float Z);
	EATER_ENGINEDLL void SetTranlate(DirectX::SimpleMath::Vector3 mPos);

	//���� ȸ������ ���� ������
	EATER_ENGINEDLL void SetRotate(float X, float Y, float Z);
	EATER_ENGINEDLL void SetRotate(DirectX::SimpleMath::Vector3 mRot);

	//���� ũ�Ⱚ�� ���� ������
	EATER_ENGINEDLL void SetScale(float X, float Y, float Z);
	EATER_ENGINEDLL void SetScale(DirectX::SimpleMath::Vector3 mScl);

	//��ġ,ȸ��,ũ�� ��ĵ��� ���� �����´�
	EATER_ENGINEDLL DirectX::SimpleMath::Matrix GetPositionXM();
	EATER_ENGINEDLL DirectX::SimpleMath::Matrix GetRotationXM();
	EATER_ENGINEDLL DirectX::SimpleMath::Matrix GetScaleXM();

	//�����Ʈ������ ������
	EATER_ENGINEDLL DirectX::SimpleMath::Matrix* GetWorld();

	//������ǥ���� �������� ������Ʈ ���ٰ��ΰ� ����
	EATER_ENGINEDLL void SetLocalUpdate(bool isUpdate);

	DirectX::SimpleMath::Matrix Load_World; //�ε�� ����
	DirectX::SimpleMath::Matrix Load_Local; //�ε�� ����

	//�ڽİ�ü �ֱ�
	void SetChild(Transform* mChild);
	//�θ� ��ü �ֱ�
	void SetParent(Transform* mParent);
	//�ڽİ�ü ���� ������Ʈ
	void Child_Local_Updata();

	//Y�� �������� ���¹������� õõ�� ȸ���Ѵ�
	EATER_ENGINEDLL void Slow_Y_Rotation(Vector3 Dir, float RotationSpeed);
private:
	//���� ��ġ ȸ�� ũ�Ⱚ�� ������ ����� ���Ѵ�
	DirectX::SimpleMath::Matrix CreateXMPos4x4();
	DirectX::SimpleMath::Matrix CreateXMRot4x4();
	DirectX::SimpleMath::Matrix CreateXMRot4x4_Q();
	DirectX::SimpleMath::Matrix CreateXMScl4x4();

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

	//������Ʈ���� �Ź� ������� ����
	void UpdateWorldXM();
	//������ǥ�� ������Ʈ �����ش�
	void UpdateLocalPosition();

	//���� ������Ʈ�� ��ġ ȸ�� ũ�Ⱚ�� ��ΰ��� ���� ���
	DirectX::SimpleMath::Matrix World_M;


	///���� �������� �θ� ��ü �ڽİ�ü
	Transform* Parent;
	std::vector<Transform*> ChildList;


	//physX
	Rigidbody* mRigidbody;
};