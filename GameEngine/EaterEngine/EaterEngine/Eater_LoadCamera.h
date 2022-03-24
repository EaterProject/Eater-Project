#pragma once

#include "Eater_Interface.h"
class Eater_LoadCamera :public Eater_Interface
{
public:
	Eater_LoadCamera();
	virtual ~Eater_LoadCamera();

	// Eater_Interface을(를) 통해 상속됨
	virtual void LoadData(std::string& Path) override;
	void LoadData(int index);
};

