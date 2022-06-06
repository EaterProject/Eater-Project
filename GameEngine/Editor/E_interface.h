#pragma once
#include <string>

class GameObject;
class E_Interface
{
public:
	E_Interface();
	virtual ~E_Interface();

	virtual void Initialize() = 0;	//초기화
	virtual void Release() = 0;		//삭제

public:
	virtual void CreateEaterFile(GameObject* Obj) = 0;		//생성
	virtual void ChangeEaterFile(GameObject* Obj) = 0;		//변경
public:
	virtual void SetFileName(std::string FileName);
	virtual void LoadData() = 0;
protected:
	std::string GetFileName(std::string& mPath);		//경로를 잘라서 파일이름을 가져온다
	std::string GetFileType(std::string& mPath);		//경로를 잘라서 파일타입을 가져온다
	std::string GetFilePath(std::string& mPath);		//경로에서 파일이름과 타입을뺀 경로만 가져온다
protected:
	std::string FilePath;
	std::string FileName;
	std::string FileType;
};

