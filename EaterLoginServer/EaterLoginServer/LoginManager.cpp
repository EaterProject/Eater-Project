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

	// �����ֱ�� ���ӵǾ��ִ� ���Ŀ��� ������ ������, ������ üũ�ϴ� ����
	m_Keep_Alive_Thread = new std::thread(std::bind(&LoginManager::SendUserState, this));

	// ���ӷ�ó���� ������ȯ�� ���� ��Ʈ��ũ (����)
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
				// Ŭ���̾�Ʈ�� ���� ��ȣ
				SOCKET _Recv_Socket_Num = Msg_Packet.Socket;
				// Ŭ���̾�Ʈ�� ���� �޼���
				C2S_Packet* C2S_Msg = static_cast<C2S_Packet*>(Msg_Packet.Packet);
				// Ŭ���̾�Ʈ�� ���������� ���
				if (C2S_Msg == NULL)
				{
					continue;
				}

				if (C2S_Msg->Packet_Type == C2S_LOGIN_SAFE_REQ)
				{
					// �α��� ��������.
					LoginCertify(_Recv_Socket_Num, C2S_Msg);
				}
				else if (C2S_Msg->Packet_Type == C2S_CREATE_USER_REQ)
				{
					// ���̵� ������ ���� ��û.
					CreateUser(_Recv_Socket_Num, C2S_Msg);
				}
				else if (C2S_Msg->Packet_Type == C2S_ADD_FRIEND)
				{
					// ģ�� �߰� ��û�� ���� ó��
					AddFriend(_Recv_Socket_Num, C2S_Msg);
				}
				else if (C2S_Msg->Packet_Type == C2S_ACCPET_FRIEND)
				{
					// ģ�� ���� ��û�� ���� ó��
					AcceptFriend(_Recv_Socket_Num, C2S_Msg);
				}
				else if (C2S_Msg->Packet_Type == C2S_PLAY_STATE)
				{
					// ������ ������ ��� (���¸� ��ȭ ���Ѿ� ��)
					StartGame(_Recv_Socket_Num, C2S_Msg);
				}

				// ����ѵ����� ����.
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

		// �����ѽð� �� �Ǿ������
		if (m_Timer->Ready_Frame())
		{
			/// ���� ģ�� ��û ������.. DB���� ������ ������Ʈ����� �� �κ�.
			// m_DB->GetFriendList() ...

			EnterCriticalSection(&g_CS);
			// �ѹ��̶� ������ �����鿡 ���ؼ� ģ�������� ������Ʈ ���ش�.
			for (auto& _User_Data : Logged_In_User_Data)
			{
				// ������ ģ���鿡 ���ؼ� ���¸� üũ�Ѵ�.
				for (const auto _User_Friend : _User_Data.second->Friend_List)
				{
					auto _Friend_Name = KeepAlive_Builder.CreateString(_User_Friend);

					// ģ������ ���������� �ִ��� �˻�
					auto _Friend_Data = Logged_In_User_Data.find(_User_Friend);

					// ���� ģ���� ���������� ���°��
					if (_Friend_Data == Logged_In_User_Data.end())
					{
						// �ش� ģ���� ������������ üũ�ؼ� ģ����Ͽ� ����Ѵ�.
						auto _Friend_State = Eater::LoginLauncher::CreateUserState(KeepAlive_Builder, USER_OFFLINE, _Friend_Name);
						_friend_state_list.push_back(_Friend_State);
					}
					// �ѹ��̶� ������ ģ���� ���
					else
					{
						// ���� ���·� ������ ���·� ģ����Ͽ� ����Ѵ�.
						auto _Friend_State = Eater::LoginLauncher::CreateUserState(KeepAlive_Builder, _Friend_Data->second->User_State, _Friend_Name);
						_friend_state_list.push_back(_Friend_State);
					}
				}

				// ������ ģ����û �����Ϳ� ���Ͽ� ������.
				for (const auto _User_Friend_Request : _User_Data.second->Friend_Request_List)
				{
					auto _Friend_Name = KeepAlive_Builder.CreateString(_User_Friend_Request);

					// �ش� ģ���� ������������ üũ�ؼ� ģ����Ͽ� ����Ѵ�.
					_friend_request_list.push_back(_Friend_Name);
				}

				// �����͸� �����ؼ� ��ó�� �����ش�.
				auto Send_RealTimeData = CreateRealTimeDataDirect(KeepAlive_Builder, &_friend_state_list, &_friend_request_list);

				KeepAlive_Builder.Finish(Send_RealTimeData);

				KeepAlive_Send_Packet.Packet_Type = S2C_CURRENT_USER_STATE;
				KeepAlive_Send_Packet.Packet_Size = KeepAlive_Builder.GetSize();
				memcpy_s(KeepAlive_Send_Packet.Packet_Buffer, KeepAlive_Builder.GetSize(), KeepAlive_Builder.GetBufferPointer(), KeepAlive_Builder.GetSize());

				// Send ���н� ������ �������Ƿ� ������ ������ ������Ʈ���ش�.
				if (m_Connect_Launcher->Send(&KeepAlive_Send_Packet, SEND_TYPE_TARGET, _User_Data.second->Socket_Num) == false)
				{
					_User_Data.second->Socket_Num = INVALID_SOCKET;
					_User_Data.second->User_State = USER_OFFLINE;
				}		

				// ����� ������ ������ ���� ����.
				KeepAlive_Builder.Clear();
				_friend_state_list.clear();
				_friend_request_list.clear();
			}

			/// ���� ģ����û ������ ������, �̺κп��� �߰��ؼ� ���� �����ش�.
			//...
			LeaveCriticalSection(&g_CS);

		}

		Sleep(0);
	}
}

