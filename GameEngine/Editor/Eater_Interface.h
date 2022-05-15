#pragma once
#include <string>
#include <vector>
class Eater_Interface
{
public:
	Eater_Interface();
	~Eater_Interface();

public:
	//데이터 로드
	virtual void LoadData(std::string& Path) = 0;
	std::string CutStr(std::string& Name);
};

