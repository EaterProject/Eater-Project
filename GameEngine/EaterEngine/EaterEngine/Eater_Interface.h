#pragma once
#include <string>
#include <vector>
class Eater_Interface
{
public:
	Eater_Interface();
	~Eater_Interface();
public:
	//������ �ε�
	virtual void LoadData(std::string& Path) = 0;
	virtual void Initialize() { return; }



	std::string CutStr(std::string& Name);
};

