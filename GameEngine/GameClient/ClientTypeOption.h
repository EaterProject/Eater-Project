#pragma once
enum class MONSTER_STATE : int
{
	IDLE,	//���
	ATTACK,	//����
	HIT,	//���� ����
	MOVE,	//�̵�
	CHASE,	//�߰�
	DEAD,	//����
};
enum class MONSTER_COLOR : int
{
	RED,		//
	GREEN,
	YELLOW,
	BLUE
};