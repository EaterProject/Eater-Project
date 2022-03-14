#include "LoginManager.h"
#include "DHNetWorkAPI.h"
#include "DHDB.h"
#include "DHTimer.h"

LoginManager::LoginManager()
{
	InitializeCriticalSection(&g_CS);

	m_Timer = new DHTimer;
	m_Timer->SetTime(5);

	m_DB = new DHDB();
	m_DB->ConnectDB(SERVER_CONNECT_IP, "CDH", "ehxk2Rnfwoa!", "LOGIN", DATABASE_PORT);

	// 일정주기로 접속되어있는 런쳐에게 정보를 보내고, 접속을 체크하는 로직
	m_Keep_Alive_Thread = new std::thread(std::bind(&LoginManager::SendUserState, this));

	// 게임런처와의 정보교환을 위한 네트워크 (서버)
	m_Connect_Launcher = new DHNetWorkAPI();
	m_Connect_Launcher->Initialize(DHNetWork_Name::DHNet);
	m_Connect_Launcher->Accept(LOGIN_SERVER_PORT, 100, 5);
}

LoginManager::~LoginManager()
{

}

void LoginManager::LauncherSideLogic()
{
	while (true)
	{
		if (m_Connect_Launcher->Recv(Launcher_Msg_Vec))
		{
			for (auto& Msg_Packet : Launcher_Msg_Vec)
			{
				// 클라이언트의 소켓 번호
				SOCKET _Recv_Socket_Num = Msg_Packet.Socket;
				// 클라이언트가 보낸 메세지
				C2S_Packet* C2S_Msg = static_cast<C2S_Packet*>(Msg_Packet.Packet);
				// 클라이언트가 강제종료한 경우
				if (C2S_Msg == NULL)
				{
					continue;
				}

				if (C2S_Msg->Packet_Type == C2S_LOGIN_SAFE_REQ)
				{
					// 로그인 인증절차.
					LoginCertify(_Recv_Socket_Num, C2S_Msg);
				}
				else if (C2S_Msg->Packet_Type == C2S_CREATE_USER_REQ)
				{
					// 아이디 생성에 대한 요청.
					CreateUser(_Recv_Socket_Num, C2S_Msg);
				}
				else if (C2S_Msg->Packet_Type == C2S_ADD_FRIEND)
				{
					// 친구 추가 요청에 대한 처리
					AddFriend(_Recv_Socket_Num, C2S_Msg);
				}
				else if (C2S_Msg->Packet_Type == C2S_ACCPET_FRIEND)
				{
					// 친구 수락 요청에 대한 처리
					AcceptFriend(_Recv_Socket_Num, C2S_Msg);
				}
				else if (C2S_Msg->Packet_Type == C2S_PLAY_STATE)
				{
					// 게임을 시작한 경우 (상태를 변화 시켜야 함)
					StartGame(_Recv_Socket_Num, C2S_Msg);
				}

				// 사용한데이터 해제.
				delete Msg_Packet.Packet;
				Msg_Packet.Packet = nullptr;
			}
			Launcher_Msg_Vec.clear();
		}

		Sleep(0);
	}
}

