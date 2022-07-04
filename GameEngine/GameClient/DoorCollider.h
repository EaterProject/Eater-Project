#pragma once
#include <Component.h>
class Collider;
class MeshFilter;
class Transform;
class DoorCollider :public Component
{
public:
	DoorCollider();
	virtual ~DoorCollider();

	void Awake() override;
	void SetUp() override;
	void Debug() override;
	void SetGateNumber(int num);

	void OnTriggerEnter(GameObject* Obj);
	void OnTriggerStay(GameObject* Obj);
	void OnTriggerExit(GameObject* Obj);



	bool Lock = false;
private:
	Collider* mCollider = nullptr;
	MeshFilter* mMeshFilter = nullptr;
	Transform* mTransform = nullptr;

	int PlayerLightIndex = -1;
	int PlayerTag = 0;
	int GateNumber = 0;

	bool isStart = false;

	float PlayerDistance;
	float BlendFactor;
	Vector3 Zero = Vector3(0.0f, 0.0f, 0.0f);

	GameObject* GroundCollider;
};

