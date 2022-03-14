#pragma once
#include "Scene.h"

class GameObject;
class Transform;
class MeshFilter;
class Rigidbody;
class AnimationController;
class ParticleSystem;
class Camera;

class TestScene : public Scene
{
public:
	TestScene() = default;
	~TestScene() = default;

public:
	void Awake() override;
	void Update() override;
	void End() override;

	void CreateTestObject();
	void CreateMap();
	void CreateParticle(float x, float y, float z);
	void ChangeCubeMap();

private:
	GameObject* testobj;
	GameObject* ParticleObj;

};

