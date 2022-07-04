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
#define TARGET_UI_OPTION 21
#define TARGET_UI_PAUSE 22
#define TARGET_UI_MANUAL 23
#define TARGET_UI_CREDIT 24
#define TARGET_UI_BOSS 25
#define TARGET_SCENE_EFFECT 26

//Player Message
#define MESSAGE_PLAYER_HIT    0				//�÷��̾ �¾Ҵ�
#define MESSAGE_PLAYER_HEAL   1				//�÷��̾� ���� ���� �ش�
#define MESSAGE_PLAYER_ATTACK_OK 2			//�÷��̾� ������ �����Ͽ���
#define MESSAGE_PLAYER_ACTIVE_TRUE 3		//�÷��̾� Active TRUE
#define MESSAGE_PLAYER_ACTIVE_FALSE 4		//�÷��̾� Active FALSE
#define MESSAGE_PLAYER_COMBO_RESET 5		//�÷��̾� �޺� ����
#define	MESSAGE_PLAYER_PUSH 6				//�÷��̾� �и�
#define MESSAGE_PLAYER_LIGHT_CHANGE 7		//�÷��̾� ��ī�� ����Ʈ ����
#define MESSAGE_PLAYER_GET_PUREMANA 8		//�÷��̾� ǻ��� ȹ��
#define MESSAGE_PLAYER_GET_COREMANA 9		//�÷��̾� �ھ�� ȹ��
#define MESSAGE_PLAYER_UPGRADE_EMAGIN 10		//�÷��̾� �̸��� Ƚ�� ��ȭ
#define MESSAGE_PLAYER_UPGRADE_HP 11		//�÷��̾� ü�� ��ȭ
#define MESSAGE_PLAYER_UPGRADE_ATTACK 12	//�÷��̾� ���ݼӵ� ��ȭ
#define MESSAGE_PLAYER_UPGRADE_MOVE 13		//�÷��̾� �̵��ӵ� ��ȭ

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
#define MESSAGE_UI_PLAYER_ACTIVE 10			//UI �÷��̾� UI
#define MESSAGE_UI_PLAYER_HIT 11			//�÷��̾� �ǰ�
#define MESSAGE_UI_STORE_ACTIVE	12			//UI ����� ��Ƽ��
#define MESSAGE_UI_BOSS_HP 13
#define MESSAGE_UI_BOSS_HP_MAX 14
#define MESSAGE_UI_BOSS_ACTIVE 16
#define MESSAGE_UI_PUREMANA 17
#define MESSAGE_UI_COREMANA 18
#define MESSAGE_UI_SKILL_E 19
#define MESSAGE_UI_SKILL_MR 20
#define MESSAGE_UI_SKILL_SPC 21

//Drone Message
#define MESSAGE_DRONE_GAME_START 0			//���� ���۽� �ؽ�Ʈ
#define MESSAGE_DRONE_DOOM_OUT 1			//�� �ܺη� ������ �ؽ�Ʈ
#define MESSAGE_DRONE_STORE 2				//���Ǳ� �� �ؽ�Ʈ
#define MESSAGE_DRONE_PURCHASE_SUCCESS 3	//���Ǳ� ���� ���� �ؽ�Ʈ
#define MESSAGE_DRONE_PURCHASE_FAIL 4		//���Ǳ� ���� ���� �ؽ�Ʈ
#define MESSAGE_DRONE_BOSS_START 5			//���� ���� �ؽ�Ʈ
#define MESSAGE_DRONE_BOSS_ZONE_IN 6		//������ ���� �ؽ�Ʈ
#define MESSAGE_DRONE_MANA_CREATE 7			//������ ���� �ؽ�Ʈ
#define MESSAGE_DRONE_GET_PUREMANA 8		//���� ���� ��� �ؽ�Ʈ
#define MESSAGE_DRONE_GET_COREMANA 9		//������ ���� ���� �ؽ�Ʈ
#define MESSAGE_DRONE_PLAYER_DIE 10			//�÷��̾� ��� �ؽ�Ʈ
#define MESSAGE_DRONE_PLAYER_HEAL 11		//�÷��̾� ü��ȸ�� �ؽ�Ʈ


