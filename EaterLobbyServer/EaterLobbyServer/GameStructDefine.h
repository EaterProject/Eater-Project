#pragma once

#include <map>
#include <vector>
#include "SimpleMath.h"

struct ManaData
{
	// ��¥�� �������� ���� �� ����.
	std::map<int, int> Mana_Spawn_Count;
	// ��¥�� �������� ������ ����
	std::map<int, DirectX::SimpleMath::Vector3> Mana_Position_Day_1;
	std::map<int, DirectX::SimpleMath::Vector3> Mana_Position_Day_2;
	std::map<int, DirectX::SimpleMath::Vector3> Mana_Position_Day_3;
	std::map<int, DirectX::SimpleMath::Vector3> Mana_Position_Day_4;
};

struct EnemyData
{
	// ��¥�� ���� ���� �� ����.
	std::map<int, int> Enemy_Spawn_Count;
	// ��¥�� ���� ������ ����
	std::map<int, DirectX::SimpleMath::Vector3> Enemy_Position_Day_1;
	std::map<int, DirectX::SimpleMath::Vector3> Enemy_Position_Day_2;
	std::map<int, DirectX::SimpleMath::Vector3> Enemy_Position_Day_3;
	std::map<int, DirectX::SimpleMath::Vector3> Enemy_Position_Day_4;
};

struct CharacterData
{
	unsigned short					Type = 0;			// ĳ���� Ÿ��
	float							Speed = 0.f;		// �ش� ĳ���� �ӵ�
};

struct PlayerData
{
	unsigned short					Type = 0;			// �÷��̾� Ÿ��
	DirectX::SimpleMath::Vector3	Position;			// �ش� �÷��̾��� ���� ��ġ
};

struct Player_Info
{
	CharacterData*				Character = nullptr;	// �ش� ������ ĳ����
	PlayerData*					Player = nullptr;		// �ش� ������ ��� �÷��̾��ΰ�? (1P,2P .. )
};
