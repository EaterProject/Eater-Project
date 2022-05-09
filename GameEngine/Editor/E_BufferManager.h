#pragma once

#include <string>
#include <vector>
namespace ParserData
{
	class CMesh;
	class CModel;
}
class OneTriangle;
class E_BufferManager
{
public:
	E_BufferManager();
	~E_BufferManager();
public:
	void ChangeEaterFile(ParserData::CModel* FBXMesh);									//�⺻������ MeshBuffer�� �̴´�
	void ChangeEaterFile_Pos(ParserData::CModel* FBXMesh,std::string NewFileName);		//�Ž����� MeshBuffer�� �̴´�
	void ChangeEaterFile_NavMEsh(ParserData::CModel* FBXMesh, std::string NewFileName);	//�Ž����� MeshBuffer�� �̾Ƽ� �׺�Ž��� ������ش�
	void SetFileName(std::string& FileName);

	void CreateBox();
private:
	//�⺻ �޽� ����
	void SetVertexBuffer(ParserData::CMesh* mMesh);
	void SetPosVertexBuffer(ParserData::CMesh* mMesh);
	void SetIndexBuffer(ParserData::CMesh* mMesh);
	void SetNavigationBuffer(ParserData::CMesh* mMesh);

	//��Ű�� �Ž� ����
	void SetSkinVertexBuffer(ParserData::CMesh* mMesh);
	bool FindInstanceIndex(int Index);

private:
	void SetVertexBuffer
	(
		float PosX,float PosY,float PosZ,
		float UV_X,float UV_Y,
		float Nomal_X,float Nomal_Y,float Nomal_Z,
		float Tanget_X,float Tanget_Y,float Tanget_Z
	);


	std::vector<int> MeshIndexList;
	std::string SaveFileName;

	std::vector<OneTriangle*> Data;
};

