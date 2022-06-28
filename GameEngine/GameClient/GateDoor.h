#pragma once
#include <Component.h>
#include "MeshFilterSetting.h"
class GameObject;
class Transform;
class Collider;
class DoorCollider;
class GateDoor : public Component
{
public:
	GateDoor();
	virtual ~GateDoor();

	void Awake() override;
	void Start() override;
	void Update() override;

	void SetOpen(int Number);
	void SetClose(int Number);

	void SetLock(int Number);
	void SetUnLock(int Number);
private:
	void Open(int _1, int _2);
	void Close(int _1, int _2);

	GameObject* Door[8] = { nullptr };


	GameObject* GateColliderObject[4];
	DoorCollider*	GateCollider[4];

	MeshFilterSetting DoorMesh[8];

	bool IS_OPEN = false;
	bool IS_CLOSE = false;

	float OpenTime = 0.0f;
	float CloseTime = 0.0f;

	Transform* DoorTR[4] = { nullptr };
};

