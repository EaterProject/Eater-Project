#pragma once
#include "Component.h"
#include "EaterEngineDLL.h"
#include <vector>
#include <string>

/// <summary>
/// Animator들을 관리하는 클래스
/// 최상위 오브젝트에만 생성될것
/// </summary>

class ModelAnimationData;
class Animator;
class AnimationController : public Component
{
public:
	EATER_ENGINEDLL AnimationController();
	EATER_ENGINEDLL virtual ~AnimationController();

	virtual void StartUpdate();
	virtual void Update();

	EATER_ENGINEDLL void Choice(std::string Name);		//선택
	EATER_ENGINEDLL void Play(float Speed, bool Loop);	//재생
	EATER_ENGINEDLL void Stop();						//일시정지

	EATER_ENGINEDLL int GetNowFrame();	//선택한 애니메이션 프레임
	EATER_ENGINEDLL int GetEndFrame();	//선택한 애니메이션 끝 프레임

	EATER_ENGINEDLL int GetAnimationCount();
	EATER_ENGINEDLL void GetAnimationList(std::vector<std::string>* NameList);
	EATER_ENGINEDLL std::string GetNowAnimationName();


	void SetBoneList(std::vector<GameObject*>* mobjList);
	void SetAnimeList(ModelAnimationData* data);
private:
	void ChangeAnime();

	//본들의 애니메이터 리스트
	std::vector<Animator*> AnimatorList;

	//한개의 모델에 들어있는 애니메이션 리스트
	ModelAnimationData* AnimationList;

	//현재 애니메이션 이름
	std::string NowAnimationName;

	bool ChangeAnimation;

};

