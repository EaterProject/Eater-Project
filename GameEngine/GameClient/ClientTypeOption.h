#pragma once

//Taget,Create Message
#define TARGET_PLAYER 0
#define TARGET_DRONE 1
#define TARGET_COMBO 2
#define TARGET_BOSS 3
#define TARGET_GATE 4
#define TARGET_MONSTER_A 5
#define TARGET_MONSTER_B 6
#define TARGET_MANA 7
#define TARGET_UI 8
#define TARGET_GATE_IN 9
#define TARGET_GATE_OUT 10
#define TARGET_GATE_MANAGER 11
#define TARGET_CAMERA_MANAGER 12
#define TARGET_GLOBAL 13
#define TARGET_PLAYER_CAMERA 14

//Player Message
#define MESSAGE_PLAYER_HIT    0		//�÷��̾ �¾Ҵ�
#define MESSAGE_PLAYER_HILL   1		//�÷��̾� ���� ���� �ش�
#define MESSAGE_PLAYER_ATTACK_OK 2	//�÷��̾� ������ �����Ͽ���

//Gate Message
#define MESSAGE_GATE_OPEN  0		//����Ʈ ����			//���� �� : int
#define MESSAGE_GATE_CLOSE 1		//����Ʈ �ݱ�			//���� �� : int
#define MESSAGE_GATE_UNLOCK 2		//����Ʈ ��� Ǯ��		//���� �� : int

//UI Message
#define MESSAGE_UI_COMBO 0			//�޺� ��Ʈ ��			//���� �� : int
#define MESSAGE_UI_HP_NOW 1			//ü�� ���� ��Ʈ ��		//���� �� : int
#define MESSAGE_UI_HP_MAX 2			//ü�� �ִ� ��Ʈ ��		//���� �� : int
#define MESSAGE_UI_EMAGIN_NOW 3		//�̸��� ���� ��Ʈ ��		//���� �� : int
#define MESSAGE_UI_EMAGIN_MAX 4		//�̸��� �ִ� ��Ʈ ��		//���� �� : int
#define MESSAGE_UI_MONSTER_UI_ON 5	//���� UI�� �����ش�	//���� �� : MONSTER_EMAGIN ����ü
#define MESSAGE_UI_RENDER 6			//��� UI ������ ����		//���� �� : bool

//Camera Message
#define MESSAGE_CAMERA_CINEMATIC_GAME_START 0				//�ó׸�ƽ ����
#define MESSAGE_CAMERA_CINEMATIC_BOSS_START 1				//�ó׸�ƽ ����
#define MESSAGE_CAMERA_CINEMATIC_BOSS_END 2					//�ó׸�ƽ ����
#define MESSAGE_CAMERA_CINEMATIC_GAME_END 3					//�ó׸�ƽ ����

#define MESSAGE_CAMERA_CHANGE_PLAYER 4						//�÷��̾� ī�޶�� ��ȯ	//���� �� : ����
#define MESSAGE_CAMERA_CHANGE_DEBUG 5						//����� ī�޶�� ��ȯ	//���� �� : ����

//Global Message
#define MESSAGE_GLOBAL_GAMESTART 0	//������ ���۵Ǿ���
#define MESSAGE_GLOBAL_GAMEEND	1	//������ ����Ǿ���


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

