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
#define MESSAGE_PLAYER_HIT    0		//플레이어가 맞았다
#define MESSAGE_PLAYER_HILL   1		//플레이어 에게 힐을 준다
#define MESSAGE_PLAYER_ATTACK_OK 2	//플레이어 공격이 성공하였다

//Gate Message
#define MESSAGE_GATE_OPEN  0		//게이트 열기
#define MESSAGE_GATE_CLOSE 1		//게이트 닫기
#define MESSAGE_GATE_UNLOCK 2		//게이트 잠금 풀기

//UI Message
#define MESSAGE_UI_COMBO 0			//콤보 폰트 값
#define MESSAGE_UI_HP_NOW 1			//체력 현재 폰트 값
#define MESSAGE_UI_HP_MAX 2			//체력 최대 폰트 값
#define MESSAGE_UI_EMAGIN_NOW 3		//이메진 현재 폰트 값
#define MESSAGE_UI_EMAGIN_MAX 4		//이메진 최대 폰트 값
#define MESSAGE_UI_MONSTER_UI_ON 5	//몬스터 UI를 보여준다

//Camera Message
#define MESSAGE_CAMERA_START_GAME 0	//게임이 시작되었을때
#define MESSAGE_CAMERA_START_BOSS 1	//보스가 생성되었을때
#define MESSAGE_CAMERA_END_BOSS 2	//보스가 죽었을때

enum class MONSTER_STATE : int
{
	IDLE,	//대기
	ATTACK,	//공격
	HIT,	//공격 당함
	MOVE,	//이동
	CHASE,	//추격
	DEAD,	//죽음
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

