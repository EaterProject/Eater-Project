#pragma once
#include "Component.h"
#include "EngineData.h"
#include <vector>
/// <summary>
/// 본에 애니메이션을 돌리기위해 본마다 들어있는 컨퍼넌트 
/// </summary>

namespace ParserData
{
	class CAnimation;
}

class Transform;
class Animator : public Component
{
public:
	Animator();
	virtual ~Animator();

	//애니메이션을 넣어준다
	void SetAnimation(ParserData::CAnimation*);

	//매개변수로 받은 시간만큼 애니메이션한개를 재생시킴
	void Play(int animeIndex, float Speed = 1, bool Loop = true);
	void ChoiceAnime(ParserData::CAnimation* Anime);

private:
	//1초에 애니메이션 한개가 돌아가야할떄 프레임시간
	float GetOnePlayTime(float mPlayTime,int EndFrameCount);

	//애니메이션 재생 리스트
	std::vector<ParserData::CAnimation*> AnimeData;

	//현재 애니메이션 
	ParserData::CAnimation* NowAnimationData;


	DirectX::XMMATRIX XM_ROT;
	DirectX::XMMATRIX XM_POS;
	DirectX::XMMATRIX XM_SCL;

	Transform* transfrom;
};