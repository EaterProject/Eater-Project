#pragma once
#include <vector>
#include <Component.h>
#include "EngineData.h"

class DissolveManager : public Component
{
public:
	DissolveManager();
	~DissolveManager();

private:
	struct DissolveData
	{
		int Index = -1;
		MaterialPropertyBlock* DissolveBlock;
	};

public:
	void Update() override;

public:
	void PushDissolve(MaterialPropertyBlock* block);
	void PopDissolve(int index);

private:
	void SetLastIndex();

private:
	int MaxSize = 100;
	int LastIndex = -1;

	std::vector<DissolveData> DissolveList;
	MaterialPropertyBlock* NowBlock;
};

