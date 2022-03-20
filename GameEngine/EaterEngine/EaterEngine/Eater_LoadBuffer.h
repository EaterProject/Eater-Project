#pragma once
#include "Eater_Interface.h"
#include <windows.h>
namespace ParserData
{
	class CMesh;
}
class Mesh;
class GraphicEngineManager;


class Eater_LoadBuffer : Eater_Interface
{
public:
	Eater_LoadBuffer();
	virtual ~Eater_LoadBuffer();

	void Initialize(GraphicEngineManager* Graphic, CRITICAL_SECTION* _cs);

	// Eater_Interface을(를) 통해 상속됨
	virtual void LoadData(std::string& Path) override;
private:
	void LoadStaticBuffer(int index, ParserData::CMesh* mesh);
	void LoadSkinBuffer(int index, ParserData::CMesh* mesh);
	void LoadIndex(int index, ParserData::CMesh* mMesh);
	Mesh* CreateBuffer(ParserData::CMesh* mesh);

	GraphicEngineManager* m_Graphic;
	CRITICAL_SECTION* m_CriticalSection;
};

