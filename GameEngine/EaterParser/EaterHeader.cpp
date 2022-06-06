#include "EaterHeader.h"
#include "EaterParser.h"
#include "EaterHeader.h"

EaterParser* Parser = new EaterParser();
void EATER_OPEN_READ_FILE(std::string Path)
{
	Parser->OPEN_FILE(Path);
}

void EATER_OPEN_WRITE_FILE(std::string FileName, std::string OutPath, std::string FileType)
{
	Parser->CREATE_FILE(FileName, OutPath, FileType);
}

void EATER_CLOSE_WRITE_FILE()
{
	Parser->CLOSE_WRITE_FILE();
}

void EATER_CLOSE_READ_FILE()
{
	Parser->CLOSE_READ_FILE();
}

void EATER_CLOSE_CHANGE_FILE(std::string FileName, std::string OutPath, std::string FileType)
{
	Parser->ChangeDataSave(FileName,OutPath,FileType);
}

 void EATER_SET_NODE(std::string NodeName)
{
	 Parser->SetNode(NodeName);
}

 void EATER_SET_MAP(std::string key, std::string value)
 {
	 Parser->SetMap(key, value);
 }

 void EATER_CHANGE_MAP(int NodeIndex, std::string key, std::string value)
 {
	 if (value == "") { value = "0"; }
	 Parser->ChangeMap(NodeIndex, key, value);
 }

 void EATER_SET_MATERIAL(const EATER_MATERIAL_DATA& Data)
 {
	 Parser->SetMaterial(Data);
 }

 void EATER_SET_LIST_START(std::string Name, int LineCount, int WordCount)
 {
	 Parser->SetListStart(Name, LineCount, WordCount);
 }

 void EATER_SET_LIST_END()
 {
	 Parser->SetListEnd();
 }

 void EATER_SET_VERTEX_START(int VetexCount, VERTEX_TYPE Type)
 {
	 Parser->SetVertexStart(VetexCount, (int)Type);
 }

 void EATER_SET_VERTEX(EATER_VERTEX_BASE& Base)
 {
	 Parser->SetVertex(Base);
 }

 void EATER_SET_VERTEX(EATER_VERTEX_SKIN& Skin)
 {
	 Parser->SetVertex(Skin);
 }

 void EATER_SET_VERTEX(EATER_VERTEX_POS& Pos)
 {
	 Parser->SetVertex(Pos);
 }

 void EATER_SET_INDEX_START(int IndexCount)
 {
	 Parser->SetIndexStart(IndexCount);
 }

 void EATER_SET_INDEX(int& x, int& y, int& z)
 {
	 Parser->SetIndex(x, y, z);
 }

 void SET_SAVE_MATERIAL()
 {
	 Parser->SaveMaterial();
 }

 void SET_SAVE_PARTICLE()
 {
	 Parser->SaveParticle();
 }

 FM_MATERIAL* GET_FORMAT_MATERIAL()
 {
	 Parser->mMaterial_Data->ReSet();
	 return Parser->mMaterial_Data;
 }

 FM_VERTEX* GET_FORMAT_VERTEX(FM_VERTEX_TYPE Type)
 {
	 Parser->mVertex_Data->Type = Type;
	 return Parser->mVertex_Data;
 }

 FM_ANIMATION* GET_FORMAT_ANIMATION()
 {
	 return Parser->mAnimation_Data;
 }

 FM_INDEX* GET_FORMAT_INDEX()
 {
	 return  Parser->mIndex_Data;
 }

 FM_PARTICLE* GET_FORMAT_PARTICLE()
 {
	 return Parser->mParticle_Data;
 }

 //EATER_ANIMATION_DATA* EATER_GET_ANIMATION_DATA()
 //{
//	 return Parser->mAnimation_Data;
 //}
 //
 //EATER_MATERIAL_DATA* EATER_GET_MATERIAL_DATA()
 //{
//	 return Parser->mMaterial_Data;
 //}
 //
 //EATER_VERTEX_BASE* EATER_GET_VERTEX_BASE_DATA()
 //{
//	 return Parser->mVertex_Base_Data;
 //}
 //
 //EATER_VERTEX_SKIN* EATER_GET_VERTEX_SKIN_DATA()
 //{
//	 return Parser->mVertex_Skin_Data;
 //}

 int EATER_GET_NODE_COUNT()
 {
	 return Parser->GetNodeCount();
 }

 std::string& EATER_GET_NODE_NAME(int NodeIndex)
 {
	 return  Parser->GetNodeName(NodeIndex);
 }

 std::string& EATER_GET_MAP(int NodeIndex ,std::string Key)
 {
	 return Parser->GetMapData(NodeIndex, Key);
 }

 int EATER_GET_LIST_CHOICE(int NodeCount, std::string ListName)
 {
	 return Parser->ChoiceListData(NodeCount, ListName);
 }

 void EATER_GET_LIST(std::vector<float>* Data, int index)
 {
	Parser->GetList(Data, index);
 }

 void EATER_GET_LIST(std::vector<std::string>* Data, int index)
 {
	 Parser->GetList(Data, index);
 }

 void EATER_CHANGE_LIST(int NodeCount, std::string ListName, int cx_index, int cy_index, std::string ChangeData)
 {
	 Parser->ChangeList(NodeCount, ListName, cx_index, cy_index, ChangeData);
 }

 void EATER_CLEAR_NODE()
 {
	 Parser->ClearNode();
 }

 void EATER_SET_LIST(std::string Data, bool Line)
 {
	 Parser->SetList(Data, Line);
 }

 void EATER_SET_LIST(int Data, bool Line)
 {
	 Parser->SetList(Data, Line);
 }

 void EATER_SET_LIST(float Data, bool Line)
 {
	 Parser->SetList(Data, Line);
 }