void LoginManager::SendUserState()
{
	std::vector<flatbuffers::Offset<Eater::LoginLauncher::UserState>> _friend_state_list;
	std::vector<flatbuffers::Offset<flatbuffers::String>> _friend_request_list;

	while (true)
	{
		m_Timer->Update();

		// 설정한시간 이 되었을경우
		if (m_Timer->Ready_Frame())
		{
			/// 추후 친구 요청 구현시.. DB에서 정보를 업데이트해줘야 될 부분.
			// m_DB->GetFriendList() ...

			EnterCriticalSection(&g_CS);
			// 한번이라도 접속한 유저들에 대해서 친구정보를 업데이트 해준다.
			for (auto& _User_Data : Logged_In_User_Data)
			{
				// 유저의 친구들에 대해서 상태를 체크한다.
				for (const auto _User_Friend : _User_Data.second->Friend_List)
				{
					auto _Friend_Name = KeepAlive_Builder.CreateString(_User_Friend);

					// 친구들이 접속한적이 있는지 검사
					auto _Friend_Data = Logged_In_User_Data.find(_User_Friend);

					// 만약 친구가 접속한적이 없는경우
					if (_Friend_Data == Logged_In_User_Data.end())
					{
						// 해당 친구는 오프라인으로 체크해서 친구목록에 등록한다.
						auto _Friend_State = Eater::LoginLauncher::CreateUserState(KeepAlive_Builder, USER_OFFLINE, _Friend_Name);
						_friend_state_list.push_back(_Friend_State);
					}
					// 한번이라도 접속한 친구인 경우
					else
					{
						// 현재 상태로 유저의 상태로 친구목록에 등록한다.
						auto _Friend_State = Eater::LoginLauncher::CreateUserState(KeepAlive_Builder, _Friend_Data->second->User_State, _Friend_Name);
						_friend_state_list.push_back(_Friend_State);
					}
				}

				// 유저의 친구요청 데이터에 대하여 보내줌.
				for (const auto _User_Friend_Request : _User_Data.second->Friend_Request_List)
				{
					auto _Friend_Name = KeepAlive_Builder.CreateString(_User_Friend_Request);

					// 해당 친구는 오프라인으로 체크해서 친구목록에 등록한다.
					_friend_request_list.push_back(_Friend_Name);
				}

				// 데이터를 정리해서 런처에 보내준다.
				auto Send_RealTimeData = CreateRealTimeDataDirect(KeepAlive_Builder, &_friend_state_list, &_friend_request_list);

				KeepAlive_Builder.Finish(Send_RealTimeData);

				KeepAlive_Send_Packet.Packet_Type = S2C_CURRENT_USER_STATE;
				KeepAlive_Send_Packet.Packet_Size = KeepAlive_Builder.GetSize();
				memcpy_s(KeepAlive_Send_Packet.Packet_Buffer, KeepAlive_Builder.GetSize(), KeepAlive_Builder.GetBufferPointer(), KeepAlive_Builder.GetSize());

				// Send 실패시 연결이 끊켰으므로 유저의 정보를 업데이트해준다.
				if (m_Connect_Launcher->Send(&KeepAlive_Send_Packet, SEND_TYPE_TARGET, _User_Data.second->Socket_Num) == false)
				{
					_User_Data.second->Socket_Num = INVALID_SOCKET;
					_User_Data.second->User_State = USER_OFFLINE;
				}		

				// 사용한 데이터 재사용을 위한 정리.
				KeepAlive_Builder.Clear();
				_friend_state_list.clear();
				_friend_request_list.clear();
			}

			/// 추후 친구요청 데이터 구현시, 이부분에서 추가해서 같이 보내준다.
			//...
			LeaveCriticalSection(&g_CS);

		}

		Sleep(0);
	}
}

