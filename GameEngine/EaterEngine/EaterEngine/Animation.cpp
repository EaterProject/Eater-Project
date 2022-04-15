#include "Animation.h"
#include "EngineData.h"
#include "AnimationManager.h"
#include "GraphicsEngine.h"

#define SAFE_RELEASE(x) { if(x != nullptr){ x->Release(); delete x; x = nullptr; } }

Animation::Animation()
{
	// Animation Buffer ����..
	m_AnimationData = new AnimationBuffer();
	m_AnimationData->Name = "None";

	// Animation ���..
	AnimationManager::PushAnimation(this);

	// Animation Graphic �� ���..
	GraphicEngine::Get()->PushAnimation(m_AnimationData);
}

Animation::~Animation()
{
	// Manager ���ο� �ִ� �ش� Animation Data ����..
	AnimationManager::DeleteAnimation(m_AnimationData->BufferIndex);
}

void Animation::Release()
{
	// Graphic ���ο� �ִ� �ش� Animation Buffer ����..
	///GraphicEngine::Get()->DeleteAnimation(m_AnimationData);

	// �ش� Animation Data ����..
	SAFE_DELETE(m_AnimationData);
}
