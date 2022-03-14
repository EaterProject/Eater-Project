#pragma once

#include <map>
#include <vector>
#include "SimpleMath.h"

struct ManaData
{
	// 날짜별 마나석이 생성 될 개수.
	std::map<int, int> Mana_Spawn_Count;
	// 날짜별 마나석의 포지션 정보
	std::map<int, DirectX::SimpleMath::Vector3> Mana_Position_Day_1;
	std::map<int, DirectX::SimpleMath::Vector3> Mana_Position_Day_2;
	std::map<int, DirectX::SimpleMath::Vector3> Mana_Position_Day_3;
	std::map<int, DirectX::SimpleMath::Vector3> Mana_Position_Day_4;
};

struct EnemyData
{
	// 날짜별 적이 생성 될 개수.
	std::map<int, int> Enemy_Spawn_Count;
	// 날짜별 적의 포지션 정보
	std::map<int, DirectX::SimpleMath::Vector3> Enemy_Position_Day_1;
	std::map<int, DirectX::SimpleMath::Vector3> Enemy_Position_Day_2;
	std::map<int, DirectX::SimpleMath::Vector3> Enemy_Position_Day_3;
	std::map<int, DirectX::SimpleMath::Vector3> Enemy_Position_Day_4;
};

struct CharacterData
{
	unsigned short					Type = 0;			// 캐릭터 타입
	float							Speed = 0.f;		// 해당 캐릭터 속도
};

struct PlayerData
{
	unsigned short					Type = 0;			// 플레이어 타입
	DirectX::SimpleMath::Vector3	Position;			// 해당 플레이어의 시작 위치
};

struct Player_Info
{
	CharacterData*				Character = nullptr;	// 해당 유저의 캐릭터
	PlayerData*					Player = nullptr;		// 해당 유저가 몇번 플레이어인가? (1P,2P .. )
};
