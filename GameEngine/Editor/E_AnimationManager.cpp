#include "E_AnimationManager.h"
#include "EaterHeader.h"
#include "ParserData.h"
#include "EditorToolScene.h"

E_AnimationManager::E_AnimationManager()
{

}

E_AnimationManager::~E_AnimationManager()
{

}

void E_AnimationManager::ChangeEaterFile(ParserData::CModel* FBXMesh)
{
	if (FBXMesh->m_ModelAnimation == nullptr) { return;}

	int Size = (int)FBXMesh->m_ModelAnimation->m_AnimationList.size();

	EATER_OPEN_WRITE_FILE(SaveFileName, "../Assets/Model/Animation/", ".Eater");
	EATER_SET_NODE("ANIMATION");
	float m_TicksPerFrame	= (float)FBXMesh->m_ModelAnimation->m_TicksPerFrame;
	float m_TotalFrame		= (float)FBXMesh->m_ModelAnimation->m_TotalFrame;
	float m_StartFrame		= (float)FBXMesh->m_ModelAnimation->m_StartFrame;
	float m_EndFrame		= (float)FBXMesh->m_ModelAnimation->m_EndFrame;
	int AnimationCount		= (int)FBXMesh->m_ModelAnimation->m_AnimationList.size();

	EATER_SET_MAP("TickFrame", std::to_string(m_TicksPerFrame));
	EATER_SET_MAP("TotalFrame", std::to_string(m_TotalFrame));
	EATER_SET_MAP("StartFrame", std::to_string(m_StartFrame));
	EATER_SET_MAP("EndFrame", std::to_string(m_EndFrame));
	EATER_SET_MAP("BoneCount", std::to_string(AnimationCount));
	EATER_SET_MAP("EventMin", "0");
	EATER_SET_MAP("EventMax", "0");

	for (int i = 0; i < AnimationCount; i++)
	{
		int FrameCount = (int)FBXMesh->m_ModelAnimation->m_AnimationList[i]->m_AniData.size();
		EATER_SET_LIST_START(std::to_string(i), FrameCount, 21);
		for (int j = 0; j < FrameCount; j++)
		{
			ParserData::CFrame* Frame = FBXMesh->m_ModelAnimation->m_AnimationList[i]->m_AniData[j];

			EATER_SET_LIST(Frame->m_Time);

			EATER_SET_LIST(Frame->m_LocalPos.x);
			EATER_SET_LIST(Frame->m_LocalPos.y);
			EATER_SET_LIST(Frame->m_LocalPos.z);
			EATER_SET_LIST(Frame->m_LocalRotQt.x);
			EATER_SET_LIST(Frame->m_LocalRotQt.y);
			EATER_SET_LIST(Frame->m_LocalRotQt.z);
			EATER_SET_LIST(Frame->m_LocalRotQt.w);
			EATER_SET_LIST(Frame->m_LocalScale.x);
			EATER_SET_LIST(Frame->m_LocalScale.y);
			EATER_SET_LIST(Frame->m_LocalScale.z);

			EATER_SET_LIST(Frame->m_WorldPos.x);
			EATER_SET_LIST(Frame->m_WorldPos.y);
			EATER_SET_LIST(Frame->m_WorldPos.z);
			EATER_SET_LIST(Frame->m_WorldRotQt.x);
			EATER_SET_LIST(Frame->m_WorldRotQt.y);
			EATER_SET_LIST(Frame->m_WorldRotQt.z);
			EATER_SET_LIST(Frame->m_WorldRotQt.w);
			EATER_SET_LIST(Frame->m_WorldScale.x);
			EATER_SET_LIST(Frame->m_WorldScale.y);
			EATER_SET_LIST(Frame->m_WorldScale.z,true);
		}
	}
	EATER_CLOSE_WRITE_FILE();

	std::string LoadName = "../Assets/Model/Animation/" + SaveFileName + ".Eater";
	EditorToolScene::MeshLoad(LoadName);
}

void E_AnimationManager::ChangeEaterFile(std::string& FileName,std::string& Name ,float min,float max)
{
	EATER_OPEN_READ_FILE(FileName);
	std::string num = EATER_GET_NODE_NAME(0);
	int NodeCount = EATER_GET_NODE_COUNT();
	EATER_CHANGE_MAP(0, "EventMin", std::to_string(min));
	EATER_CHANGE_MAP(0, "EventMax", std::to_string(max));
	EATER_CLOSE_CHANGE_FILE(Name,"../Assets/Model/Animation/");
}


void E_AnimationManager::SetFileName(std::string& FileName)
{
	SaveFileName = FileName;
}
