#pragma once
#include <string>

class GameObject;
class E_Interface
{
public:
	E_Interface();
	virtual ~E_Interface();

	virtual void Initialize() = 0;	//�ʱ�ȭ
	virtual void Release() = 0;		//����

public:
	virtual void CreateEaterFile(GameObject* Obj) = 0;		//����
	virtual void ChangeEaterFile(GameObject* Obj) = 0;		//����
public:
	virtual void SetFileName(std::string FileName);
	virtual void LoadData() = 0;
protected:
	std::string GetFileName(std::string& mPath);		//��θ� �߶� �����̸��� �����´�
	std::string GetFileType(std::string& mPath);		//��θ� �߶� ����Ÿ���� �����´�
	std::string GetFilePath(std::string& mPath);		//��ο��� �����̸��� Ÿ������ ��θ� �����´�
protected:
	std::string FilePath;
	std::string FileName;
	std::string FileType;
};

