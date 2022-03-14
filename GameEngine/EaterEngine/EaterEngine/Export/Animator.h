#pragma once
#include "Component.h"
#include "EngineData.h"
#include <vector>
/// <summary>
/// ���� �ִϸ��̼��� ���������� ������ ����ִ� ���۳�Ʈ 
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

	//�ִϸ��̼��� �־��ش�
	void SetAnimation(ParserData::OneAnimation*);

	//�Ű������� ���� �ð���ŭ �ִϸ��̼��Ѱ��� �����Ŵ
	void Play(float Speed = 1 ,bool Loop = true);
	void Stop();
	void ReStart();
	void ChoiceAnime(ParserData::OneAnimation* Anime);


	int GetNowFrame();
	int GetEndFrame();
private:
	//1�ʿ� �ִϸ��̼� �Ѱ��� ���ư����ҋ� �����ӽð�
	float GetOnePlayTime(float mPlayTime,int EndFrameCount);
	void AnimeFrameIndex();

	//�ִϸ��̼� ��� ����Ʈ
	std::vector<ParserData::OneAnimation*> AnimeData;

	//���� �ִϸ��̼� 
	ParserData::OneAnimation* NowAnimationData;


	DirectX::XMMATRIX XM_ROT;
	DirectX::XMMATRIX XM_POS;
	DirectX::XMMATRIX XM_SCL;

	float	mTime;			//���� ���Ÿ��
	float	PlayTime;		//�ִϸ��̼��� �ѹ��� ���� �ð� 
	int		AnimeIndex;		//�ִϸ��̼� �ε�����
	bool	mLoop;			//�ݺ� ��� ����
	bool	mStop;			//���㿩��
	bool	mPlay;			//�������


	Transform* transfrom;
};