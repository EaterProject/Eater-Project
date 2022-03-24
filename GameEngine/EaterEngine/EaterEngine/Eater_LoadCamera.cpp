#include "Eater_LoadCamera.h"
#include "EaterHeader.h"
#include "EngineData.h"
#include <vector>
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
	CameraAnimation* CamAnime = new CameraAnimation();
	int NodeCount = EATER_GET_LIST_CHOICE(index, "KEY");
	for (int i = 0; i < NodeCount; i++)
	{
		std::vector<float> Data;
		EATER_GET_LIST(&Data, i);

		Vector3 Pos = { Data[0],Data[1],Data[2] };
		Vector3 Rot = { Data[3],Data[4],Data[5] };
		CamAnime->Position.push_back(Pos);
		CamAnime->Position.push_back(Rot);
	}
}
