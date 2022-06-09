#pragma once
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