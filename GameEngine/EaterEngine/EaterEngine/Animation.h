#pragma once
#include <string>
#include "EaterEngineDLL.h"

class ModelAnimationData;
class AnimationBuffer;
class AnimationManager;

class Animation
{
public:
	EATER_ENGINEDLL Animation();
	~Animation();

public:
	friend class AnimationManager;

public:
	void Release();

public:
	void SetAnimation(ModelAnimationData* animation);

public:
	std::string Name;

	ModelAnimationData* m_AnimationData;
	AnimationBuffer* m_AnimationBuffer;
};

