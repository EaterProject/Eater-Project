#pragma once
/// <summary>
/// ���۳�Ʈ���� �ֻ��� Ŭ����
/// ��Ŭ������ ����ϰ� ���� ���۳�Ʈ����
/// ���߿� ���ӿ�����Ʈ�� ����� AddComponent�� �ҋ� �������̵��� �ߴ��� ���ߴ��� �˻� ��
/// ������Ʈ �Լ� ������ ����Ʈ�� �־���
/// </summary>

#include "EaterEngineDLL.h"

class TimeManager;
class KeyinputManager;
class GameObject;

class Component
{
public:
	EATER_ENGINEDLL Component();
	virtual EATER_ENGINEDLL ~Component();

	//Ȱ��ȭ ����
	bool Enabled	= true;
public:
	//���� ������Ʈ ����
	virtual void SetObjectData() {}

	//�Ŵ��� �־��ֱ�
	static void SetManager(TimeManager* time, KeyinputManager* key);

	//�����۳�Ʈ�� ������ �ִ� ���ӿ�����Ʈ
	GameObject* gameobject;
protected:
	int OrderCount  = FUNCTION_ORDER_CENTER;

	//Dll�ȿ����� ���� �Ŵ�����
	static TimeManager*		mTimeManager;
	static KeyinputManager*	mKeyInputManger;
protected:
	///�ʱ�ȭ �ܰ�
	//���� �ܰ� �ʿ��� ���۳�Ʈ Get
	virtual void Awake() {};
	//���۳�Ʈ Initialize 
	virtual void SetUp() {};
	//�ʱ�ȭ�Ȱ����� ���۳�Ʈ ����
	virtual void Start() {};

	///������Ʈ �ܰ�
	//���� �ܰ� ������Ʈ
	virtual void StartUpdate() {};
	//�̵� ��� ������Ʈ
	virtual void TransformUpdate() {};
	//���� ������Ʈ
	virtual void PhysicsUpdate() {};
	//�Ϲ� ������Ʈ
	virtual void Update() {};
	//������ ������Ʈ
	virtual void EndUpdate() {};

protected:
	///����۳�Ʈ�� �Լ��� �Լ�����Ʈ�� ���°�� ����ɰ����� ����
	//ex)MeshFilter�� StartUpdate�� Transform�� StartUpdate ���� ���� ����Ǿ�� �Ѵٸ�
	//MeshFileter �� TransformUpdate_Order = 0; ����ù����
	//Transform �� TransformUpdate_Order = 4; ���� ���������� �ִ´�
	//0 = ���� ���� , 4 = ���� ���߿�
	const int FUNCTION_ORDER_FIRST		= 0;
	const int FUNCTION_ORDER_CENTER		= 2;
	const int FUNCTION_ORDER_LAST		= 4;
	
	unsigned int Awake_Order			= FUNCTION_ORDER_CENTER;
	unsigned int Start_Order			= FUNCTION_ORDER_CENTER;
	unsigned int SetUp_Order			= FUNCTION_ORDER_CENTER;


	unsigned int StartUpdate_Order		= FUNCTION_ORDER_CENTER;
	unsigned int TransformUpdate_Order	= FUNCTION_ORDER_CENTER;
	unsigned int PhysicsUpdate_Order	= FUNCTION_ORDER_CENTER;
	unsigned int DefaultUpdate_Order	= FUNCTION_ORDER_CENTER;
	unsigned int EndUpdate_Order		= FUNCTION_ORDER_CENTER;
private:
	//���� ������Ʈ �ȿ� ���۳�Ʈ�� ã���� �̰����� ã�ƿ�
	size_t ComponentType;

	//�����۳�Ʈ�� ��Լ��� �������̵��Ǿ��ִ��� Ȯ���ϱ�����
	unsigned int FUNCTION_MASK = 0x00000000;

	
	//���� ���θ� �Ʒ� Ŭ�����鿡�Դ� �����
	friend class GameObject;
	friend class ObjectManager;
};



