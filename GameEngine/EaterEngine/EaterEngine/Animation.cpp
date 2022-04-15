#include "Animation.h"
#include "EngineData.h"
#include "AnimationManager.h"
#include "GraphicsEngine.h"

#define SAFE_RELEASE(x) { if(x != nullptr){ x->Release(); delete x; x = nullptr; } }

Animation::Animation()
{
	// Animation Buffer 생성..
	m_AnimationData = new AnimationBuffer();
	m_AnimationData->Name = "None";

	// Animation 등록..
	AnimationManager::PushAnimation(this);

	// Animation Graphic 측 등록..
	GraphicEngine::Get()->PushAnimation(m_AnimationData);
}

Animation::~Animation()
{
	// Manager 내부에 있는 해당 Animation Data 삭제..
	AnimationManager::DeleteAnimation(m_AnimationData->BufferIndex);
}

void Animation::Release()
{
	// Graphic 내부에 있는 해당 Animation Buffer 삭제..
	///GraphicEngine::Get()->DeleteAnimation(m_AnimationData);

	// 해당 Animation Data 해제..
	SAFE_DELETE(m_AnimationData);
}
