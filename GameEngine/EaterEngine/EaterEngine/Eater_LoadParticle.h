#pragma once
#include "Eater_Interface.h"
class Eater_LoadParticle : public Eater_Interface
{
public:
	Eater_LoadParticle();
	~Eater_LoadParticle();

	// Eater_Interface��(��) ���� ��ӵ�
	virtual void LoadData(std::string& Path) override;
};

