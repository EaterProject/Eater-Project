#pragma once
#include <Component.h>
#include <string.h>

class Camera;
class PlayerCamera;
class CinematicCamera;

class CameraManager : public Component
{
public:
	CameraManager();
	virtual ~CameraManager();

	void Awake() override;
	void Start() override;
	void Update() override;

	void SetPlayerCamera(GameObject* Obj);
	void Change(int Type);

	void SetCinematic(int Type, std::string Data);
	void SetCinematic(int Type, bool* Data = nullptr);
	void CinematicSkip();

	void SetMouseFix(bool Active);

private:
	void GameStartCinematic();
	void GameEndCinematic();
	void BossStartCinematic();
	void BossEndCinematic();
	void TitleCinematic();

private:
	bool* Finish_Play;
	bool IsCinematic_Play = false;
	bool IsGameStart_Play = false;

	int CinematicType = 0;
	PlayerCamera* mPlayerCamera			= nullptr;
	CinematicCamera* mCinematicCamera	= nullptr;
	Camera* mMain		= nullptr;
	Camera* mDebug		= nullptr;
	Camera* mPlayer		= nullptr;
};

