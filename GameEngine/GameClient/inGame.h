#pragma once
#include "Scene.h"
class GameLogic;
class InGame : public Scene
{
public:
	InGame() = default;
	~InGame()= default;

	virtual void Awake();
	virtual void Update();
	virtual void End();
public:
	GameLogic* Logic;
};