#pragma once
#include <vector>
#include <Component.h>
#include "EngineData.h"

class DissolveManager : public Component
{
public:
	DissolveManager();
	~DissolveManager();

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

	std::vector<MaterialPropertyBlock*> DissolveList;
	MaterialPropertyBlock* NowBlock;
};

