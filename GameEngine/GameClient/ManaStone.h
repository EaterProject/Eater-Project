#pragma once
#include "ClientComponent.h"
class MeshFilter;
class Transform;
class GameObject;
class AnimationController;

class ManaStone :public ClientComponent
{
public:
	ManaStone();
	~ManaStone();
	void Awake();
	void SetUp();
	void Update();
	void ManaStoneUpdate(Vector3 Pos);
	// ClientComponent��(��) ���� ��ӵ�
	void ReSet() override;
private:
	MeshFilter*				mMeshFilter;
	Transform*				mTransform;
	AnimationController*	mAnimation;

};