void LoginManager::LoginCertify(SOCKET _Socket_Num, C2S_Packet* _C2S_Msg)
{
	// �α��� ��������.
	bool Is_Login_Success = false;

	// ���� �����͸� flatbuffer �� �°� ĳ����
	const uint8_t* Recv_Data_Ptr = (unsigned char*)_C2S_Msg->Packet_Buffer;
	const auto Recv_LoginReqData = flatbuffers::GetRoot<Eater::LoginLauncher::LoginReqData>(Recv_Data_Ptr);

	// Ŭ���̾�Ʈ���� �α��� �õ��� ���̵�� �н�����.
	auto Login_ID = Recv_LoginReqData->id()->str();
	auto Login_Password = Recv_LoginReqData->password()->str();

	// �������� Ŭ���̾�Ʈ���� ���� �޼��� Ÿ��
	Launcher_Send_Packet.Packet_Type = S2C_LOGIN_SAFE_RES;
	flatbuffers::Offset<Eater::LoginLauncher::LoginResData> Send_LoginResData;

	// �̹� �α����� �Ǿ��ִ°�쿡�� �̹� �α��εǾ��ٴ� ������ �˷��ش�.
	if (Logged_In_User_Data.find(Login_ID) != Logged_In_User_Data.end())
	{
		if (Logged_In_User_Data[Login_ID]->Socket_Num != INVALID_SOCKET)
		{
			Send_LoginResData = Eater::LoginLauncher::CreateLoginResData(Launcher_Builder, LOGIN_ALREADY_FAIL, 0);

			// ������ ����ȭ
			Launcher_Builder.Finish(Send_LoginResData);
			Launcher_Send_Packet.Packet_Size = Launcher_Builder.GetSize();
			memcpy_s(Launcher_Send_Packet.Packet_Buffer, Launcher_Builder.GetSize(), Launcher_Builder.GetBufferPointer(), Launcher_Builder.GetSize());

			// �ش� ��Ŷ�� ����.
			m_Connect_Launcher->Send(&Launcher_Send_Packet, SEND_TYPE_TARGET, _Socket_Num);

			Launcher_Builder.Clear();
			return;
		}
	}

	if (m_DB->SearchID(Login_ID))
	{
		// ���̵� O �н����� O (success)
		if (m_DB->ComparePassword(Login_ID, Login_Password))
		{
			// ���� �����͸� �����. ( �α��� ����� �ش��ϴ� ���̵� ���� �ĺ����� ���� �����ش�. )
			Send_LoginResData = Eater::LoginLauncher::CreateLoginResData(Launcher_Builder, LOGIN_SUCCESS, m_DB->GetIdentifier(Login_ID));
			Is_Login_Success = true;	// �α��μ���
		}
		// ���̵� O �н����� X
		else
		{
			Send_LoginResData = Eater::LoginLauncher::CreateLoginResData(Launcher_Builder, LOGIN_PASSWORD_FAIL, 0);
		}
	}
	// ���̵� X �е���� X
	else
	{
		Send_LoginResData = Eater::LoginLauncher::CreateLoginResData(Launcher_Builder, LOGIN_ID_FAIL, 0);
	}

	// ������ ����ȭ
	Launcher_Builder.Finish(Send_LoginResData);
	Launcher_Send_Packet.Packet_Size = Launcher_Builder.GetSize();
	memcpy_s(Launcher_Send_Packet.Packet_Buffer, Launcher_Builder.GetSize(), Launcher_Builder.GetBufferPointer(), Launcher_Builder.GetSize());

	// �ش� ��Ŷ�� ����.
	m_Connect_Launcher->Send(&Launcher_Send_Packet, SEND_TYPE_TARGET, _Socket_Num);

	Launcher_Builder.Clear();

	// �α����� �����Ѵٸ� �α��μ��� ������ ����Ѵ�.
	if (Is_Login_Success)
	{
		auto User_Init = Logged_In_User_Data.find(Login_ID);
		// �������� �ʴ� �������
		if (User_Init == Logged_In_User_Data.end())
		{
			// ���� �� ���̵�� ���Ϲ�ȣ �׸��� ģ������� ����Ѵ�.
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
			// �̹� �����ϴ� ������� (�ش� ������ ���������� ������Ʈ ���ش�.)
			User_Init->second->Socket_Num.exchange(_Socket_Num);
			User_Init->second->User_State.exchange(USER_ONLINE);
		}
	}
}

