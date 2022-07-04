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

	for (int i = 0; i <= LastIndex; i++)				
	{
		NowBlock = DissolveList[i].DissolveBlock;

		if (NowBlock == nullptr) continue;

		NowBlock->DissolveTimer += dTime / NowBlock->DissolvePlayTime;

		if (NowBlock->DissolveTimer >= 1.0f)
		{
			NowBlock->Dissolve = false;	
			NowBlock->DissolveTimer = 0.0f;
			NowBlock->DissolvePlayTime = 0.0f;

			// 재생이 끝난 블록 제거..
			PopDissolve(i);
		}
	}
}

void DissolveManager::PushDissolve(MaterialPropertyBlock* block)
{
	for (int i = 0; i < MaxSize; i++)
	{
		if (DissolveList[i].Index == -1)
		{
			DissolveData Data;
			Data.Index = i;
			Data.DissolveBlock = block;

			DissolveList[i] = Data;

			if (LastIndex < i)
			{
				LastIndex = i;
			}

			return;
		}
	}
}

void DissolveManager::PopDissolve(int index)
{
	if (LastIndex == index)
	{
		SetLastIndex();
	}

	DissolveList[index].Index = -1;
	DissolveList[index].DissolveBlock = nullptr;
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
		if (DissolveList[i].Index != -1)
		{
			LastIndex = i;
			return;
		}
	}
}
