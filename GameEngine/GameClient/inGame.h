#pragma once
#include "Scene.h"
class GameLogic;
class RenderOption;
class InGame : public Scene
{
public:
	InGame() = default;
	~InGame()= default;

	virtual void Awake();
	virtual void Update();
	virtual void End();
private:
	GameLogic* Logic;
	GameObject* DebugCam;
	GameObject* MainCame;
	RenderOption* Option;
	bool IsStart = false;
};