void LoginManager::CreateUser(SOCKET _Socket_Num, C2S_Packet* _C2S_Msg)
{
	// ���� �����͸� flatbuffer �� �°� ĳ����
	const uint8_t* Recv_Data_Ptr = (unsigned char*)_C2S_Msg->Packet_Buffer;
	const auto Recv_CreateUserData = flatbuffers::GetRoot<Eater::LoginLauncher::CreateUser>(Recv_Data_Ptr);

	// Ŭ���̾�Ʈ���� �α��� �õ��� ���̵�� �н�����.
	auto User_ID = Recv_CreateUserData->id()->str();
	auto User_Password = Recv_CreateUserData->password()->str();

	// DB�� ���� ������ �����ϴ��� ��ȸ�Ѵ�.
	if (!m_DB->SearchID(User_ID))
	{
		if (m_DB->CreateNewAccount(User_ID, User_Password))
		{
			// ���������� ���̵� �������� ���
			Launcher_Send_Packet.Packet_Type = S2C_CREATE_USER_RES;
			Launcher_Send_Packet.Packet_Buffer[0] = true;
			Launcher_Send_Packet.Packet_Size = 1;

			// �ش� ��Ŷ�� ����.
			m_Connect_Launcher->Send(&Launcher_Send_Packet, SEND_TYPE_TARGET, _Socket_Num);
			return;
		}
	}

	// ���̵� �������� ������ ���� �ʾ����� �����ߴٰ� �˷���.
	Launcher_Send_Packet.Packet_Type = S2C_CREATE_USER_RES;
	Launcher_Send_Packet.Packet_Buffer[0] = false;
	Launcher_Send_Packet.Packet_Size = 1;

	// �ش� ��Ŷ�� ����.
	m_Connect_Launcher->Send(&Launcher_Send_Packet, SEND_TYPE_TARGET, _Socket_Num);
	return;
}

