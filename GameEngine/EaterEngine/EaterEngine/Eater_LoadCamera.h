#pragma once
#include <string>
#include <vector>
#include "Eater_Interface.h"
#include "EngineData.h"
class Eater_LoadCamera :public Eater_Interface
{
public:
	Eater_LoadCamera();
	virtual ~Eater_LoadCamera();

	// Eater_Interface을(를) 통해 상속됨
	virtual void LoadData(std::string& Path) override;
	void LoadData(int index);
	void SetPath(std::string Path);
private:
	void CreateKeyData();
	std::string Path;
	std::vector<Vector3> NodePosition;
	std::vector<Vector3> NodeRotation;

};

