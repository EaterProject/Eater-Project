#pragma once
#include "Eater_Interface.h"
class Eater_LoadMaterial: public Eater_Interface
{
public:
	Eater_LoadMaterial();
	virtual ~Eater_LoadMaterial();

	// Eater_Interface��(��) ���� ��ӵ�
	virtual void LoadData(std::string& Path) override;
private:
};

