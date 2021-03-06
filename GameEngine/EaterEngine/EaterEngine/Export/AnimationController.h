#pragma once
#include "Component.h"
#include "EaterEngineDLL.h"
#include <vector>
#include <string>

/// <summary>
/// Animator들을 관리하는 클래스
/// 최상위 오브젝트에만 생성될것
/// </summary>

namespace ParserData
{
	class CModelAnimation;
}

class AnimationBuffer;
class AnimationData;
class Animation;
class Animator;
class AnimationController : public Component
{
public:
	EATER_ENGINEDLL AnimationController();
	EATER_ENGINEDLL virtual ~AnimationController();

	virtual void StartUpdate();
	virtual void Update();

	EATER_ENGINEDLL void Choice(std::string Name,float Speed =1 ,bool Loop = true);//선택
	
	EATER_ENGINEDLL void Play();	//재생
	EATER_ENGINEDLL void Stop();	//멈춤
	EATER_ENGINEDLL void Pause();	//중지
	EATER_ENGINEDLL void SetFrame(int index);
	EATER_ENGINEDLL void SetIsBoneUpdate(bool enable);

	EATER_ENGINEDLL int GetNowFrame();					//선택한 애니메이션 프레임
	EATER_ENGINEDLL int GetEndFrame();					//선택한 애니메이션 끝 프레임
	EATER_ENGINEDLL float GetFrameTime();				//선택한 애니메이션 시간
	EATER_ENGINEDLL int GetAnimationCount();
	EATER_ENGINEDLL void GetAnimationList(std::vector<std::string>* NameList);
	EATER_ENGINEDLL std::string GetNowAnimationName();	//현재 애니메이션의 이름
	EATER_ENGINEDLL bool EventCheck();	//현재 애니메이션의 이름


	void SetSkin(GameObject* obj);
	void SetBoneList(std::vector<GameObject*>* mobjList);
	void SetAnimation(Animation* animation);

private:
	void ChangeAnime();
	void AnimationFrameIndex();
	void Reset();

	//본들의 애니메이터 리스트
	std::vector<Animator*> AnimatorList;

	//스킨 게임 오브젝트
	std::vector<GameObject*> SkinObject;

	//한개의 모델에 들어있는 애니메이션 리스트
	Animation* mAnimation;

	AnimationBuffer* NowAnimationBuffer;
	AnimationBuffer* NextAnimationBuffer;
	AnimationData* mAnimationData;

	ParserData::CModelAnimation* NowAnimation;

	//현재 애니메이션 이름
	std::string NowAnimationName;

	bool ChangeAnimation;

	bool	IsBoneUpdate;
	bool	mStop;
	bool	mLoop;
	bool	mPlay;

	int		mPrevFrame;
	int		mNowFrame;
	int		mNextFrame;

	float	mSpeed;
	float	mPlayTime;		//애니메이션을 한바퀴 도는 시간 
	float 	mTime;			//현재 재생타임
	float 	mFrameTime;		//현재 프레임 재생타임

};

