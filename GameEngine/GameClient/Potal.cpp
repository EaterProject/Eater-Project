#include "Potal.h"
#include "MainHeader.h"
#include "Transform.h"
#include "GameObject.h"
#include "MonsterA.h"
#include "MonsterB.h"
Potal::Potal()
{
	mTransform = nullptr;
}

Potal::~Potal()
{
	mTransform = nullptr;
}

void Potal::SetUp()
{
	mTransform = gameobject->GetTransform();
	CreateTime = 0;
}

void Potal::Update()
{
	if (Activation == true)
	{
		CreateTime += GetDeltaTime();
		if (CreateTime >= CreateMonsterMaxTime)
		{
			CreateTime -= CreateMonsterMaxTime;
			MonsterA* Monster = mObjectGM->GetMonsterA();

			if (Monster == nullptr) { return; }
			Monster->gameobject->GetTransform()->Position = mTransform->Position;
			Monster->SetMovePoint(5, 0, -10);
		}
	}
}

void Potal::OnPotalActivation(bool Active)
{
	Activation = Active;
}

void Potal::ReSet()
{


}

