#include "MonsterBase.h"

MonsterBase::MonsterBase()
{
	//��� ���ϴ� ��
	HP = 100;
	AttackPower = 10;
	AttackDelayTime = 5.0f;
	State = STATE::CREATE;

	//������ ��
	Start_HP = 100;
	Start_AttackPower = 10;
	Start_AttackDelayTime = 5.0f;
	Start_State = STATE::CREATE;

	isLife = false;
}

MonsterBase::~MonsterBase()
{

}

void MonsterBase::Awake()
{


}

void MonsterBase::SetPlayer(GameObject* Obj)
{
	mPlayer = Obj;
}

GameObject* MonsterBase::GetPlayer()
{
	return mPlayer;
}

void MonsterBase::Reset()
{
	//������ ���¸� ó�����·� �ǵ�����
	HP = Start_HP;
	AttackPower = Start_AttackPower;
	AttackDelayTime = Start_AttackDelayTime;
	Start_State = STATE::CREATE;
}

void MonsterBase::Create(float mHP, float mAttackPower, float mAttackTime)
{
	//�ʱⰪ ����
	Start_HP = mHP;
	Start_AttackPower = mAttackPower;
	Start_AttackDelayTime = mAttackTime;
	Start_State = STATE::CREATE;
}

void MonsterBase::ErrorCheck()
{
	

}

void MonsterBase::SetPosition(float x, float y, float z)
{


}
