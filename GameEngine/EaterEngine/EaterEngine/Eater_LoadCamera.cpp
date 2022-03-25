#include "Eater_LoadCamera.h"
#include "EaterHeader.h"
#include "EngineData.h"
#include "LoadManager.h"
Eater_LoadCamera::Eater_LoadCamera()
{
}

Eater_LoadCamera::~Eater_LoadCamera()
{
}

void Eater_LoadCamera::LoadData(std::string& Path)
{

}

void Eater_LoadCamera::LoadData(int index)
{
	NodePosition.clear();
	NodeRotation.clear();
	CameraAnimation* CamAnime = new CameraAnimation();

	std::string OneFrame  = (EATER_GET_MAP(index, "Frame"));
	std::string AddKeyCount  = (EATER_GET_MAP(index, "AddKeyCount"));
	CamAnime->OneFrame		= std::stof(OneFrame);
	CamAnime->AddKeyCount	= std::stoi(AddKeyCount);

	int NodeCount = EATER_GET_LIST_CHOICE(index, "KEY");
	for (int i = 0; i < NodeCount; i++)
	{
		std::vector<float> Data;
		EATER_GET_LIST(&Data, i);

		Vector3 Pos = { Data[0],Data[1],Data[2] };
		Vector3 Rot = { Data[3],Data[4],Data[5] };
		NodePosition.push_back(Pos);
		NodeRotation.push_back(Rot);
	}

	float OneFrameAddCount = (1.0f / CamAnime->AddKeyCount); //증가시킬 값
	float ADD = 0.0f; //초기값
	for (int i = 0; i < NodeCount -1; i++)
	{
		Vector3 StartPos	= NodePosition[i];
		Vector3 EndPos		= NodePosition[i + 1];

		Vector3 StartRot	= NodePosition[i];
		Vector3 EndRot		= NodePosition[i + 1];

		for (int j = 0; j < CamAnime->AddKeyCount; j++)
		{
			Vector3 LerpPos = Vector3::Lerp(StartPos, EndPos, ADD);
			Vector3 LerpRot = Vector3::Lerp(StartRot, EndRot, ADD);

			CamAnime->Position.push_back(LerpPos);
			CamAnime->Rotation.push_back(LerpRot);
			ADD += OneFrameAddCount;
		}
	}
	//파일 넣어주기
	std::string FilePathName = CutStr(Path);
	LoadManager::CamAnimationList.insert({ FilePathName ,CamAnime });
}

void Eater_LoadCamera::SetPath(std::string mPath)
{
	Path = mPath;
}

void Eater_LoadCamera::CreateKeyData()
{


}
