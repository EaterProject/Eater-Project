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

	for (int i = 0; i < AnimationCount; i++)
	{
		int FrameCount = (int)FBXMesh->m_ModelAnimation->m_AnimationList[i]->m_AniData.size();
		EATER_SET_LIST_START(std::to_string(i), FrameCount, 11);
		for (int j = 0; j < FrameCount; j++)
		{
			ParserData::CFrame* Frame = FBXMesh->m_ModelAnimation->m_AnimationList[i]->m_AniData[j];

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

			EATER_SET_LIST(Frame->m_Time, true);
		}
	}
	EATER_CLOSE_WRITE_FILE();

	std::string LoadName = "../Assets/Model/Animation/" + SaveFileName + ".Eater";
	EditorToolScene::MeshLoad(LoadName);
}

void E_AnimationManager::SetFileName(std::string& FileName)
{
	SaveFileName = FileName;
}
