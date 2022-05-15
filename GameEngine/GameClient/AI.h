#pragma once

/// <summary>
/// �ܺο��� ������ ���۳�Ʈ�� ����� ������ Ȯ���ϱ����� �׽�Ʈ ���۳�Ʈ 
/// </summary>

#include "Component.h"

class Transform;
class Rigidbody;
class MeshFilter;
class AnimationController;
class PhysRayCast;
class UnitNet;
class Camera;


class AI : public Component
{
public:
	AI();
	virtual~AI();

	virtual void Awake();
	virtual void SetUp();
	virtual void Update();

	void KeyInputMove();
private:
	Transform*				mTransform;
	Rigidbody*				mRigidbody;
	MeshFilter*				mMeshFilter;
	AnimationController*	mAnimationController;
	GameObject*				mObject;

	bool isJump;
	bool isMove;
	bool isGround;
	
	float Keyinput_Right	= 0;
	float keyinput_Up		= 0;
	float Keyinput_Angle	= 0;
	float Speed;
	float Grvity = 0;
};