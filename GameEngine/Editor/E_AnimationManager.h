#pragma once
#include <string>
namespace ParserData
{
	class CModel;
}
class E_AnimationManager
{
public:
	E_AnimationManager();
	~E_AnimationManager();

	void ChangeEaterFile(ParserData::CModel* FBXMesh);
	void SetFileName(std::string& FileName);
private:
	std::string SaveFileName;
};

