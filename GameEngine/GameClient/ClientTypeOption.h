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
#define TARGET_BOSS_WEAPON 15
#define TARGET_BOSS_FRIEND 16
#define TARGET_UI_EFFECT 17
#define TARGET_UI_TITLE 18
#define TARGET_UI_STORE 19
#define TARGET_STORE 20

//Player Message
#define MESSAGE_PLAYER_HIT    0				//�÷��̾ �¾Ҵ�
#define MESSAGE_PLAYER_HILL   1				//�÷��̾� ���� ���� �ش�
#define MESSAGE_PLAYER_ATTACK_OK 2			//�÷��̾� ������ �����Ͽ���
#define MESSAGE_PLAYER_ACTIVE_TRUE 3		//�÷��̾� Active TRUE
#define MESSAGE_PLAYER_ACTIVE_FALSE 4		//�÷��̾� Active FALSE
#define MESSAGE_PLAYER_COMBO_RESET 5

//Gate Message
#define MESSAGE_GATE_OPEN  0				//����Ʈ ����			//���� �� : int
#define MESSAGE_GATE_CLOSE 1				//����Ʈ �ݱ�			//���� �� : int
#define MESSAGE_GATE_UNLOCK 2				//����Ʈ ��� Ǯ��		//���� �� : int
#define MESSAGE_GATE_LOCK 3					//����Ʈ ��� ����		//���� �� : int

//UI Message
#define MESSAGE_UI_COMBO 0					//�޺� ��Ʈ ��			//���� �� : int
#define MESSAGE_UI_HP_NOW 1					//ü�� ���� ��Ʈ ��		//���� �� : int
#define MESSAGE_UI_HP_MAX 2					//ü�� �ִ� ��Ʈ ��		//���� �� : int
#define MESSAGE_UI_EMAGIN_NOW 3				//�̸��� ���� ��Ʈ ��		//���� �� : int
#define MESSAGE_UI_EMAGIN_MAX 4				//�̸��� �ִ� ��Ʈ ��		//���� �� : int
#define MESSAGE_UI_MONSTER_UI_ON 5			//���� UI�� �Ҵ�		//���� �� : MONSTER_EMAGIN ����ü
#define MESSAGE_UI_MONSTER_UI_OFF 6			//���� UI�� ����		//���� �� : MONSTER_EMAGIN ����ü
#define MESSAGE_UI_MONSTER_UI_UPDATE 7		//���� UI�� ������Ʈ	//���� �� : MONSTER_EMAGIN ����ü
#define MESSAGE_UI_FADE_IN 8				//���̵� ��
#define MESSAGE_UI_FADE_OUT 9				//���̵� �ƿ�
#define MESSAGE_UI_STORE_ACTIVE	10			//UI ����� ��Ƽ��
#define MESSAGE_UI_PLAYER_ACTIVE 11			//UI �÷��̾� UI


//Camera Message
#define MESSAGE_CAMERA_CINEMATIC_GAME_START 0				//�ó׸�ƽ ����
#define MESSAGE_CAMERA_CINEMATIC_BOSS_START 1				//�ó׸�ƽ ����
#define MESSAGE_CAMERA_CINEMATIC_BOSS_END 2					//�ó׸�ƽ ����
#define MESSAGE_CAMERA_CINEMATIC_GAME_END 3					//�ó׸�ƽ ����
#define MESSAGE_CAMERA_CINEMATIC_TITLE 4					//�ó׸�ƽ ����

#define MESSAGE_CAMERA_CHANGE_PLAYER 5						//�÷��̾� ī�޶�� ��ȯ	//���� �� : ����
#define MESSAGE_CAMERA_CHANGE_DEBUG 6						//����� ī�޶�� ��ȯ	//���� �� : ����

//Global Message
#define MESSAGE_GLOBAL_GAMESTART 0							//������ ���۵Ǿ���
#define MESSAGE_GLOBAL_GAMEEND	1							//������ ����Ǿ���
#define MESSAGE_GLOBAL_TITLE 2								//Ÿ��Ʋ


#define MONSTER_COLOR_RED 0
#define MONSTER_COLOR_BLUE 1
#define MONSTER_TYPE_A 0
#define MONSTER_TYPE_B 1

enum class MONSTER_STATE : int
{
	IDLE	= 0,	//���
	ATTACK	= 1,	//����
	HIT		= 2,	//���� ����
	MOVE	= 3,	//�̵�
	CHASE	= 4,	//�߰�
	DEAD	= 5,	//����
};

enum class BOSS_STATE : int
{
	//����
	GROGGY_START		= 0,	//�׷α� ���� ���� �ܰ�
	GROGGY_PLAY			= 1,	//�׷α� ���� �߰� �ܰ�
	GROGGY_END			= 2,	//�׷α� ���� �� �ܰ�
	IDLE				= 3,	//���
	DEAD				= 4,	//����
	CLOSER_ATTACK_L		= 5,	//���� ���� ����
	CLOSER_ATTACK_R		= 6,	//���� ���� ������
	CHASE_ATTACK_READY	= 7,	//���� �߻�ü �غ�
	CHASE_ATTACK_PLAY	= 8,	//���� �߻�ü �߻�
	RANDOM_ATTACK_READY	= 9,	//������ �߻�ü �غ�
	RANDOM_ATTACK_PLAY	= 10,	//������ �߻�ü �߻�
	TELEPORT_READY		= 11,	//���� �����̵� �غ�
	TELEPORT_START		= 12,	//���� �����̵� 
	CREATE_FRIEND		= 13,	//�н� ��ȯ
	HIT					= 14,	//�¾�����
	RENDOM_ATTACK_END   = 15,	//
};

enum class PLAYER_STATE : int
{
	IDLE		= 0,
	ATTACK_01	= 1,
	ATTACK_02	= 2,
	SKILL_01	= 3,
	SKILL_02	= 4,
	JUMP		= 5,
	MOVE		= 6,
	DEAD		= 7
};

enum class MONSTER_COLOR : int
{
	RED,		
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
	int Type;
	void* Object;
};

