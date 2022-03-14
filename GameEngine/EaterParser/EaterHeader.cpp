#include "EaterHeader.h"
#include "EaterParser.h"

EaterParser* Parser = new EaterParser();
void EATER_OPEN_FILE(std::string Path)
{
	Parser->OPEN_File(Path);
}

void EATER_CREATE_FILE(std::string FileName, std::string OutPath, std::string FileType)
{
	Parser->CREATE_File(FileName, OutPath, FileType);
}

void EATER_CLOSE_FILE()
{
	Parser->CLOSE_File();
}

 void EATER_SET_NODE(std::string NodeName)
{
	 Parser->SetNode(NodeName);
}

 void EATER_SET_MAP(std::string key, std::string value)
 {
	 Parser->SetMap(key, value);
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

 void EATER_SET_INDEX_START(int IndexCount)
 {
	 Parser->SetIndexStart(IndexCount);
 }

 void EATER_SET_INDEX(int& x, int& y, int& z)
 {
	 Parser->SetIndex(x, y, z);
 }

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
