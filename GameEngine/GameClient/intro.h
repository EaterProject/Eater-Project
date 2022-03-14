#pragma once
#include "Scene.h"

class GameObject;
class Transform;
class MeshFilter;
class Rigidbody;
class AnimationController;
class ParticleSystem;
class Camera;

class  intro :public Scene
{
public:
	intro() {};
	~intro() {};

	virtual void Awake() override;
	virtual void Update() override;
	virtual void End() override;
	
private:
	void CreateTerrain();
	void ChangeCubeMap();
private:
	GameObject* testobj;
	GameObject* ParticleObj;
	Rigidbody* rig;
	AnimationController* AC;

	Camera* MainCam;
	Camera* DebugCam;
};

