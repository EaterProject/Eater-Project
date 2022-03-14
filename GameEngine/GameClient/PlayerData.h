#pragma once

//플레이어 데이터 구조체
class PlayerData
{
public:
	PlayerData() = default;
	~PlayerData() = default;
public:
	//위치
	Vector3 Position;
	//애니메이션 타입
	int AnimationType;
	//객체 번호
	int ObjectNumber;

	double Passed_Time;

	int SequenceNumber;
};


//프레임 정보
class FrameData
{
public:
	FrameData() = default;
	~FrameData()= default;

	void Reset()
	{
		m_Sequence	= -1;
		Position	= { 0,0,0 };
		Direction	= { 0,0,0 };
		Dtime		= 0.0f;
	}

	int		m_Sequence;		//시퀀스 넘버
	Vector3 Position;		//위치 정보
	Vector3 Direction;		//방향 정보
	double	Dtime;			//현재 프레임 정보

	bool Run = false;		//이 데이터를 사용중인지 아닌지 여부
};


