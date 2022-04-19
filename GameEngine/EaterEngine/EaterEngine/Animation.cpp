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

	m_AnimationData = new ModelAnimationData();
	m_AnimationData->ModelName = "None";

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

void Animation::BakeAnimation()
{

}

void Animation::Release()
{
	// Graphic ���ο� �ִ� �ش� Animation Buffer ����..
	GraphicEngine::Get()->DeleteAnimation(m_AnimationBuffer);

	// �ش� Animation Data ����..
	SAFE_DELETE(m_AnimationBuffer);
}

int Animation::GetAnimationCount()
{
	return (int)m_AnimationData->AnimList.size();
}
