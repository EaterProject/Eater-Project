#pragma once
#include "NetworkComponent.h"
#include <queue>
#include "PlayerData.h"

class AnimationController;
class Transform;
class MeshFilter;
class Camera;

class FrameData;
class PlayerData;
class Player :public NetworkComponent
{
public:
	Player();
	virtual ~Player();
	void Awake();
	void SetUp();
	void Update();

	Transform* mTransform;
	float Speed;
public:
	///��Ʈ��ũ ������ �޾ƿ���
	void SetNetworkPlayerData(PlayerData Data);		//��ġ�� �޾ƿ���
	void SetClientNumber(int ClientNumber,int CharacterNumber);	//�����Ҷ� �ʿ��� ������
private:
	void SEND_Player_Keyinput();
	//�÷��̾� Ű��ǲ
	void PlayerKeyinput();
	void Player01_KeyInput();
	void Player02_KeyInput();
	void UserDataSetting();
	void PlayerRotation(Vector3 Dir, float RotationSpeed);

	//������ ������ ����
	FrameData* GetFrameData();
	void SetFrameData(FrameData* Data);
	void SettingFrameData(int CreateFrameDataCount);
	void DeleteFrameData();
private:
	///���۳�Ʈ
	AnimationController*	mAnimation;
	MeshFilter*				mMeshFilter;
	Transform*				mCameraTR;
private:
	Vector3 DirPos;			//����

	bool isClientKeyinput;	//Ű ��ǲ�� �Ͼ 
	int mClientNumber;		//Ŭ���̾�Ʈ �ѹ�
	int mCharacterNumber;	//ĳ���� �ѹ�
	int _Current_Sequence;	//������ �ѹ�
private:
	///���� ��ġ��
	Vector3 PastPosition;	//������ ��ġ��
	Vector3 NowPosition;	//���� �������� ���� ��ġ��

	double _Interpolation;	
	double ServerDelayTime;	//�������� �����͸� �ִ� ���� �ð�
	double _RTT;
	
	std::queue<FrameData*> FrameQueue;
	std::vector<FrameData*> FrameList;
	PlayerData UserData;

	int AnimationType;		//�ִϸ��̼� Ÿ���� �����ϱ�����
};