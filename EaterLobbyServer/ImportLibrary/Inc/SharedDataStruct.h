#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "NetWorkDefine.h"

/*
	2021/10/22 07:21 - CDH

	< ������� >
		1. NetWorkStruct�� Packet ����� �ϳ��� ��ħ.

	2021/11/22 11:24 - CDH

	< ������� >
		1. FlatBuffer �� �����ϴ� �޼��� Ÿ�� ������.
		2. ������ ������ ����.

	2021/11/24 10:47 - CDH

	< ������� >
		1. ū �����͸� ������쿡 ���� ����ü ����.

	2021/12/10 11:15 - CDH

	< ������� >
		1. ū �����Ϳ� ���� ����ü�� �����ϰ� ������ ����ü�� ���� ���Ǹ� �ּ�ȭ ��.
		2. 1. ���� ������ �����ϱ� ���� Overlapped ����ü�� Big Data�� ���Ͽ� ���� �������� ũ�⸦ ������ ���� �߰�.

*/

///////////////////////////////////////////////////////////////////////////
///					Socket & Overlapped Struct ����						///
///////////////////////////////////////////////////////////////////////////

//////////////////////////////////
//	Socket & Overlapped Struct	//
//////////////////////////////////

struct Socket_Struct
{
	Socket_Struct()
	{
		// Placement new �� ����ϱ� ���� �����ڿ��� m_Socket �ʱ�ȭ.
		m_Socket = INVALID_SOCKET;
		Is_Available = false;
	}

	~Socket_Struct()
	{
		// �Ҹ��ڸ� ȣ���ϴ� ��������, ������ ����Ǿ������� �ش� ������ ������ ����.
		if (INVALID_SOCKET != m_Socket)
		{
			shutdown(m_Socket, SD_BOTH);
			closesocket(m_Socket);
		}
	}

	SOCKET m_Socket;			// ����� ����
	std::string IP;				// ���� �� ���� IP
	unsigned short PORT = 0;	// ���� �� ���� PORT
	bool Is_Available;			// ���� ��밡���� �����ΰ�?
};

// �⺻ ���������� WSAOVERLAPPED �� ��ӹ޴� Overlapped Struct
struct Overlapped_Struct : public WSAOVERLAPPED
{
	Overlapped_Struct()
	{
		hEvent = 0;
		Internal = 0;
		InternalHigh = 0;
		Offset = 0;
		OffsetHigh = 0;
		m_IOType = IOType::IOType_None;
		m_Socket = INVALID_SOCKET;
		ZeroMemory(m_Buffer, sizeof(OVERLAPPED_BUFIZE));
		ZeroMemory(m_Processing_Packet_Buffer, sizeof(MAX_PACKET_SIZE));
		m_Data_Size = 0;
		m_Processing_Packet_Size = 0;
		m_Processed_Packet_Size = 0;
		m_WSABUF.buf = m_Buffer;
		m_WSABUF.len = 0;
	}
	~Overlapped_Struct()
	{
		ZeroMemory(m_Buffer, sizeof(OVERLAPPED_BUFIZE));
		ZeroMemory(m_Processing_Packet_Buffer, sizeof(MAX_PACKET_SIZE));
	}

	/// Overlapped I/O�� �۾� ����.
	/// �񵿱� ó���� ���ؼ� Type�� �����صΰ�, �ش� IOCP�� ���� ó���� �����Ѵ�.
	enum class IOType
	{
		IOType_Accept,
		// ������ ������ ���� DisconnectEx �� ���.
		IOType_Disconnect,
		IOType_Recv,
		IOType_Send,
		IOType_None,		// �ƹ��͵� �������� ���� ����
	};

	IOType			m_IOType;						// ó������� �뺸���� �� �۾��� �����ϱ� ����.
	SOCKET			m_Socket;						// ���������� ����̵Ǵ� ����
	WSABUF			m_WSABUF;						// WSAbuffer�� �ߺ� ������ �������� �� �������忡 �ϳ��� ���۸� �Ҵ��صд�.
	size_t			m_Data_Size;					// ó���ؾ��ϴ� �������� ��
	char			m_Buffer[OVERLAPPED_BUFIZE];	// Send/Recv ����

	/// ������� ���ۻ���� 100�ε�, ����� 60�� �����Ͱ� ���ÿ� �ΰ� ������ 100 / 20���� ���� ������ ������, 60�� ó���ϰ� 40�� �����ص� ��, ���� 20 ��Ŷ�� ���� ���ļ� ó���ؾ��Ѵ�.
	size_t			m_Processing_Packet_Size;						// ���� �������忡�� �޾Ҵ� ��Ŷ�� ��
	char			m_Processing_Packet_Buffer[MAX_PACKET_SIZE];	// ���� �������忡�� �޾ƿ� ����������.

	/// Big Data�� �� ��� ó���� ��Ŷ�� ����� ����ϱ� ���� �뵵.
	size_t			m_Processed_Packet_Size;
};

//////////////////////////////////////////////////////////////////////////
///			Ŭ���̾�Ʈ to ���� ��Ŷ���� Ÿ���� �����Ѵ�.				   ///
//////////////////////////////////////////////////////////////////////////

/// ����� ��Ŷ ����ü�� 1����Ʈ�� ������ �Ѵ�.
#pragma pack(push , 1)

struct C2S_Packet : public Packet_Header
{
	C2S_Packet()
	{
		// ��Ŷ�� �� �������, ���� C2S_Packet ����ü�� ũ���̴�.
		Packet_Size = sizeof(*this) - sizeof(Packet_Size);
		Packet_Type = PACKET_TYPE_NONE;
	}

	// ������ ���� �����͸� ���� Packet_Buffer �� �����Ǿ�����.
	char Packet_Buffer[MAX_PACKET_SIZE] = { 0, };
};

//////////////////////////////////////////////////////////////////////////
///			���� to Ŭ���̾�Ʈ ��Ŷ���� Ÿ���� �����Ѵ�.				   ///
//////////////////////////////////////////////////////////////////////////

struct S2C_Packet : public Packet_Header
{
	S2C_Packet()
	{
		// ��Ŷ�� �� �������, ���� C2S_Packet ����ü�� ũ���̴�.
		Packet_Size = sizeof(*this) - sizeof(Packet_Size);
		Packet_Type = PACKET_TYPE_NONE;
	}

	// �����͸� ���� Packet_Buffer �� �����Ǿ�����.
	char Packet_Buffer[MAX_PACKET_SIZE]	= { 0, };
};

/// 1����Ʈ ���� ��.
#pragma pack(pop)