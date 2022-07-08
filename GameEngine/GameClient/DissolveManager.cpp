#include "DissolveManager.h"
#include "EaterEngineAPI.h"

DissolveManager::DissolveManager()
{
	DissolveList.resize(MaxSize);
}

DissolveManager::~DissolveManager()
{

}

void DissolveManager::Update()
{
	float dTime = GetDeltaTime();

	for (int i = 0; i < MaxSize; i++)
	{
		NowBlock = DissolveList[i];

		if (NowBlock == nullptr) continue;

		NowBlock->DissolveTimer += dTime / NowBlock->DissolvePlayTime;

		if (NowBlock->DissolveTimer >= 0.99f)
		{
			NowBlock->Dissolve = false;
			NowBlock->DissolveTimer = 0.0f;

			// 재생이 끝난 블록 제거..
			PopDissolve(i);
		}
	}
}

void DissolveManager::PushDissolve(MaterialPropertyBlock* block)
{
	for (int i = 0; i < MaxSize; i++)
	{
		if (DissolveList[i] == nullptr)
		{
			DissolveList[i] = block;
			return;
		}
	}
}

void DissolveManager::PopDissolve(int index)
{
	DissolveList[index] = nullptr;
}

void DissolveManager::SetLastIndex()
{
	LastIndex--;

	if (LastIndex <= -1)
	{
		LastIndex = -1;
		return;
	}

	for (int i = LastIndex; i >= 0; i--)
	{
		if (DissolveList[i] != nullptr)
		{
			LastIndex = i;
			return;
		}
	}
}