//Camera Message
#define MESSAGE_CAMERA_CINEMATIC 0							//�ó׸�ƽ ����
#define MESSAGE_CAMERA_CINEMATIC_GAME_START 1				//�ó׸�ƽ ����
#define MESSAGE_CAMERA_CINEMATIC_BOSS_START 2				//�ó׸�ƽ ����
#define MESSAGE_CAMERA_CINEMATIC_BOSS_END 3					//�ó׸�ƽ ����
#define MESSAGE_CAMERA_CINEMATIC_GAME_END 4					//�ó׸�ƽ ����
#define MESSAGE_CAMERA_CINEMATIC_TITLE 5					//�ó׸�ƽ ����

#define MESSAGE_CAMERA_CHANGE_PLAYER 6						//�÷��̾� ī�޶�� ��ȯ	//���� �� : ����
#define MESSAGE_CAMERA_CHANGE_DEBUG 7						//����� ī�޶�� ��ȯ	//���� �� : ����

//Global Message
#define MESSAGE_GLOBAL_GAMESTART 0							//������ ���۵Ǿ���
#define MESSAGE_GLOBAL_GAMEEND	1							//������ ����Ǿ���
#define MESSAGE_GLOBAL_TITLE 2								//Ÿ��Ʋ
#define MESSAGE_GLOBAL_OPTION 3								//�ɼ�
#define MESSAGE_GLOBAL_PAUSE 4								//�Ͻ�����
#define MESSAGE_GLOBAL_RESUME 5								//���Ӻ���
#define MESSAGE_GLOBAL_MANUAL 6								//���۹�
#define MESSAGE_GLOBAL_STORE 7								//����
#define MESSAGE_GLOBAL_CREDIT 8								//���� ũ����


#define MONSTER_COLOR_RED 0
#define MONSTER_COLOR_BLUE 1
#define MONSTER_TYPE_A 0
#define MONSTER_TYPE_B 1

enum class MONSTER_STATE : int
{
	IDLE = 0,	//���
	ATTACK = 1,	//����
	HIT = 2,	//���� ����
	MOVE = 3,	//�̵�
	CHASE = 4,	//�߰�
	DEAD = 5,	//����
};

enum class BOSS_STATE : int
{
	//����
	GROGGY_START = 0,		//�׷α� ���� ���� �ܰ�
	GROGGY_PLAY = 1,		//�׷α� ���� �߰� �ܰ�
	GROGGY_END = 2,			//�׷α� ���� �� �ܰ�
	IDLE = 3,				//���
	DEAD = 4,				//����
	CLOSER_ATTACK_L = 5,	//���� ���� ����
	CLOSER_ATTACK_R = 6,	//���� ���� ������

	CHASE_ATTACK_READY = 7,	//���� �߻�ü �غ�
	CHASE_ATTACK_PLAY = 8,	//���� �߻�ü �߻�
	CHASE_ATTACK_END = 9,	//���� �߻�ü ��

	RENDOM_ATTACK_START		= 10,	//������ �߻�ü �غ�
	RENDOM_ATTACK_PLAY		= 11,	//������ �߻�ü �߻�
	RENDOM_ATTACK_END		= 12,		//������ �߻�ü �߻�
	RENDOM_ATTACK_RESET		= 13,	//������ �߻�ü �߻�
	RENDOM_ATTACK_ADDSKILL	= 14,	//������ �߻�ü �߻�

	TELEPORT_READY = 15,	//���� �����̵� �غ�
	TELEPORT_START = 16,	//���� �����̵� 

	CREATE_FRIEND	= 17,	//�н� ��ȯ
	HIT				= 18,	//�¾�����
	PHASE_UP_START	= 19,	//�������
	PHASE_UP_END	= 20,	//�������

	MISSILE_START	=21,
	MISSILE_PLAY	=22,
	MISSILE_END		=23,


	RENDOM_ATTACK_BIG = 24,
};

enum class PLAYER_STATE : int
{
	IDLE = 0,
	ATTACK_01 = 1,
	ATTACK_02 = 2,
	SKILL_01 = 3,
	SKILL_02 = 4,
	JUMP = 5,
	MOVE = 6,
	DEAD = 7
};

enum class MONSTER_COLOR : int
{
	RED,
	GREEN,
	YELLOW,
	BLUE,
};
struct MONSTER_EMAGIN
{
	float R = 255.f;
	float G = 255.f;
	float B = 255.f;
	int ComboCount = 7;
	int HP;
	int MaxHP;
	int Type;
	void* Object;
};

