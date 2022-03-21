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

	isClientKeyinput	= false;	//Ű��ǲ ����
	mCharacterNumber	= -1;		//ĳ���� �ѹ�
	mClientNumber		= -1;		//Ŭ���̾�Ʈ �ѹ�
	_Current_Sequence	= 0;		//������ �ʹ�

	_RTT = 1.0f / SERVER_FREQUENCY;

	ServerDelayTime = 0.0f;
	_Interpolation	= 0.0f;

	PastPosition	= { 0,0,0 };
	NowPosition		= { 0,0,0 };

	AnimationType = ANIMATION_IDLE;
	//�����ӵ����� �޸�Ǯ
	SettingFrameData(10);
}

Player::~Player()
{
	//������ ������ ����
	DeleteFrameData();
}

void Player::Awake()
{
	//���۳�Ʈ ��������
	mTransform	= gameobject->GetTransform();
	mMeshFilter = gameobject->GetComponent<MeshFilter>();
	mAnimation	= gameobject->GetComponent<AnimationController>();
	mTransform->Scale = { 0.5f,0.5f ,0.5f };
}

void Player::SetUp()
{
	//�Ž�, �ִϸ��̼� ���� �Է�
	mMeshFilter->SetModelName("MOdNA");
	mMeshFilter->SetAnimationName("MOdNA");
	mAnimation->Choice("Idle");

	//������ ����
	//mTransform->Scale = { 0.25f,0.25f ,0.25f };
}

void Player::Update()
{
	//�������� �����͸� �ִ� ���� �ð� üũ
	ServerDelayTime += GetDeltaTime();	

	//�÷��̾ Ű�Է��� �������� üũ �Ѵ�
	PlayerKeyinput();

	//Ű��ǲ�� �������� �޼����� ������
	SEND_Player_Keyinput();

	//�����ƴ� �ٸ������� ��ġ�� �������ش�
	UserDataSetting();

	DirPos = { 0,0,0 };
	mAnimation->Play(1, true);
}

void Player::SetNetworkPlayerData(PlayerData Data)
{
	//���� ���� �����ϰ��ִ� ĳ���Ͱ� �ƴ϶�� �������ִ� �����͸� �״�ι���
	if (mClientNumber != mCharacterNumber)
	{
		//�����͸� �ް� ��ġ���� ����
		UserData = Data;
		if (mTransform != nullptr)
		{
			//��ġ�� ����
			PastPosition	= NowPosition;			//���� ��ġ�� ������ ��ġ��
			NowPosition		= UserData.Position;	//���� ��ġ�� �������� ���� ��ġ��

			//�����͸� �޾����� �ð� �ʱ�ȭ
			ServerDelayTime = 0.0f;
		}
	}
	else 
	{
		while (!FrameQueue.empty())
		{
			// ť���� �����͸� ������� ����
			FrameData* _FData = FrameQueue.front();
			FrameQueue.pop();
			//���� ���� ���
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
	//Ŭ���̾�Ʈ �ѹ� (�����ϴ� ��ǻ�� ���� �ٸ���)
	if (mClientNumber == -1)
	{
		mClientNumber = ClientNumber;
	}

	//������Ʈ �ѹ� 
	if (mCharacterNumber == -1)
	{
		mCharacterNumber = CharacterNumber;
	}

	//�ѹ� �ΰ��� ��� �޾����� ���� �����ϼ��ִ� ĳ�������� �Ǵ��� ī�޶� ������
	if (mClientNumber == mCharacterNumber)
	{
		//�÷��̾ ���ڽ��϶� ī�޶�� ���� �����Ŵ
		GameObject* MainCam = GetMainCamera();
		MainCam->GetComponent<PlayerCamera>()->Userobject = gameobject;
		mCameraTR = MainCam->GetTransform();
	}
}

void Player::SEND_Player_Keyinput()
{
	//������ Ű��ǲ�� �ִٸ� �޼����� ������
	if (isClientKeyinput == true)
	{
		mAnimation->Choice("Run");
		FrameData* FData	= GetFrameData();		//������ �����͸� �ϳ� �����´�
		FData->m_Sequence	= _Current_Sequence;	//������
		FData->Direction	= DirPos;				//���� ��ġ
		FData->Dtime		= GetDeltaTime();		//�ð�

		_Builder;
		//��ġ�� ����
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
	
		//������ ����ȭ
		_Builder->Finish(_Move_Packet_Data);
	
		//��Ʈ��ũ�� ����
		Network_Send(_Builder, C2S_PLAYER_MOVE);
	
		//�̵� ���
		mTransform->SetTranlate(Speed * DirPos * FData->Dtime);

		//���� ����� ��ġ���� �־��ش�
		FData->Position = mTransform->Position;

		FrameQueue.push(FData);

		//���� ����
		DirPos = { 0,0,0 };
		//���� ������üũ�ϱ����� false�� ����
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
	//���� ��ġ�� �����Ѵ�
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

		//��ġ�� ����
		Vector3 _Interpolation_Pos;

		//���� ���� ��ġ���� �������� ���� ������
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
		//���� ��������� �ʴ� �����͸� �������ش�
		if((*it)->Run == false)
		{
			(*it)->Reset();
			(*it)->Run = true;
			return (*it);
		}
	}

	//������� �����Դٴ°��� ���� ����ҵ����Ͱ����ٴ°�
	FrameData* Data = new FrameData();
	FrameList.push_back(Data);

	Data->Reset();
	Data->Run = true;
	return Data;
}

void Player::SetFrameData(FrameData* Data)
{
	//������ ����
	Data->Reset();
	Data->Run = false;
}

void Player::SettingFrameData(int CreateFrameDataCount)
{
	//�÷��̾� ��ü�� �����ɶ� �����ӵ����͸� �⺻ 10�� �־��ش�
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


