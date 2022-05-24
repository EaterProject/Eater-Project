#include "Animation.h"
#include "EngineData.h"
#include "AnimationManager.h"
#include "GraphicEngineAPI.h"

#define SAFE_RELEASE(x) { if(x != nullptr){ x->Release(); delete x; x = nullptr; } }

Animation::Animation()
{
	// Animation Buffer 생성..
	m_AnimationBuffer = new AnimationBuffer();
	m_AnimationBuffer->Name = "None";

	m_AnimationData = new ModelAnimationData();
	m_AnimationData->Name = "None";

	// Animation 등록..
	AnimationManager::SetIndex(&m_AnimationBuffer->BufferIndex);

	// Animation Graphic 측 등록..
	GraphicEngine::Get()->PushAnimation(m_AnimationBuffer);
}

Animation::~Animation()
{
	// Manager 내부에 있는 해당 Animation Data 삭제..
	AnimationManager::DeleteIndex(m_AnimationBuffer->BufferIndex);

	// 해당 Resource 제거..
	Release();
}

void Animation::BakeAnimation()
{

}

void Animation::Release()
{
	// Graphic 내부에 있는 해당 Animation Buffer 삭제..
	GraphicEngine::Get()->DeleteAnimation(m_AnimationBuffer);

	// 해당 Animation Data 해제..
	SAFE_DELETE(m_AnimationBuffer);
}

int Animation::GetAnimationCount()
{
	return (int)m_AnimationData->AnimList.size();
}
