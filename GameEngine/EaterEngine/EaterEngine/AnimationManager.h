#pragma once
#include <windows.h>
#include <vector>
#include <unordered_map>

class GraphicEngineManager;
class Animation;

class AnimationManager
{
public:
	AnimationManager();
	~AnimationManager();
	
public:
	void Initialize(GraphicEngineManager* Graphic, CRITICAL_SECTION* _cs);
	void Release();

	static void PushAnimation(Animation* animation);
	static void DeleteAnimation(UINT index);

	void BakeAnimation();

private:
	static std::vector<std::pair<UINT, bool>> g_IndexList;
	static std::unordered_map<UINT, Animation*> g_AnimationList;

	GraphicEngineManager* m_Graphic;
	CRITICAL_SECTION* m_CriticalSection;
};

