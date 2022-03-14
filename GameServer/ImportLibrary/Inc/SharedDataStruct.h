#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "NetWorkDefine.h"

/*
	2021/10/22 07:21 - CDH

	< 변경사항 >
		1. NetWorkStruct와 Packet 헤더를 하나로 합침.

	2021/11/22 11:24 - CDH

	< 변경사항 >
		1. FlatBuffer 에 대응하는 메세지 타입 재정의.
		2. 데이터 구조의 변경.

	2021/11/24 10:47 - CDH

	< 변경사항 >
		1. 큰 데이터를 받을경우에 대한 구조체 선언.

	2021/12/10 11:15 - CDH

	< 변경사항 >
		1. 큰 데이터에 대한 구조체를 삭제하고 데이터 구조체에 대한 정의를 최소화 함.
		2. 1. 번의 사항을 실행하기 위해 Overlapped 구조체에 Big Data에 대하여 받은 데이터의 크기를 저장할 변수 추가.

*/

///////////////////////////////////////////////////////////////////////////
///					Socket & Overlapped Struct 정의						///
///////////////////////////////////////////////////////////////////////////

//////////////////////////////////
//	Socket & Overlapped Struct	//
//////////////////////////////////

struct Socket_Struct
{
	Socket_Struct()
	{
		// Placement new 를 사용하기 위해 생성자에서 m_Socket 초기화.
		m_Socket = INVALID_SOCKET;
		Is_Available = false;
	}

	~Socket_Struct()
	{
		// 소멸자를 호출하는 시점에서, 소켓이 연결되어있으면 해당 소켓의 해제를 진행.
		if (INVALID_SOCKET != m_Socket)
		{
			shutdown(m_Socket, SD_BOTH);
			closesocket(m_Socket);
		}
	}

	SOCKET m_Socket;			// 연결된 소켓
	std::string IP;				// 연결 된 곳의 IP
	unsigned short PORT = 0;	// 연결 된 곳의 PORT
	bool Is_Available;			// 현재 사용가능한 상태인가?
};

// 기본 오버랩드인 WSAOVERLAPPED 를 상속받는 Overlapped Struct
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

	/// Overlapped I/O의 작업 종류.
	/// 비동기 처리를 위해서 Type을 지정해두고, 해당 IOCP에 대한 처리를 진행한다.
	enum class IOType
	{
		IOType_Accept,
		// 소켓의 재사용을 위해 DisconnectEx 를 사용.
		IOType_Disconnect,
		IOType_Recv,
		IOType_Send,
		IOType_None,		// 아무것도 지정되지 않은 상태
	};

	IOType			m_IOType;						// 처리결과를 통보받은 후 작업을 구분하기 위해.
	SOCKET			m_Socket;						// 오버랩드의 대상이되는 소켓
	WSABUF			m_WSABUF;						// WSAbuffer의 중복 생성을 막기위해 한 오버랩드에 하나의 버퍼를 할당해둔다.
	size_t			m_Data_Size;					// 처리해야하는 데이터의 양
	char			m_Buffer[OVERLAPPED_BUFIZE];	// Send/Recv 버퍼

	/// 예를들어 버퍼사이즈가 100인데, 사이즈가 60인 데이터가 동시에 두개 들어오면 100 / 20으로 나눠 들어오기 때문에, 60을 처리하고 40을 저장해둔 후, 다음 20 패킷과 같이 합쳐서 처리해야한다.
	size_t			m_Processing_Packet_Size;						// 이전 오버랩드에서 받았던 패킷의 양
	char			m_Processing_Packet_Buffer[MAX_PACKET_SIZE];	// 이전 오버랩드에서 받아온 데이터정보.

	/// Big Data가 올 경우 처리된 패킷의 사이즈를 기록하기 위한 용도.
	size_t			m_Processed_Packet_Size;
};

//////////////////////////////////////////////////////////////////////////
///			클라이언트 to 서버 패킷들의 타입을 정의한다.				   ///
//////////////////////////////////////////////////////////////////////////

/// 사용할 패킷 구조체들 1바이트로 정렬을 한다.
#pragma pack(push , 1)

struct C2S_Packet : public Packet_Header
{
	C2S_Packet()
	{
		// 패킷의 총 사이즈는, 현재 C2S_Packet 구조체의 크기이다.
		Packet_Size = sizeof(*this) - sizeof(Packet_Size);
		Packet_Type = PACKET_TYPE_NONE;
	}

	// 서버로 보낼 데이터를 담을 Packet_Buffer 로 구성되어있음.
	char Packet_Buffer[MAX_PACKET_SIZE] = { 0, };
};

//////////////////////////////////////////////////////////////////////////
///			서버 to 클라이언트 패킷들의 타입을 정의한다.				   ///
//////////////////////////////////////////////////////////////////////////

struct S2C_Packet : public Packet_Header
{
	S2C_Packet()
	{
		// 패킷의 총 사이즈는, 현재 C2S_Packet 구조체의 크기이다.
		Packet_Size = sizeof(*this) - sizeof(Packet_Size);
		Packet_Type = PACKET_TYPE_NONE;
	}

	// 데이터를 담을 Packet_Buffer 로 구성되어있음.
	char Packet_Buffer[MAX_PACKET_SIZE]	= { 0, };
};

/// 1바이트 정렬 끝.
#pragma pack(pop)