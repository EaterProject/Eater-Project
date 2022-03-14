#pragma once

/*
	2022/01/11 17:04 - CDH
	
	< 변경사항 >
	1. GameServer(Client), Lobby(Server) 간의 패킷 정의.

*/

// 유저의 상태
#define USER_OFFLINE				0b0000
#define USER_ONLINE					0b0010
#define USER_IN_LOBBY				0b0100
#define USER_IN_GAME				0b1000

// Character Type
#define EATER_CHARACTER_01			10000
#define EATER_CHARACTER_02			10001

// Player Identifier
#define EATER_PLAYER_01				20000
#define EATER_PLAYER_02				20001
#define EATER_PLAYER_03				20002
#define EATER_PLAYER_04				20003

// Client To Server
#define C2S_CONNECT_PORT_REQ		2001

// Server To Client
#define S2C_MATCHING_SUCCESS		1501
#define S2C_CONNECT_PORT_RES		2501