void LoginManager::LoginCertify(SOCKET _Socket_Num, C2S_Packet* _C2S_Msg)
{
	// 로그인 성공여부.
	bool Is_Login_Success = false;

	// 받은 데이터를 flatbuffer 에 맞게 캐스팅
	const uint8_t* Recv_Data_Ptr = (unsigned char*)_C2S_Msg->Packet_Buffer;
	const auto Recv_LoginReqData = flatbuffers::GetRoot<Eater::LoginLauncher::LoginReqData>(Recv_Data_Ptr);

	// 클라이언트에서 로그인 시도한 아이디와 패스워드.
	auto Login_ID = Recv_LoginReqData->id()->str();
	auto Login_Password = Recv_LoginReqData->password()->str();

	// 서버에서 클라이언트에게 보낼 메세지 타입
	Launcher_Send_Packet.Packet_Type = S2C_LOGIN_SAFE_RES;
	flatbuffers::Offset<Eater::LoginLauncher::LoginResData> Send_LoginResData;

	// 이미 로그인이 되어있는경우에는 이미 로그인되었다는 정보를 알려준다.
	if (Logged_In_User_Data.find(Login_ID) != Logged_In_User_Data.end())
	{
		if (Logged_In_User_Data[Login_ID]->Socket_Num != INVALID_SOCKET)
		{
			Send_LoginResData = Eater::LoginLauncher::CreateLoginResData(Launcher_Builder, LOGIN_ALREADY_FAIL, 0);

			// 데이터 직렬화
			Launcher_Builder.Finish(Send_LoginResData);
			Launcher_Send_Packet.Packet_Size = Launcher_Builder.GetSize();
			memcpy_s(Launcher_Send_Packet.Packet_Buffer, Launcher_Builder.GetSize(), Launcher_Builder.GetBufferPointer(), Launcher_Builder.GetSize());

			// 해당 패킷을 보냄.
			m_Connect_Launcher->Send(&Launcher_Send_Packet, SEND_TYPE_TARGET, _Socket_Num);

			Launcher_Builder.Clear();
			return;
		}
	}

	if (m_DB->SearchID(Login_ID))
	{
		// 아이디 O 패스워드 O (success)
		if (m_DB->ComparePassword(Login_ID, Login_Password))
		{
			// 보낼 데이터를 만든다. ( 로그인 결과와 해당하는 아이디에 대한 식별값을 같이 보내준다. )
			Send_LoginResData = Eater::LoginLauncher::CreateLoginResData(Launcher_Builder, LOGIN_SUCCESS, m_DB->GetIdentifier(Login_ID));
			Is_Login_Success = true;	// 로그인성공
		}
		// 아이디 O 패스워드 X
		else
		{
			Send_LoginResData = Eater::LoginLauncher::CreateLoginResData(Launcher_Builder, LOGIN_PASSWORD_FAIL, 0);
		}
	}
	// 아이디 X 패드워드 X
	else
	{
		Send_LoginResData = Eater::LoginLauncher::CreateLoginResData(Launcher_Builder, LOGIN_ID_FAIL, 0);
	}

	// 데이터 직렬화
	Launcher_Builder.Finish(Send_LoginResData);
	Launcher_Send_Packet.Packet_Size = Launcher_Builder.GetSize();
	memcpy_s(Launcher_Send_Packet.Packet_Buffer, Launcher_Builder.GetSize(), Launcher_Builder.GetBufferPointer(), Launcher_Builder.GetSize());

	// 해당 패킷을 보냄.
	m_Connect_Launcher->Send(&Launcher_Send_Packet, SEND_TYPE_TARGET, _Socket_Num);

	Launcher_Builder.Clear();

	// 로그인이 성공한다면 로그인서버 정보에 등록한다.
	if (Is_Login_Success)
	{
		auto User_Init = Logged_In_User_Data.find(Login_ID);
		// 존재하지 않는 유저라면
		if (User_Init == Logged_In_User_Data.end())
		{
			// 유저 의 아이디와 소켓번호 그리고 친구목록을 등록한다.
			User_Data* _User_Data = new User_Data;
			_User_Data->Socket_Num.exchange(_Socket_Num);
			_User_Data->User_State.exchange(USER_ONLINE);
			m_DB->GetFriendList(Login_ID, _User_Data->Friend_List, _User_Data->Friend_Request_List);

			EnterCriticalSection(&g_CS);
			Logged_In_User_Data.insert({ Login_ID, _User_Data });
			LeaveCriticalSection(&g_CS);
		}
		else
		{
			// 이미 존재하는 유저라면 (해당 유저의 소켓정보를 업데이트 해준다.)
			User_Init->second->Socket_Num.exchange(_Socket_Num);
			User_Init->second->User_State.exchange(USER_ONLINE);
		}
	}
}

void LoginManager::CreateUser(SOCKET _Socket_Num, C2S_Packet* _C2S_Msg)
{
	// 받은 데이터를 flatbuffer 에 맞게 캐스팅
	const uint8_t* Recv_Data_Ptr = (unsigned char*)_C2S_Msg->Packet_Buffer;
	const auto Recv_CreateUserData = flatbuffers::GetRoot<Eater::LoginLauncher::CreateUser>(Recv_Data_Ptr);

	// 클라이언트에서 로그인 시도한 아이디와 패스워드.
	auto User_ID = Recv_CreateUserData->id()->str();
	auto User_Password = Recv_CreateUserData->password()->str();

	// DB에 유저 정보가 존재하는지 조회한다.
	if (!m_DB->SearchID(User_ID))
	{
		if (m_DB->CreateNewAccount(User_ID, User_Password))
		{
			// 성공적으로 아이디를 생성했을 경우
			Launcher_Send_Packet.Packet_Type = S2C_CREATE_USER_RES;
			Launcher_Send_Packet.Packet_Buffer[0] = true;
			Launcher_Send_Packet.Packet_Size = 1;

			// 해당 패킷을 보냄.
			m_Connect_Launcher->Send(&Launcher_Send_Packet, SEND_TYPE_TARGET, _Socket_Num);
			return;
		}
	}

	// 아이디가 정상적인 생성이 되지 않았으면 실패했다고 알려줌.
	Launcher_Send_Packet.Packet_Type = S2C_CREATE_USER_RES;
	Launcher_Send_Packet.Packet_Buffer[0] = false;
	Launcher_Send_Packet.Packet_Size = 1;

	// 해당 패킷을 보냄.
	m_Connect_Launcher->Send(&Launcher_Send_Packet, SEND_TYPE_TARGET, _Socket_Num);
	return;
}

