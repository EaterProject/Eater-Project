#pragma once
#include "Component.h"
#include "EngineData.h"
#include <vector>
/// <summary>
/// 본에 애니메이션을 돌리기위해 본마다 들어있는 컨퍼넌트 
/// </summary>

namespace ParserData
{
	class OneAnimation;
}

class Transform;
class Animator : public Component
{
public:
	Animator();
	virtual ~Animator();

	//애니메이션을 넣어준다
	void SetAnimation(ParserData::OneAnimation*);

	//매개변수로 받은 시간만큼 애니메이션한개를 재생시킴
	void Play(float Speed = 1 ,bool Loop = true);
	void Stop();
	void ReStart();
	void ChoiceAnime(ParserData::OneAnimation* Anime);


	int GetNowFrame();
	int GetEndFrame();
private:
	//1초에 애니메이션 한개가 돌아가야할떄 프레임시간
	float GetOnePlayTime(float mPlayTime,int EndFrameCount);
	void AnimeFrameIndex();

	//애니메이션 재생 리스트
	std::vector<ParserData::OneAnimation*> AnimeData;

	//현재 애니메이션 
	ParserData::OneAnimation* NowAnimationData;


	DirectX::XMMATRIX XM_ROT;
	DirectX::XMMATRIX XM_POS;
	DirectX::XMMATRIX XM_SCL;

	float	mTime;			//현재 재생타임
	float	PlayTime;		//애니메이션을 한바퀴 도는 시간 
	int		AnimeIndex;		//애니메이션 인덱스값
	bool	mLoop;			//반복 재생 여부
	bool	mStop;			//멈춤여부
	bool	mPlay;			//재생여부


	Transform* transfrom;
};