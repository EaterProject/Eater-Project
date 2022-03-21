#include "Player.h"
#include "AnimationController.h"
#include "Transform.h"
#include "MeshFilter.h"
#include "GameObject.h"
#include "MainHeader.h"
#include "Camera.h"

#include "GameClientGameServerPacketDefine.h"
#include "PlayerData.h"
#include "WorldData_generated.h"
#include "PlayerCamera.h"

#define MAX_SEQUENCE		60000
Player::Player()
{
	mAnimation	= nullptr;
	mTransform	= nullptr;
	mMeshFilter = nullptr;

	isClientKeyinput	= false;	//키인풋 여부
	mCharacterNumber	= -1;		//캐릭터 넘버
	mClientNumber		= -1;		//클라이언트 넘버
	_Current_Sequence	= 0;		//시컨스 너버

	_RTT = 1.0f / SERVER_FREQUENCY;

	ServerDelayTime = 0.0f;
	_Interpolation	= 0.0f;

	PastPosition	= { 0,0,0 };
	NowPosition		= { 0,0,0 };

	AnimationType = ANIMATION_IDLE;
	//프레임데이터 메모리풀
	SettingFrameData(10);
}

Player::~Player()
{
	//프레임 데이터 삭제
	DeleteFrameData();
}

void Player::Awake()
{
	//컨퍼넌트 가져오기
	mTransform	= gameobject->GetTransform();
	mMeshFilter = gameobject->GetComponent<MeshFilter>();
	mAnimation	= gameobject->GetComponent<AnimationController>();
	mTransform->Scale = { 0.5f,0.5f ,0.5f };
}

void Player::SetUp()
{
	//매쉬, 애니메이션 정보 입력
	mMeshFilter->SetModelName("MOdNA");
	mMeshFilter->SetAnimationName("MOdNA");
	mAnimation->Choice("Idle");

	//사이즈 조정
	//mTransform->Scale = { 0.25f,0.25f ,0.25f };
}

void Player::Update()
{
	//서버에서 데이터를 주는 간격 시간 체크
	ServerDelayTime += GetDeltaTime();	

	//플레이어가 키입력을 눌렀는지 체크 한다
	PlayerKeyinput();

	//키인풋이 눌렸을때 메세지를 보낸다
	SEND_Player_Keyinput();

	//내가아닌 다른유저의 위치를 보정해준다
	UserDataSetting();

	DirPos = { 0,0,0 };
	mAnimation->Play(1, true);
}

void Player::SetNetworkPlayerData(PlayerData Data)
{
	//현재 내가 조정하고있는 캐릭터가 아니라면 서버에있는 데이터를 그대로받음
	if (mClientNumber != mCharacterNumber)
	{
		//데이터를 받고 위치값을 저장
		UserData = Data;
		if (mTransform != nullptr)
		{
			//위치값 조정
			PastPosition	= NowPosition;			//현재 위치를 과거의 위치로
			NowPosition		= UserData.Position;	//현재 위치는 서버에서 받은 위치로

			//데이터를 받았으니 시간 초기화
			ServerDelayTime = 0.0f;
		}
	}
	else 
	{
		while (!FrameQueue.empty())
		{
			// 큐에서 데이터를 순서대로 뺴옴
			FrameData* _FData = FrameQueue.front();
			FrameQueue.pop();
			//오차 범위 계산
			if (_FData->m_Sequence == Data.SequenceNumber)
			{
				if (abs((Data.Position.x) - _FData->Position.x) > 0.01f ||
					abs((Data.Position.y) - _FData->Position.y) > 0.01f ||
					abs((Data.Position.z) - _FData->Position.z) > 0.01f)
				{
					mTransform->Position = Data.Position;
					SetFrameData(_FData);
					break;
				}
			}
			SetFrameData(_FData);
		}
	}
}

void Player::SetClientNumber(int ClientNumber, int CharacterNumber)
{
	//클라이언트 넘버 (실행하는 컴퓨터 마다 다를것)
	if (mClientNumber == -1)
	{
		mClientNumber = ClientNumber;
	}

	//오브젝트 넘버 
	if (mCharacterNumber == -1)
	{
		mCharacterNumber = CharacterNumber;
	}

	//넘버 두개를 모두 받았으면 내가 움직일수있는 캐릭터인지 판단후 카메라를 가져옴
	if (mClientNumber == mCharacterNumber)
	{
		//플레이어가 나자신일때 카메라와 나를 연결시킴
		GameObject* MainCam = GetMainCamera();
		MainCam->GetComponent<PlayerCamera>()->Userobject = gameobject;
		mCameraTR = MainCam->GetTransform();
	}
}

void Player::SEND_Player_Keyinput()
{
	//유저의 키인풋이 있다면 메세지를 보낸다
	if (isClientKeyinput == true)
	{
		mAnimation->Choice("Run");
		FrameData* FData	= GetFrameData();		//프레임 데이터를 하나 가져온다
		FData->m_Sequence	= _Current_Sequence;	//시퀀스
		FData->Direction	= DirPos;				//방향 위치
		FData->Dtime		= GetDeltaTime();		//시간

		_Builder;
		//위치값 생성
		auto _Mov_Vec = Eater::GameData::Vec3(DirPos.x, DirPos.y, DirPos.z);
		auto _Move_Packet_Data = Eater::GameData::CreateClientMove(*_Builder,_Current_Sequence, FData->Dtime,&_Mov_Vec);
	
		if (_Current_Sequence == MAX_SEQUENCE)
		{
			_Current_Sequence = 0;
		}
		else
		{
			_Current_Sequence++;
		}
	
		//데이터 직렬화
		_Builder->Finish(_Move_Packet_Data);
	
		//네트워크로 전송
		Network_Send(_Builder, C2S_PLAYER_MOVE);
	
		//이동 계산
		mTransform->SetTranlate(Speed * DirPos * FData->Dtime);

		//현재 계산한 위치값을 넣어준다
		FData->Position = mTransform->Position;

		FrameQueue.push(FData);

		//방향 리셋
		DirPos = { 0,0,0 };
		//다음 프레임체크하기위해 false로 변경
		isClientKeyinput = false;
	}
}

