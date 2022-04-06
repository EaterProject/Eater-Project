#pragma once
#include "ClientComponent.h"

class Transform;
class Potal:public ClientComponent
{
public:
	Potal();
	~Potal();

	void SetUp();
	void Update();

	void OnPotalActivation(bool Active);

	virtual void ReSet() override;
private:
	Transform* mTransform;
private:
	float	CreateTime = 0.0f;		//���� ���� ����
	bool	Activation = false;		//��Ż Ȱ��ȭ ����

	const float CreateMonsterMaxTime = 10.0f;
};

