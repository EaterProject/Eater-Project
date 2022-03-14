#pragma once
#include <vector>
#include "Scene.h"
class ParticleSystem;
class AnimationController;
class InGame : public Scene
{
public:
	InGame() {};
	~InGame() {};

	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void End();

public:
	GameObject* testobj;

	GameObject* PointLightObj;
	GameObject* SpotLightObj;
	GameObject* ParticleObj;

	bool sphereRot = false;
	bool objRot = false;

	std::vector<GameObject*> SphereList;
	std::vector<GameObject*> ObjList;

	std::vector<ParticleSystem*> ParticleList;
	AnimationController* AC;
	ParticleSystem* particles;
};