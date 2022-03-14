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
	///네트워크 데이터 받아오기
	void SetNetworkPlayerData(PlayerData Data);		//위치값 받아오기
	void SetClientNumber(int ClientNumber,int CharacterNumber);	//생성할때 필요한 데이터
private:
	void SEND_Player_Keyinput();
	//플레이어 키인풋
	void PlayerKeyinput();
	void Player01_KeyInput();
	void Player02_KeyInput();
	void UserDataSetting();
	void PlayerRotation(Vector3 Dir, float RotationSpeed);

	//프레임 데이터 관리
	FrameData* GetFrameData();
	void SetFrameData(FrameData* Data);
	void SettingFrameData(int CreateFrameDataCount);
	void DeleteFrameData();
private:
	///컨퍼넌트
	AnimationController*	mAnimation;
	MeshFilter*				mMeshFilter;
	Transform*				mCameraTR;
private:
	Vector3 DirPos;			//방향

	bool isClientKeyinput;	//키 인풋이 일어남 
	int mClientNumber;		//클라이언트 넘버
	int mCharacterNumber;	//캐릭터 넘버
	int _Current_Sequence;	//시퀀스 넘버
private:
	///상대방 위치값
	Vector3 PastPosition;	//과거의 위치값
	Vector3 NowPosition;	//현재 서버에서 받은 위치값

	double _Interpolation;	
	double ServerDelayTime;	//서버에서 데이터를 주는 간격 시간
	double _RTT;
	
	std::queue<FrameData*> FrameQueue;
	std::vector<FrameData*> FrameList;
	PlayerData UserData;

	int AnimationType;		//애니메이션 타입을 저장하기위해
};