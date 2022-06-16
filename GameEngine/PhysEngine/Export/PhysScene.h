#pragma once
#include "PhysEngineDLL.h"
#include "SimpleMath.h"

class PhysScene
{
public:
	PhysScene();
	~PhysScene();
private:
	DirectX::SimpleMath::Vector3 Grvity;
};

