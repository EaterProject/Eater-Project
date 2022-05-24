#pragma once
#include <windows.h>
#include "IndexManager.h"

class GraphicEngineManager;

class ModelAnimationData;
class Animation;

class AnimationManager : public IndexManager
{
public:
	AnimationManager();
	~AnimationManager();
	
public:
	void Initialize(GraphicEngineManager* Graphic, CRITICAL_SECTION* _cs);

	void BakeAnimation();
	void ResetAnimationIndex(Animation* animation);

private:
	GraphicEngineManager* m_Graphic;
	CRITICAL_SECTION* m_CriticalSection;
};

