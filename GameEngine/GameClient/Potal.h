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
	float	CreateTime = 0.0f;		//몬스터 생성 여부
	bool	Activation = false;		//포탈 활성화 여부

	const float CreateMonsterMaxTime = 10.0f;
};

