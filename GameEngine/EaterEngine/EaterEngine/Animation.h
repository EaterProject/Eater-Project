#pragma once
#include <string>
#include "EaterEngineDLL.h"
#include "EngineData.h"

class AnimationManager;

class Animation
{
public:
	EATER_ENGINEDLL Animation();
	~Animation();

public:
	friend class AnimationManager;

public:
	void BakeAnimation();
	void Release();

public:
	int GetAnimationCount();

public:
	std::string Name;

	ModelAnimationData* m_AnimationData;
	AnimationBuffer* m_AnimationBuffer;
};