void LoginManager::AddFriend(SOCKET _Socket_Num, C2S_Packet* _C2S_Msg)
{
	// 받은 데이터를 flatbuffer 에 맞게 캐스팅
	const uint8_t* Recv_Data_Ptr = (unsigned char*)_C2S_Msg->Packet_Buffer;
	const auto Recv_StartGame = flatbuffers::GetRoot<Eater::LoginLauncher::AddFriend>(Recv_Data_Ptr);
	User_Data* _User_Data = nullptr;

	auto _User_ID = Recv_StartGame->id()->str();
	auto _Friend_Name = Recv_StartGame->name()->str();

	_User_Data = LoginUserCheck(_Socket_Num, _User_ID);

	// 만약 로그인 된 유저가 아닌경우
	if (_User_Data == nullptr)	return;

	// 이미 친구가 되어있는 사람인지?
	auto _Find_Friend_List = _User_Data->Friend_List.find(_Friend_Name);

	// 이미 친구가 되어있다면 return
	if (_Find_Friend_List != _User_Data->Friend_List.end())
	{
		unsigned short _Reason = ALREADY_FRIEND;
		// 아이디가 정상적인 생성이 되지 않았으면 실패했다고 알려줌.
		Launcher_Send_Packet.Packet_Type = S2C_RESULT_MSG;
		memcpy_s(Launcher_Send_Packet.Packet_Buffer, 2, &_Reason, 2);
		Launcher_Send_Packet.Packet_Size = 2;

		// 해당 패킷을 보냄.
		m_Connect_Launcher->Send(&Launcher_Send_Packet, SEND_TYPE_TARGET, _Socket_Num);
		return;
	}

	// 친구 요청 리스트에 이미 들어있는지?
	auto _Find_Friend_Req_List = _User_Data->Friend_Request_List.find(_Friend_Name);

	// 만약 이미 친구요청을 보낸 경우 보낸 상대가 온라인 상태라면 해당 유저의 요청리스트에 추가해 줌.
	if (_Find_Friend_Req_List == _User_Data->Friend_Request_List.end())
	{
		// 친구요청을 DB에 등록
		m_DB->FriendRequest(_User_ID, _Friend_Name);

		auto _Friend_Online = Logged_In_User_Data.find(_Friend_Name);

		// 해당 친구가 온라인인 경우
		if (_Friend_Online != Logged_In_User_Data.end())
		{
			EnterCriticalSection(&g_CS);
			_Friend_Online->second->Friend_Request_List.insert({ _User_ID });
			LeaveCriticalSection(&g_CS);
		}
	}
}

