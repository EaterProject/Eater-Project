#pragma once

/*
	2021/12/23 10:29 - CDH
	
	< ������� >
		1.  GameLauncher(Client), LoginServer(Server) ���� ��Ŷ ����.
		
*/

// ����� ó���� ���� �κ�
#define LOGIN_ID_FAIL				0b0000
#define LOGIN_PASSWORD_FAIL			0b0010
#define LOGIN_SUCCESS				0b0100
#define LOGIN_ALREADY_FAIL			0b1000

// ���� ������ ��Ÿ�� Flag��
#define USER_OFFLINE				0b0000
#define USER_ONLINE					0b0010
#define USER_IN_GAME				0b0100

#define USER_ICON_TYPE_1			0b0000
#define USER_ICON_TYPE_2			0b0001

// Client To Server
#define	C2S_LOGIN_SAFE_REQ			5001
#define	C2S_ADD_FRIEND				5002
#define	C2S_ACCPET_FRIEND			5003
#define	C2S_LOGIN_SERVER_START		5004
#define C2S_CREATE_USER_REQ			5005
#define C2S_PLAY_STATE				5006

// Server To Client
#define	S2C_CURRENT_USER_STATE		5501
#define	S2C_LOGIN_SAFE_RES			5502
#define	S2C_ADD_FRIEND_RES			5503
#define S2C_CREATE_USER_RES			5504
#define S2C_RESULT_MSG				5505

// ����� ���� �޼��� ������ ����ƴ°�?
#define QUIT_NOT_LOGIN				0b0000	// �α��� ������ ���µ� ���ӽ����� �õ��� ���
#define ALREADY_FRIEND				0b0010	// �̹� ģ���� ���
