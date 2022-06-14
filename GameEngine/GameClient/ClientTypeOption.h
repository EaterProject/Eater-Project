#pragma once

//Taget,Create Message
#define TARGET_PLAYER 0
#define TARGET_DRONE 1
#define TARGET_COMBO 2
#define TARGET_BOSS 3
#define TARGET_GATE 4
#define TARGET_MONSTER_A 5
#define TARGET_MONSTER_B 6
#define TARGET_GLOBAL 7
#define TARGET_MANA 8
#define TARGET_UI 9
#define TARGET_GATE_IN 10
#define TARGET_GATE_OUT 11
#define TARGET_GATE_MANAGER 12


//Player Message
#define MESSAGE_PLAYER_HIT    0		//�÷��̾ �¾Ҵ�
#define MESSAGE_PLAYER_HILL   1		//�÷��̾� ���� ���� �ش�
#define MESSAGE_PLAYER_ATTACK_OK 2	//�÷��̾� ������ �����Ͽ���


//Gate Message
#define MESSAGE_GATE_OPEN  0
#define MESSAGE_GATE_CLOSE 1

//Combo Message
#define MESSAGE_GLOBAL_COMBO 0
#define MESSAGE_GLOBAL_HP_NOW 1
#define MESSAGE_GLOBAL_HP_MAX 2
#define MESSAGE_GLOBAL_EMAGIN_NOW 3
#define MESSAGE_GLOBAL_EMAGIN_MAX 4



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

