#pragma once
#include <Component.h>
#include <string.h>
class Camera;
class CameraDebugKeyInput;
class CameraManager :public Component
{
public:
	CameraManager();
	virtual ~CameraManager();

	void Awake() override;
	void Update() override;

	void SetPlayerCamera(GameObject* Obj);
	void Change(int Type);

	void SetCinematic(int Type, std::string& Name);
	void SetCinematic(int Type, std::string&& Name);
private:
	bool IsCinematic_Play = false;
	bool IsGameStart_Play = false;

	int CinematicType = 0;

	Camera* mMain		= nullptr;
	Camera* mDebug		= nullptr;
	Camera* mCinematic	= nullptr;
	Camera* mPlayer		= nullptr;
};

