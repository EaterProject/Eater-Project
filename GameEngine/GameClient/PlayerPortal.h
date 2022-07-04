#pragma once
#include <Component.h>

class Transform;
class MeshFilter;
class AnimationController;
class PlayerCapsule :public Component
{
public:
	PlayerCapsule();
	~PlayerCapsule();

public:
	void Awake() override;
	void SetUp() override;
	void Start() override;
public:
	Transform*				mTransform	= nullptr;
	MeshFilter*				mMeshFilter = nullptr;
	AnimationController*	mAnime		= nullptr;
};

