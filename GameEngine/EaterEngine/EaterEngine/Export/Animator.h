#pragma once
#include "Component.h"
#include "EngineData.h"
#include <vector>
/// <summary>
/// ���� �ִϸ��̼��� ���������� ������ ����ִ� ���۳�Ʈ 
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

	//�ִϸ��̼��� �־��ش�
	void SetAnimation(ParserData::CAnimation*);

	//�Ű������� ���� �ð���ŭ �ִϸ��̼��Ѱ��� �����Ŵ
	void Play(int animeIndex, float Speed = 1, bool Loop = true);
	void ChoiceAnime(ParserData::CAnimation* Anime);

private:
	//1�ʿ� �ִϸ��̼� �Ѱ��� ���ư����ҋ� �����ӽð�
	float GetOnePlayTime(float mPlayTime,int EndFrameCount);

	//�ִϸ��̼� ��� ����Ʈ
	std::vector<ParserData::CAnimation*> AnimeData;

	//���� �ִϸ��̼� 
	ParserData::CAnimation* NowAnimationData;


	DirectX::XMMATRIX XM_ROT;
	DirectX::XMMATRIX XM_POS;
	DirectX::XMMATRIX XM_SCL;

	Transform* transfrom;
};