#pragma once
#include "Scene.h"
class ParticleSystem;
class GameObject;
class AnimationController;
class Loading : public Scene
{
public:
	// Scene��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void End() override;
	virtual void ThreadFunction();
private:
	ParticleSystem* particles;
	GameObject* obj;
	GameObject* ParticleObj;
	AnimationController* AC;
};

