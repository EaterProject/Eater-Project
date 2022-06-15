#pragma once
#include <Component.h>
class GameObject;
class Transform;
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
private:
	void Open(int _1, int _2);
	void Close(int _1, int _2);

	GameObject* Door[6] = { nullptr };

	bool IS_OPEN = false;
	bool IS_CLOSE = false;


	float OpenTime = 0.0f;
	float CloseTime = 0.0f;

	Transform* DoorTR[4] = { nullptr };
};

