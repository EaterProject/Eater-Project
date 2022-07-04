#include "AnimationManager.h"
#include "EngineData.h"
#include "Animation.h"
#include "LoadManager.h"
#include "GraphicEngineManager.h"

#include "Profiler/Profiler.h"

#define SAFE_RELEASE(x) { if(x != nullptr){ x->Release(); delete x; x = nullptr; } }

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

void AnimationManager::BakeAnimation()
{
	// 로드된 모든 Model Animation Data를 Baking 한다..

	for (auto& modelAnimation : LoadManager::AnimationList)
	{
		Animation* animation = modelAnimation.second;

		if (animation->m_AnimationBuffer->AnimationCount == animation->m_AnimationData->AnimationCount) continue;

		ModelData* modelData = LoadManager::GetModelData(modelAnimation.first);

		if (modelData == nullptr) continue;

		// 새로운 Animation 생성..
		EnterCriticalSection(m_CriticalSection);
		m_Graphic->CreateAnimationBuffer(modelData, animation->m_AnimationData, &animation->m_AnimationBuffer);
		LeaveCriticalSection(m_CriticalSection);

		if (animation->m_AnimationBuffer == nullptr)
		{
			PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ Engine ][ Bake ][ Animation Buffer ] '%s' FAILED!!", modelAnimation.first.c_str());
		}
		else
		{
			ResetAnimationIndex(animation);

			EnterCriticalSection(m_CriticalSection);
			m_Graphic->PushChangeAnimation(animation->m_AnimationBuffer);
			LeaveCriticalSection(m_CriticalSection);
		}
	}
}

void AnimationManager::ResetAnimationIndex(Animation* animation)
{
	int animation_Index = 0;

	for (auto& model_animation : animation->m_AnimationData->AnimList)
	{
		model_animation.second->m_Index = animation_Index++;
	}
}
