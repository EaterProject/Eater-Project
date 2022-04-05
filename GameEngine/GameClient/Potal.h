#pragma once
#include "ClientComponent.h"
class Potal:public ClientComponent
{
public:
	Potal();
	~Potal();

	// ClientComponent을(를) 통해 상속됨
	virtual void ReSet() override;
};

