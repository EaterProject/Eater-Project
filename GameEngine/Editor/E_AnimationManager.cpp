#include "E_AnimationManager.h"
#include "EaterHeader.h"
#include "ParserData.h"
#include "Demo.h"

E_AnimationManager::E_AnimationManager()
{

}

E_AnimationManager::~E_AnimationManager()
{

}

void E_AnimationManager::ChangeEaterFile(ParserData::CModel* FBXMesh)
{
	int Size = FBXMesh->m_AnimationList.size();
	if (Size == 0) { return;}

	EATER_CREATE_FILE(SaveFileName, "../Assets/Model/Animation/", ".Eater");
	EATER_SET_NODE("ANIMATION");
	float m_TicksPerFrame = FBXMesh->m_AnimationList[0]->m_TicksPerFrame;
	float m_TotalFrame = FBXMesh->m_AnimationList[0]->m_TotalFrame;
	float m_StartFrame = FBXMesh->m_AnimationList[0]->m_StartFrame;
	float m_EndFrame = FBXMesh->m_AnimationList[0]->m_EndFrame;
	int AnimationCount = (int)FBXMesh->m_AnimationList.size();

	EATER_SET_MAP("TickFrame", std::to_string(m_TicksPerFrame));
	EATER_SET_MAP("TotalFrame", std::to_string(m_TotalFrame));
	EATER_SET_MAP("StartFrame", std::to_string(m_StartFrame));
	EATER_SET_MAP("EndFrame", std::to_string(m_EndFrame));
	EATER_SET_MAP("BoneCount", std::to_string(AnimationCount));

	for (int i = 0; i < AnimationCount; i++)
	{
		int FrameCount = (int)FBXMesh->m_AnimationList[i]->m_AniData.size();
		EATER_SET_LIST_START(std::to_string(i), FrameCount, 11);
		for (int j = 0; j < FrameCount; j++)
		{
			ParserData::CFrame* Frame = FBXMesh->m_AnimationList[i]->m_AniData[j];

			EATER_SET_LIST(Frame->m_Pos.x);
			EATER_SET_LIST(Frame->m_Pos.y);
			EATER_SET_LIST(Frame->m_Pos.z);

			EATER_SET_LIST(Frame->m_RotQt.x);
			EATER_SET_LIST(Frame->m_RotQt.y);
			EATER_SET_LIST(Frame->m_RotQt.z);
			EATER_SET_LIST(Frame->m_RotQt.w);

			EATER_SET_LIST(Frame->m_Scale.x);
			EATER_SET_LIST(Frame->m_Scale.y);
			EATER_SET_LIST(Frame->m_Scale.z);

			EATER_SET_LIST(Frame->m_Time, true);
		}
	}
	EATER_CLOSE_FILE();

	std::string LoadName = "../Assets/Model/Animation/" + SaveFileName + ".Eater";
	Demo::MeshLoad(LoadName);
}

void E_AnimationManager::SetFileName(std::string& FileName)
{
	SaveFileName = FileName;
}