void LoginManager::AcceptFriend(SOCKET _Socket_Num, C2S_Packet* _C2S_Msg)
{
	// 받은 데이터를 flatbuffer 에 맞게 캐스팅
	const uint8_t* Recv_Data_Ptr = (unsigned char*)_C2S_Msg->Packet_Buffer;
	const auto Recv_StartGame = flatbuffers::GetRoot<Eater::LoginLauncher::AcceptFriend>(Recv_Data_Ptr);
	User_Data* _User_Data = nullptr;

	auto _User_ID = Recv_StartGame->id()->str();
	auto _Friend_Name = Recv_StartGame->name()->str();
	auto _Accept_Result = Recv_StartGame->isaccept();
	_User_Data = LoginUserCheck(_Socket_Num, _User_ID);

	// 만약 로그인 된 유저가 아닌경우
	if (_User_Data == nullptr)	return;

	// 친구 요청 리스트에 이미 들어있는지?
	// 만약 이미 친구로 등록된 경우..
	if (_User_Data->Friend_Request_List.find(_Friend_Name) != _User_Data->Friend_Request_List.end())
	{
		EnterCriticalSection(&g_CS);
		_User_Data->Friend_Request_List.erase(_Friend_Name);
		LeaveCriticalSection(&g_CS);
	}

	// 이미 친구가 되어있는 사람인지?
	auto _Find_Friend_List = _User_Data->Friend_List.find(_Friend_Name);

	// 이미 친구가 되어있다면 return
	if (_Find_Friend_List != _User_Data->Friend_List.end())
	{
		unsigned short _Reason = ALREADY_FRIEND;
		// 아이디가 정상적인 생성이 되지 않았으면 실패했다고 알려줌.
		Launcher_Send_Packet.Packet_Type = S2C_RESULT_MSG;
		memcpy_s(Launcher_Send_Packet.Packet_Buffer, 2, &_Reason, 2);
		Launcher_Send_Packet.Packet_Size = 2;

		// 해당 패킷을 보냄.
		m_Connect_Launcher->Send(&Launcher_Send_Packet, SEND_TYPE_TARGET, _Socket_Num);
		return;
	}

	// 유저의 수락결과에 따라 처리.
	// 수락한 경우
	auto _Friend_Data = Logged_In_User_Data.find(_Friend_Name);

	if (_Accept_Result)
	{
		EnterCriticalSection(&g_CS);

		// 만약 친구도 로그인서버에 접속해 있다면, 해당 친구에게도 친구로 추가되었음을 등록해야한다.
		if (_Friend_Data != Logged_In_User_Data.end())
		{
			// 만약 이미 친구로 등록된 경우..
			if (_Friend_Data->second->Friend_Request_List.find(_User_ID) != _Friend_Data->second->Friend_Request_List.end())
			{
				_Friend_Data->second->Friend_Request_List.erase(_User_ID);
			}

			_Friend_Data->second->Friend_List.insert({_User_ID});
		}

		_User_Data->Friend_List.insert({ _Friend_Name });

		LeaveCriticalSection(&g_CS);
	}
	// 친구요청을 수락하지 않은 경우
	else
	{
		// 만약 친구도 로그인서버에 접속해 있다면, 해당 친구에게도 친구로 추가되었음을 등록해야한다.
		if (_Friend_Data != Logged_In_User_Data.end())
		{
			// 만약 이미 친구로 등록된 경우..
			if (_Friend_Data->second->Friend_Request_List.find(_User_ID) != _Friend_Data->second->Friend_Request_List.end())
			{
				EnterCriticalSection(&g_CS);
				_Friend_Data->second->Friend_Request_List.erase(_User_ID);
				LeaveCriticalSection(&g_CS);
			}
		}
	}

	// DB 데이터도 수정.
	m_DB->FriendAccept(_User_ID, _Friend_Name, _Accept_Result);
}

void LoginManager::StartGame(SOCKET _Socket_Num, C2S_Packet* _C2S_Msg)
{
	// 받은 데이터를 flatbuffer 에 맞게 캐스팅
	const uint8_t* Recv_Data_Ptr = (unsigned char*)_C2S_Msg->Packet_Buffer;
	const auto Recv_StartGame = flatbuffers::GetRoot<Eater::LoginLauncher::PlayState>(Recv_Data_Ptr);
	User_Data* _User_Data = nullptr;

	auto _User_ID = Recv_StartGame->id()->str();
	auto _In_Game = Recv_StartGame->state();
	_User_Data = LoginUserCheck(_Socket_Num, _User_ID);

	// 만약 로그인 된 유저가 아닌경우
	if (_User_Data == nullptr)	return;

	if (_In_Game)
	{
		// 게임을 시작했다고 바꿔준다. (상태 업데이트)
		_User_Data->User_State.store(USER_IN_GAME);
	}
	else
	{
		_User_Data->User_State.store(USER_ONLINE);
	}
}

User_Data* LoginManager::LoginUserCheck(SOCKET _Socket_Num, std::string& _Name)
{
	auto _Find_Result = Logged_In_User_Data.find(_Name);

	// 현재 로그인이 되어있는 유저가 아닌 경우 (강제로 클라이언트를 종료시키고 로그인부터 하게 해야함)
	if (_Find_Result == Logged_In_User_Data.end() || _Find_Result->second->Socket_Num == INVALID_SOCKET)
	{
		unsigned short _Reason = QUIT_NOT_LOGIN;
		// 아이디가 정상적인 생성이 되지 않았으면 실패했다고 알려줌.
		Launcher_Send_Packet.Packet_Type = S2C_RESULT_MSG;
		memcpy_s(Launcher_Send_Packet.Packet_Buffer, 2, &_Reason, 2);
		Launcher_Send_Packet.Packet_Size = 2;

		// 해당 패킷을 보냄.
		m_Connect_Launcher->Send(&Launcher_Send_Packet, SEND_TYPE_TARGET, _Socket_Num);
		return nullptr;
	}

	return _Find_Result->second;
}
