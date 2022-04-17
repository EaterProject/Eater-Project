#include "Animation.h"
#include "EngineData.h"
#include "AnimationManager.h"
#include "GraphicsEngine.h"

#define SAFE_RELEASE(x) { if(x != nullptr){ x->Release(); delete x; x = nullptr; } }

Animation::Animation()
{
	// Animation Buffer ����..
	m_AnimationBuffer = new AnimationBuffer();
	m_AnimationBuffer->Name = "None";

	// Animation ���..
	AnimationManager::PushAnimation(this);

	// Animation Graphic �� ���..
	GraphicEngine::Get()->PushAnimation(m_AnimationBuffer);
}

Animation::~Animation()
{
	// Manager ���ο� �ִ� �ش� Animation Data ����..
	AnimationManager::DeleteAnimation(m_AnimationBuffer->BufferIndex);
}

void Animation::Release()
{
	// Graphic ���ο� �ִ� �ش� Animation Buffer ����..
	GraphicEngine::Get()->DeleteAnimation(m_AnimationBuffer);

	// �ش� Animation Data ����..
	SAFE_DELETE(m_AnimationBuffer);
}

void Animation::SetAnimation(ModelAnimationData* animation)
{
	m_AnimationData = animation;
}