void Player::PlayerKeyinput()
{
	if (mClientNumber == EATER_PLAYER_01)
	{
		if (mClientNumber == mCharacterNumber)
		{
			Player01_KeyInput();
			mTransform->Slow_Y_Rotation(DirPos, 300);
		}
	}

	if (mClientNumber == EATER_PLAYER_02)
	{
		if (mClientNumber == mCharacterNumber)
		{
			Player02_KeyInput();
			mTransform->Slow_Y_Rotation(DirPos, 300);
		}
	}
}

void Player::Player01_KeyInput()
{
	if (GetKey('D'))
	{
		isClientKeyinput = true;
		DirPos += mCameraTR->GetLocalPosition_Right();
	}

	if (GetKey('A'))
	{
		DirPos += -mCameraTR->GetLocalPosition_Right();
		isClientKeyinput = true;
	}

	if (GetKey('W'))
	{
		Vector3 Pos = mCameraTR->GetLocalPosition_Look();
		Pos.y = 0;
		DirPos += Pos;
		isClientKeyinput = true;
	}

	if (GetKey('S'))
	{
		Vector3 Pos = mCameraTR->GetLocalPosition_Look();
		Pos.y = 0;
		DirPos += -Pos;
		isClientKeyinput = true;
	}

	DirPos.Normalize();
	if (DirPos == Vector3(0, 0, 0))
	{
		mAnimation->Choice("Idle");
	}
	else
	{
		mAnimation->Choice("Run");
	}
}

void Player::Player02_KeyInput()
{
	if (GetKey(VK_RIGHT))
	{
		DirPos += mCameraTR->GetLocalPosition_Right();
		isClientKeyinput = true;
	}

	if (GetKey(VK_LEFT))
	{
		DirPos += -mCameraTR->GetLocalPosition_Right();;
		isClientKeyinput = true;
	}

	if (GetKey(VK_UP))
	{
		Vector3 Pos = mCameraTR->GetLocalPosition_Look();
		Pos.y = 0;
		DirPos += Pos;
		isClientKeyinput = true;
	}

	if (GetKey(VK_DOWN))
	{
		Vector3 Pos = mCameraTR->GetLocalPosition_Look();
		Pos.y = 0;
		DirPos -= Pos;
		isClientKeyinput = true;
	}

	DirPos.Normalize();
	if (DirPos == Vector3(0, 0, 0))
	{
		mAnimation->Choice("Idle");
	}
	else
	{
		mAnimation->Choice("Run");
	}
}

void Player::UserDataSetting()
{
	//상대방 위치를 보간한다
	if (mClientNumber != mCharacterNumber)
	{
		UserData.AnimationType;
		UserData.ObjectNumber;
		_Interpolation = ServerDelayTime / _RTT;

		switch(UserData.AnimationType)
		{
		case ANIMATION_IDLE:
			if (AnimationType != ANIMATION_IDLE)
			{
				mAnimation->Choice("Idle");
				AnimationType = ANIMATION_IDLE;
			}
			break;
		case ANIMATION_RUN:
			if (AnimationType != ANIMATION_RUN)
			{
				mAnimation->Choice("Run");
				AnimationType = ANIMATION_RUN;
			}
			break;
		}

		
		if (_Interpolation >= 1.0f)
		{
			_Interpolation = 1.0f;
		}

		//위치를 보간
		Vector3 _Interpolation_Pos;

		//현재 나의 위치에서 서버에서 받은 데이터
		_Interpolation_Pos = DirectX::SimpleMath::Vector3::Lerp(PastPosition, NowPosition,_Interpolation);
		mTransform->Position = _Interpolation_Pos;

		DirectX::SimpleMath::Vector3 _Dir_Vec = NowPosition - PastPosition;
		_Dir_Vec.Normalize();
		mTransform->Slow_Y_Rotation(_Dir_Vec, 300);
	}
}

FrameData* Player::GetFrameData()
{
	std::vector<FrameData*>::iterator it = FrameList.begin();
	for (it; it != FrameList.end(); it++)
	{
		//현재 사용중이지 않는 데이터를 가져다준다
		if((*it)->Run == false)
		{
			(*it)->Reset();
			(*it)->Run = true;
			return (*it);
		}
	}

	//여기까지 내려왔다는것은 현재 사용할데이터가없다는것
	FrameData* Data = new FrameData();
	FrameList.push_back(Data);

	Data->Reset();
	Data->Run = true;
	return Data;
}

void Player::SetFrameData(FrameData* Data)
{
	//데이터 셋팅
	Data->Reset();
	Data->Run = false;
}

void Player::SettingFrameData(int CreateFrameDataCount)
{
	//플레이어 객체가 생성될때 프레임데이터를 기본 10를 넣어준다
	for (int i = 0; i < CreateFrameDataCount; i++)
	{
		FrameList.push_back(new FrameData);
	}
}

void Player::DeleteFrameData()
{
	std::vector<FrameData*>::iterator it = FrameList.begin();
	for (it; it != FrameList.end(); ++it)
	{
		delete (*it);
		(*it) = nullptr;
	}
	FrameList.clear();
}


