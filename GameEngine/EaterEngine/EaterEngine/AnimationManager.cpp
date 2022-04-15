#include "AnimationManager.h"
#include "EngineData.h"
#include "Animation.h"
#include "LoadManager.h"
#include "GraphicEngineManager.h"

#include "Profiler/Profiler.h"

#define SAFE_RELEASE(x) { if(x != nullptr){ x->Release(); delete x; x = nullptr; } }

std::vector<std::pair<UINT, bool>>		AnimationManager::g_IndexList;
std::unordered_map<UINT, Animation*>	AnimationManager::g_AnimationList;

AnimationManager::AnimationManager()
{

}

AnimationManager::~AnimationManager()
{

}

void AnimationManager::Initialize(GraphicEngineManager* Graphic, CRITICAL_SECTION* _cs)
{
	m_Graphic = Graphic;
	m_CriticalSection = _cs;
}

void AnimationManager::Release()
{

}

void AnimationManager::PushAnimation(Animation* animation)
{
	// 추가된 Animation Index 부여..
	UINT animation_Index = 0;

	for (int i = 0; i < g_IndexList.size(); i++)
	{
		// Index List에 빈곳이 있다면 해당 Index 부여..
		if (g_IndexList[i].second == false)
		{
			animation_Index = g_IndexList[i].first;
			g_IndexList[i].second = true;
			break;
		}
	}

	// 만약 Index List에 빈곳이 없다면 다음 Index 추가..
	if (animation_Index == 0)
	{
		animation_Index = (UINT)g_IndexList.size();
		g_IndexList.push_back(std::pair<UINT, bool>(animation_Index, true));
	}

	// 현재 Animation Index 설정..
	animation->m_AnimationData->BufferIndex = animation_Index;

	// Animation List 추가..
	g_AnimationList.insert(std::make_pair(animation_Index, animation));
}

void AnimationManager::DeleteAnimation(UINT index)
{
	// 해당 Mesh 검색..
	Animation* animation = g_AnimationList.find(index)->second;

	// 해당 Mesh Data 삭제..
	animation->Release();
	g_AnimationList.erase(index);

	// 해당 Mesh Index 빈곳으로 설정..
	g_IndexList[index].second = false;
}

void AnimationManager::BakeAnimation()
{
	AnimationBuffer* animation = nullptr;

	ModelData* modelData = nullptr;
	ModelAnimationData* aniData = nullptr;

	// 로드된 모든 Model Animation Data를 Baking 한다..
	for (auto& modelAnimation : LoadManager::AnimationDataList)
	{
		std::string modelName = modelAnimation.first;

		aniData = modelAnimation.second;
		modelData = LoadManager::GetModelData(modelName);

		EnterCriticalSection(m_CriticalSection);
		m_Graphic->CreateAnimationBuffer(modelData, aniData, &animation);
		LeaveCriticalSection(m_CriticalSection);

		if (animation == nullptr)
		{
			PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ Engine ][ Bake ][ Animation Buffer ] '%s' FAILED!!", modelName.c_str());
		}
		else
		{
			animation->Name = modelName;

			Animation* Buffer = new Animation();
			Buffer->Name = modelName;

			LoadManager::AnimationList.insert({ modelName, Buffer });
		}
	}
}
