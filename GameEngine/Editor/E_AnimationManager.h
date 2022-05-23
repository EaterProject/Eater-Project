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
	void ChangeEaterFile(std::string& FileName, std::string& Name, float min, float max);
	void SetFileName(std::string& FileName);
private:
	std::string SaveFileName;
};

