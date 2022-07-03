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

//Player Message
#define MESSAGE_PLAYER_HIT    0				//플레이어가 맞았다
#define MESSAGE_PLAYER_HILL   1				//플레이어 에게 힐을 준다
#define MESSAGE_PLAYER_ATTACK_OK 2			//플레이어 공격이 성공하였다
#define MESSAGE_PLAYER_ACTIVE_TRUE 3		//플레이어 Active TRUE
#define MESSAGE_PLAYER_ACTIVE_FALSE 4		//플레이어 Active FALSE
#define MESSAGE_PLAYER_COMBO_RESET 5		//플레이어 콤보 리셋
#define	MESSAGE_PLAYER_PUSH 6				//플레이어 밀림

//Gate Message
#define MESSAGE_GATE_OPEN  0				//게이트 열기			//인자 값 : int
#define MESSAGE_GATE_CLOSE 1				//게이트 닫기			//인자 값 : int
#define MESSAGE_GATE_UNLOCK 2				//게이트 잠금 풀기		//인자 값 : int
#define MESSAGE_GATE_LOCK 3					//게이트 잠금 열기		//인자 값 : int

//Store Message
#define MESSAGE_STORE_EXIT  0				//스토어 닫기
#define MESSAGE_STORE_PAUSE  1				//스토어 일시정지 상태

//UI Message
#define MESSAGE_UI_COMBO 0					//콤보 폰트 값			//인자 값 : int
#define MESSAGE_UI_HP_NOW 1					//체력 현재 폰트 값		//인자 값 : int
#define MESSAGE_UI_HP_MAX 2					//체력 최대 폰트 값		//인자 값 : int
#define MESSAGE_UI_EMAGIN_NOW 3				//이메진 현재 폰트 값		//인자 값 : int
#define MESSAGE_UI_EMAGIN_MAX 4				//이메진 최대 폰트 값		//인자 값 : int
#define MESSAGE_UI_MONSTER_UI_ON 5			//몬스터 UI를 켠다		//인자 값 : MONSTER_EMAGIN 구조체
#define MESSAGE_UI_MONSTER_UI_OFF 6			//몬스터 UI를 끈다		//인자 값 : MONSTER_EMAGIN 구조체
#define MESSAGE_UI_MONSTER_UI_UPDATE 7		//몬스터 UI를 업데이트	//인자 값 : MONSTER_EMAGIN 구조체
#define MESSAGE_UI_FADE_IN 8				//페이드 인
#define MESSAGE_UI_FADE_OUT 9				//페이드 아웃
#define MESSAGE_UI_PLAYER_ACTIVE 10			//UI 플레이어 UI
#define MESSAGE_UI_PLAYER_HIT 11			//플레이어 피격
#define MESSAGE_UI_STORE_ACTIVE	12			//UI 스토어 엑티브
#define MESSAGE_UI_BOSS_HP 13
#define MESSAGE_UI_BOSS_HP_MAX 14
#define MESSAGE_UI_BOSS_ACTIVE 15


//Camera Message
#define MESSAGE_CAMERA_CINEMATIC 0							//시네마틱 실행
#define MESSAGE_CAMERA_CINEMATIC_GAME_START 1				//시네마틱 실행
#define MESSAGE_CAMERA_CINEMATIC_BOSS_START 2				//시네마틱 실행
#define MESSAGE_CAMERA_CINEMATIC_BOSS_END 3					//시네마틱 실행
#define MESSAGE_CAMERA_CINEMATIC_GAME_END 4					//시네마틱 실행
#define MESSAGE_CAMERA_CINEMATIC_TITLE 5					//시네마틱 실행

#define MESSAGE_CAMERA_CHANGE_PLAYER 6						//플레이어 카메라로 변환	//인자 값 : 없음
#define MESSAGE_CAMERA_CHANGE_DEBUG 7						//디버그 카메라로 변환	//인자 값 : 없음

//Global Message
#define MESSAGE_GLOBAL_GAMESTART 0							//게임이 시작되었다
#define MESSAGE_GLOBAL_GAMEEND	1							//게임이 종료되었다
#define MESSAGE_GLOBAL_TITLE 2								//타이틀
#define MESSAGE_GLOBAL_OPTION 3								//옵션
#define MESSAGE_GLOBAL_PAUSE 4								//일시정지
#define MESSAGE_GLOBAL_RESUME 5								//게임복귀
#define MESSAGE_GLOBAL_MANUAL 6								//조작법
#define MESSAGE_GLOBAL_STORE 7								//상점
#define MESSAGE_GLOBAL_CREDIT 8								//엔딩 크래딧


#define MONSTER_COLOR_RED 0
#define MONSTER_COLOR_BLUE 1
#define MONSTER_TYPE_A 0
#define MONSTER_TYPE_B 1

enum class MONSTER_STATE : int
{
	IDLE = 0,	//대기
	ATTACK = 1,	//공격
	HIT = 2,	//공격 당함
	MOVE = 3,	//이동
	CHASE = 4,	//추격
	DEAD = 5,	//죽음
};

enum class BOSS_STATE : int
{
	//상태
	GROGGY_START = 0,		//그로기 상태 시작 단계
	GROGGY_PLAY = 1,		//그로기 상태 중간 단계
	GROGGY_END = 2,			//그로기 상태 끝 단계
	IDLE = 3,				//대기
	DEAD = 4,				//죽음
	CLOSER_ATTACK_L = 5,	//근접 공격 왼쪽
	CLOSER_ATTACK_R = 6,	//근접 공격 오른쪽

	CHASE_ATTACK_READY = 7,	//추적 발사체 준비
	CHASE_ATTACK_PLAY = 8,	//추적 발사체 발사
	CHASE_ATTACK_END = 9,	//추적 발사체 끝

	RENDOM_ATTACK_START		= 10,	//장판형 발사체 준비
	RENDOM_ATTACK_PLAY		= 11,	//장판형 발사체 발사
	RENDOM_ATTACK_END		= 12,		//장판형 발사체 발사
	RENDOM_ATTACK_RESET		= 13,	//장판형 발사체 발사
	RENDOM_ATTACK_ADDSKILL	= 14,	//장판형 발사체 발사

	TELEPORT_READY = 15,	//보스 순간이동 준비
	TELEPORT_START = 16,	//보스 순간이동 

	CREATE_FRIEND	= 17,	//분신 소환
	HIT				= 18,	//맞았을떄
	PHASE_UP_START	= 19,	//페이즈업
	PHASE_UP_END	= 20,	//페이즈업

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
	int Type;
	void* Object;
};

