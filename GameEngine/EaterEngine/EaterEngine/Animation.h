#pragma once
#include <string>
#include "EaterEngineDLL.h"

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
	std::string Name;
	AnimationBuffer* m_AnimationData;
};

