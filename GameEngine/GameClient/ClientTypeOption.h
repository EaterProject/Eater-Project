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

//Player Message
#define MESSAGE_PLAYER_HIT    0		//플레이어가 맞았다
#define MESSAGE_PLAYER_HILL   1		//플레이어 에게 힐을 준다
#define MESSAGE_PLAYER_ATTACK_OK 2	//플레이어 공격이 성공하였다

//Gate Message
#define MESSAGE_GATE_OPEN  0		//게이트 열기			//인자 값 : int
#define MESSAGE_GATE_CLOSE 1		//게이트 닫기			//인자 값 : int
#define MESSAGE_GATE_UNLOCK 2		//게이트 잠금 풀기		//인자 값 : int

//UI Message
#define MESSAGE_UI_COMBO 0			//콤보 폰트 값			//인자 값 : int
#define MESSAGE_UI_HP_NOW 1			//체력 현재 폰트 값		//인자 값 : int
#define MESSAGE_UI_HP_MAX 2			//체력 최대 폰트 값		//인자 값 : int
#define MESSAGE_UI_EMAGIN_NOW 3		//이메진 현재 폰트 값		//인자 값 : int
#define MESSAGE_UI_EMAGIN_MAX 4		//이메진 최대 폰트 값		//인자 값 : int
#define MESSAGE_UI_MONSTER_UI_ON 5	//몬스터 UI를 보여준다	//인자 값 : MONSTER_EMAGIN 구조체
#define MESSAGE_UI_RENDER 6			//모든 UI 렌더링 여부		//인자 값 : bool

//Camera Message
#define MESSAGE_CAMERA_CINEMATIC_GAME_START 0				//시네마틱 실행
#define MESSAGE_CAMERA_CINEMATIC_BOSS_START 1				//시네마틱 실행
#define MESSAGE_CAMERA_CINEMATIC_BOSS_END 2					//시네마틱 실행
#define MESSAGE_CAMERA_CINEMATIC_GAME_END 3					//시네마틱 실행
#define MESSAGE_CAMERA_CHANGE_PLAYER 4						//플레이어 카메라로 변환	//인자 값 : 없음
#define MESSAGE_CAMERA_CHANGE_DEBUG 5						//디버그 카메라로 변환	//인자 값 : 없음

//Global Message
#define MESSAGE_GLOBAL_GAMESTART 0							//게임이 시작되었다
#define MESSAGE_GLOBAL_GAMEEND	1							//게임이 종료되었다


enum class MONSTER_STATE : int
{
	IDLE	= 0,	//대기
	ATTACK	= 1,	//공격
	HIT		= 2,	//공격 당함
	MOVE	= 3,	//이동
	CHASE	= 4,	//추격
	DEAD	= 5,	//죽음
};

enum class BOSS_STATE : int
{
	//상태
	GROGGY_START		= 0,	//그로기 상태 시작 단계
	GROGGY_PLAY			= 1,	//그로기 상태 중간 단계
	GROGGY_END			= 2,	//그로기 상태 끝 단계
	IDLE				= 3,	//대기
	DEAD				= 4,	//죽음
	CLOSER_ATTACK_L		= 5,	//근접 공격 왼쪽
	CLOSER_ATTACK_R		= 6,	//근접 공격 오른쪽
	CHASE_ATTACK_READY	= 7,	//추적 발사체 준비
	CHASE_ATTACK_PLAY	= 8,	//추적 발사체 발사
	RANDOM_ATTACK_READY	= 9,	//장판형 발사체 준비
	RANDOM_ATTACK_PLAY	= 10,	//장판형 발사체 발사
	TELEPORT_READY		= 11,	//보스 순간이동 준비
	TELEPORT_START		= 12,	//보스 순간이동 
	CREATE_FRIEND		= 13,	//분신 소환
	HIT					= 14,	//맞았을떄
	RENDOM_ATTACK_END   = 15,	//
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

