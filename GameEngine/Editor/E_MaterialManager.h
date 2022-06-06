#pragma once

#include <string>
#include <vector>
#include "EditorData.h"
#include "E_interface.h"
namespace ParserData
{
	class CMesh;
	class CModel;
}
struct InstanceMaterial;

class E_MaterialManager : public E_Interface
{
public:
	E_MaterialManager();
	~E_MaterialManager();

	void ChangeEaterFile(ParserData::CModel* FBXMesh);

	void Create(InstanceMaterial* mInstanceMaterial);
	void CreateBase(std::string Filename);

	// E_Interface을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Release() override;
	virtual void CreateEaterFile(GameObject* Obj) override;
	virtual void ChangeEaterFile(GameObject* Obj) override;
	virtual void LoadData() override;
private:
	std::string SaveFileName;
	std::string CutStr(std::string& Path);

};

