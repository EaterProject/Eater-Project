#pragma once
#include "ClientComponent.h"
class Potal:public ClientComponent
{
public:
	Potal();
	~Potal();

	// ClientComponent��(��) ���� ��ӵ�
	virtual void ReSet() override;
};

