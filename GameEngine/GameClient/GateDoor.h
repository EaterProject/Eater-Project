#pragma once
#include <Component.h>
class GameObject;
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

	bool IS_OPEN;
	bool IS_CLOSE;


	GameObject* UP_Front;
	GameObject* UP_Back;
	GameObject* Down_Front;
	GameObject* Down_Back;
};

