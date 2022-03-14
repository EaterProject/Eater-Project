#pragma once

/*
	2021/12/20 10:54 - CDH
	
	< 변경사항 >
		1. GameClient(Client), GameServer(Server) 간의 패킷 정의.
		
*/

// Server & Client Physics Update time
#define PHYSX_UPDATE_TIME				0.016

// Server Frequency
#define SERVER_FREQUENCY				20

// Character Type
#define EATER_CHARACTER_01				10000
#define EATER_CHARACTER_02				10001

// Player Identifier
#define EATER_PLAYER_01					20000
#define EATER_PLAYER_02					20001
#define EATER_PLAYER_03					20002
#define EATER_PLAYER_04					20003

// Animation Type
#define ANIMATION_IDLE					30000	
#define ANIMATION_RUN					30001	

// Object Type
#define EATER_OBJECT_MANA				40000

// Enemy Type
#define EATER_ENEMY_NORMAL				50000

// Client To Server
#define C2S_LOADING_COMPLETE_REQ		1
#define C2S_PLAYER_MOVE					1001
#define C2S_KEEP_ALIVE_CHECK_REQ		1002

// Server To Client
// Loop
#define S2C_WORLD_UPDATE				1501
#define S2C_KEEP_ALIVE_CHECK_RES		1502
#define S2C_MONSTER_UPDATE				1503
#define S2C_BOSS_UPDATE					1504
// Non-Loop
#define S2C_LOADING_COMPLETE_RES		501
#define S2C_START_GAME					502