void LoginManager::AddFriend(SOCKET _Socket_Num, C2S_Packet* _C2S_Msg)
{
	// ���� �����͸� flatbuffer �� �°� ĳ����
	const uint8_t* Recv_Data_Ptr = (unsigned char*)_C2S_Msg->Packet_Buffer;
	const auto Recv_StartGame = flatbuffers::GetRoot<Eater::LoginLauncher::AddFriend>(Recv_Data_Ptr);
	User_Data* _User_Data = nullptr;

	auto _User_ID = Recv_StartGame->id()->str();
	auto _Friend_Name = Recv_StartGame->name()->str();

	_User_Data = LoginUserCheck(_Socket_Num, _User_ID);

	// ���� �α��� �� ������ �ƴѰ��
	if (_User_Data == nullptr)	return;

	// �̹� ģ���� �Ǿ��ִ� �������?
	auto _Find_Friend_List = _User_Data->Friend_List.find(_Friend_Name);

	// �̹� ģ���� �Ǿ��ִٸ� return
	if (_Find_Friend_List != _User_Data->Friend_List.end())
	{
		unsigned short _Reason = ALREADY_FRIEND;
		// ���̵� �������� ������ ���� �ʾ����� �����ߴٰ� �˷���.
		Launcher_Send_Packet.Packet_Type = S2C_RESULT_MSG;
		memcpy_s(Launcher_Send_Packet.Packet_Buffer, 2, &_Reason, 2);
		Launcher_Send_Packet.Packet_Size = 2;

		// �ش� ��Ŷ�� ����.
		m_Connect_Launcher->Send(&Launcher_Send_Packet, SEND_TYPE_TARGET, _Socket_Num);
		return;
	}

	// ģ�� ��û ����Ʈ�� �̹� ����ִ���?
	auto _Find_Friend_Req_List = _User_Data->Friend_Request_List.find(_Friend_Name);

	// ���� �̹� ģ����û�� ���� ��� ���� ��밡 �¶��� ���¶�� �ش� ������ ��û����Ʈ�� �߰��� ��.
	if (_Find_Friend_Req_List == _User_Data->Friend_Request_List.end())
	{
		// ģ����û�� DB�� ���
		m_DB->FriendRequest(_User_ID, _Friend_Name);

		auto _Friend_Online = Logged_In_User_Data.find(_Friend_Name);

		// �ش� ģ���� �¶����� ���
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
	// ���� �����͸� flatbuffer �� �°� ĳ����
	const uint8_t* Recv_Data_Ptr = (unsigned char*)_C2S_Msg->Packet_Buffer;
	const auto Recv_StartGame = flatbuffers::GetRoot<Eater::LoginLauncher::AcceptFriend>(Recv_Data_Ptr);
	User_Data* _User_Data = nullptr;

	auto _User_ID = Recv_StartGame->id()->str();
	auto _Friend_Name = Recv_StartGame->name()->str();
	auto _Accept_Result = Recv_StartGame->isaccept();
	_User_Data = LoginUserCheck(_Socket_Num, _User_ID);

	// ���� �α��� �� ������ �ƴѰ��
	if (_User_Data == nullptr)	return;

	// ģ�� ��û ����Ʈ�� �̹� ����ִ���?
	// ���� �̹� ģ���� ��ϵ� ���..
	if (_User_Data->Friend_Request_List.find(_Friend_Name) != _User_Data->Friend_Request_List.end())
	{
		EnterCriticalSection(&g_CS);
		_User_Data->Friend_Request_List.erase(_Friend_Name);
		LeaveCriticalSection(&g_CS);
	}

	// �̹� ģ���� �Ǿ��ִ� �������?
	auto _Find_Friend_List = _User_Data->Friend_List.find(_Friend_Name);

	// �̹� ģ���� �Ǿ��ִٸ� return
	if (_Find_Friend_List != _User_Data->Friend_List.end())
	{
		unsigned short _Reason = ALREADY_FRIEND;
		// ���̵� �������� ������ ���� �ʾ����� �����ߴٰ� �˷���.
		Launcher_Send_Packet.Packet_Type = S2C_RESULT_MSG;
		memcpy_s(Launcher_Send_Packet.Packet_Buffer, 2, &_Reason, 2);
		Launcher_Send_Packet.Packet_Size = 2;

		// �ش� ��Ŷ�� ����.
		m_Connect_Launcher->Send(&Launcher_Send_Packet, SEND_TYPE_TARGET, _Socket_Num);
		return;
	}

	// ������ ��������� ���� ó��.
	// ������ ���
	auto _Friend_Data = Logged_In_User_Data.find(_Friend_Name);

	if (_Accept_Result)
	{
		EnterCriticalSection(&g_CS);

		// ���� ģ���� �α��μ����� ������ �ִٸ�, �ش� ģ�����Ե� ģ���� �߰��Ǿ����� ����ؾ��Ѵ�.
		if (_Friend_Data != Logged_In_User_Data.end())
		{
			// ���� �̹� ģ���� ��ϵ� ���..
			if (_Friend_Data->second->Friend_Request_List.find(_User_ID) != _Friend_Data->second->Friend_Request_List.end())
			{
				_Friend_Data->second->Friend_Request_List.erase(_User_ID);
			}

			_Friend_Data->second->Friend_List.insert({_User_ID});
		}

		_User_Data->Friend_List.insert({ _Friend_Name });

		LeaveCriticalSection(&g_CS);
	}
	// ģ����û�� �������� ���� ���
	else
	{
		// ���� ģ���� �α��μ����� ������ �ִٸ�, �ش� ģ�����Ե� ģ���� �߰��Ǿ����� ����ؾ��Ѵ�.
		if (_Friend_Data != Logged_In_User_Data.end())
		{
			// ���� �̹� ģ���� ��ϵ� ���..
			if (_Friend_Data->second->Friend_Request_List.find(_User_ID) != _Friend_Data->second->Friend_Request_List.end())
			{
				EnterCriticalSection(&g_CS);
				_Friend_Data->second->Friend_Request_List.erase(_User_ID);
				LeaveCriticalSection(&g_CS);
			}
		}
	}

	// DB �����͵� ����.
	m_DB->FriendAccept(_User_ID, _Friend_Name, _Accept_Result);
}

void LoginManager::StartGame(SOCKET _Socket_Num, C2S_Packet* _C2S_Msg)
{
	// ���� �����͸� flatbuffer �� �°� ĳ����
	const uint8_t* Recv_Data_Ptr = (unsigned char*)_C2S_Msg->Packet_Buffer;
	const auto Recv_StartGame = flatbuffers::GetRoot<Eater::LoginLauncher::PlayState>(Recv_Data_Ptr);
	User_Data* _User_Data = nullptr;

	auto _User_ID = Recv_StartGame->id()->str();
	auto _In_Game = Recv_StartGame->state();
	_User_Data = LoginUserCheck(_Socket_Num, _User_ID);

	// ���� �α��� �� ������ �ƴѰ��
	if (_User_Data == nullptr)	return;

	if (_In_Game)
	{
		// ������ �����ߴٰ� �ٲ��ش�. (���� ������Ʈ)
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

	// ���� �α����� �Ǿ��ִ� ������ �ƴ� ��� (������ Ŭ���̾�Ʈ�� �����Ű�� �α��κ��� �ϰ� �ؾ���)
	if (_Find_Result == Logged_In_User_Data.end() || _Find_Result->second->Socket_Num == INVALID_SOCKET)
	{
		unsigned short _Reason = QUIT_NOT_LOGIN;
		// ���̵� �������� ������ ���� �ʾ����� �����ߴٰ� �˷���.
		Launcher_Send_Packet.Packet_Type = S2C_RESULT_MSG;
		memcpy_s(Launcher_Send_Packet.Packet_Buffer, 2, &_Reason, 2);
		Launcher_Send_Packet.Packet_Size = 2;

		// �ش� ��Ŷ�� ����.
		m_Connect_Launcher->Send(&Launcher_Send_Packet, SEND_TYPE_TARGET, _Socket_Num);
		return nullptr;
	}

	return _Find_Result->second;
}
