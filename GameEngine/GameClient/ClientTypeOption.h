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
#define MESSAGE_GATE_OPEN  0		//����Ʈ ����
#define MESSAGE_GATE_CLOSE 1		//����Ʈ �ݱ�
#define MESSAGE_GATE_UNLOCK 2		//����Ʈ ��� Ǯ��

//UI Message
#define MESSAGE_UI_COMBO 0			//�޺� ��Ʈ ��
#define MESSAGE_UI_HP_NOW 1			//ü�� ���� ��Ʈ ��
#define MESSAGE_UI_HP_MAX 2			//ü�� �ִ� ��Ʈ ��
#define MESSAGE_UI_EMAGIN_NOW 3		//�̸��� ���� ��Ʈ ��
#define MESSAGE_UI_EMAGIN_MAX 4		//�̸��� �ִ� ��Ʈ ��
#define MESSAGE_UI_MONSTER_UI_ON 5	//���� UI�� �����ش�

//Camera Message
#define MESSAGE_CAMERA_START_GAME 0	//������ ���۵Ǿ�����
#define MESSAGE_CAMERA_START_BOSS 1	//������ �����Ǿ�����
#define MESSAGE_CAMERA_END_BOSS 2	//������ �׾�����

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
struct MONSTER_EMAGIN
{
	float R = 255.f;
	float G = 255.f;
	float B = 255.f;
	int ComboCount = 7;
	int HP;
};

