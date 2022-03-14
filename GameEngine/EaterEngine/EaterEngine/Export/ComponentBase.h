#pragma once
#include "EaterEngineDLL.h"

class GameObject;
class IComponent
{
public:
	EATER_ENGINEDLL IComponent() = default;
	virtual ~IComponent() = default;

public:
	//Ȱ��ȭ ����
	bool Enabled = true;

protected:
	int OrderCount = FUNCTION_ORDER_CENTER;

	//�����۳�Ʈ�� ������ �ִ� ���ӿ�����Ʈ
	GameObject* gameobject;

protected:
	///����۳�Ʈ�� �Լ��� �Լ�����Ʈ�� ���°�� ����ɰ����� ����
	//ex)MeshFilter�� StartUpdate�� Transform�� StartUpdate ���� ���� ����Ǿ�� �Ѵٸ�
	//MeshFileter �� TransformUpdate_Order = 0; ����ù����
	//Transform �� TransformUpdate_Order = 4; ���� ���������� �ִ´�
	//0 = ���� ���� , 4 = ���� ���߿�
	const int FUNCTION_ORDER_FIRST = 0;
	const int FUNCTION_ORDER_CENTER = 2;
	const int FUNCTION_ORDER_LAST = 4;

	unsigned int Awake_Order = FUNCTION_ORDER_CENTER;
	unsigned int Start_Order = FUNCTION_ORDER_CENTER;
	unsigned int StartUpdate_Order = FUNCTION_ORDER_CENTER;
	unsigned int TransformUpdate_Order = FUNCTION_ORDER_CENTER;
	unsigned int PhysicsUpdate_Order = FUNCTION_ORDER_CENTER;
	unsigned int DefaultUpdate_Order = FUNCTION_ORDER_CENTER;
	unsigned int EndUpdate_Order = FUNCTION_ORDER_CENTER;
private:
	//���� ������Ʈ �ȿ� ���۳�Ʈ�� ã���� �̰����� ã�ƿ�
	size_t ComponentType;

	//�����۳�Ʈ�� ��Լ��� �������̵��Ǿ��ִ��� Ȯ���ϱ�����
	unsigned int FUNCTION_MASK = 0x00000000;


protected:
	//���� �ܰ迡 ������� ����Ǵ� �Լ�
	virtual void Awake() {};
	//���� �Լ�
	virtual void Start() {};
	//ó��
	virtual void StartUpdate() {};
	//�̵� ��� ������Ʈ
	virtual void TransformUpdate() {};
	//���� ������Ʈ
	virtual void PhysicsUpdate() {};
	//�Ϲ� ������Ʈ
	virtual void Update() {};
	//������ ������Ʈ
	virtual void EndUpdate() {};
};
