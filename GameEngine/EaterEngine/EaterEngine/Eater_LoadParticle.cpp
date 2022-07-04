#include "Eater_LoadParticle.h"
#include "EaterHeader.h"
#include "EngineData.h"
#include "LoadManager.h"
#include <string>
Eater_LoadParticle::Eater_LoadParticle()
{
}

Eater_LoadParticle::~Eater_LoadParticle()
{
}

void Eater_LoadParticle::LoadData(std::string& Path)
{
	LoadParticleData* mBaseAttackParticle = new LoadParticleData();

	EATER_OPEN_READ_FILE(Path);
	int NodeCount = EATER_GET_NODE_COUNT();
	for (int i = 0; i < NodeCount; i++)
	{
		mBaseAttackParticle->TextrueName  = EATER_GET_MAP(i, "TextureName");
		mBaseAttackParticle->RenderType   = std::stoi(EATER_GET_MAP(i, "RenderType"));
		mBaseAttackParticle->ColorType	= std::stoi(EATER_GET_MAP(i, "ColorType"));
		mBaseAttackParticle->ScaleType	= std::stoi(EATER_GET_MAP(i, "ScaleType"));
		mBaseAttackParticle->MaxParticle	= std::stoi(EATER_GET_MAP(i, "MaxParticle"));
		mBaseAttackParticle->RateOverTime = std::stof(EATER_GET_MAP(i, "RateOverTime"));
		mBaseAttackParticle->Strength		= std::stof(EATER_GET_MAP(i, "Strength"));

		mBaseAttackParticle->PlayTime		= std::stof(EATER_GET_MAP(i, "PlayTime"));
		mBaseAttackParticle->Loop			= std::stoi(EATER_GET_MAP(i, "Loop"));

		std::vector<float> Data;
		EATER_GET_LIST_CHOICE(i, "Radius");
		EATER_GET_LIST(&Data, 0);
		mBaseAttackParticle->Radius.x = Data[0];
		mBaseAttackParticle->Radius.y = Data[1];
		mBaseAttackParticle->Radius.z = Data[2];

		EATER_GET_LIST_CHOICE(i, "Tiling");
		EATER_GET_LIST(&Data, 0);
		mBaseAttackParticle->Tiling.x = Data[0];
		mBaseAttackParticle->Tiling.y = Data[1];

		EATER_GET_LIST_CHOICE(i, "StartForce");
		EATER_GET_LIST(&Data, 0);
		mBaseAttackParticle->StartForce_Min.x = Data[0];
		mBaseAttackParticle->StartForce_Min.y = Data[1];
		mBaseAttackParticle->StartForce_Min.z = Data[2];
		EATER_GET_LIST(&Data, 1);
		mBaseAttackParticle->StartForce_Max.x = Data[0];
		mBaseAttackParticle->StartForce_Max.y = Data[1];
		mBaseAttackParticle->StartForce_Max.z = Data[2];

		EATER_GET_LIST_CHOICE(i, "StartColor");
		EATER_GET_LIST(&Data, 0);
		mBaseAttackParticle->StartColor_Min.x = Data[0];
		mBaseAttackParticle->StartColor_Min.y = Data[1];
		mBaseAttackParticle->StartColor_Min.z = Data[2];
		mBaseAttackParticle->StartColor_Min.w = Data[3];
		EATER_GET_LIST(&Data, 1);
		mBaseAttackParticle->StartColor_Max.x = Data[0];
		mBaseAttackParticle->StartColor_Max.y = Data[1];
		mBaseAttackParticle->StartColor_Max.z = Data[2];
		mBaseAttackParticle->StartColor_Max.w = Data[3];

		EATER_GET_LIST_CHOICE(i, "StartLifeTime");
		EATER_GET_LIST(&Data, 0);
		mBaseAttackParticle->StartLifeTime.x = Data[0];
		mBaseAttackParticle->StartLifeTime.y = Data[1];

		EATER_GET_LIST_CHOICE(i, "StartScale");
		EATER_GET_LIST(&Data, 0);
		mBaseAttackParticle->StartScale.x = Data[0];
		mBaseAttackParticle->StartScale.y = Data[1];

		EATER_GET_LIST_CHOICE(i, "StartRotation");
		EATER_GET_LIST(&Data, 0);
		mBaseAttackParticle->StartRotation.x = Data[0];
		mBaseAttackParticle->StartRotation.y = Data[1];

		EATER_GET_LIST_CHOICE(i, "LifeForce");
		EATER_GET_LIST(&Data, 0);
		mBaseAttackParticle->LifeForce_Min.x = Data[0];
		mBaseAttackParticle->LifeForce_Min.y = Data[1];
		mBaseAttackParticle->LifeForce_Min.z = Data[2];
		EATER_GET_LIST(&Data, 1);
		mBaseAttackParticle->LifeForce_Max.x = Data[0];
		mBaseAttackParticle->LifeForce_Max.y = Data[1];
		mBaseAttackParticle->LifeForce_Max.z = Data[2];

		EATER_GET_LIST_CHOICE(i, "LifeColor");
		EATER_GET_LIST(&Data, 0);
		mBaseAttackParticle->LifeColor_Min.x = Data[0];
		mBaseAttackParticle->LifeColor_Min.y = Data[1];
		mBaseAttackParticle->LifeColor_Min.z = Data[2];
		mBaseAttackParticle->LifeColor_Min.w = Data[3];
		EATER_GET_LIST(&Data, 1);
		mBaseAttackParticle->LifeColor_Max.x = Data[0];
		mBaseAttackParticle->LifeColor_Max.y = Data[1];
		mBaseAttackParticle->LifeColor_Max.z = Data[2];
		mBaseAttackParticle->LifeColor_Max.w = Data[3];

		EATER_GET_LIST_CHOICE(i, "LifeScale");
		EATER_GET_LIST(&Data, 0);
		mBaseAttackParticle->LifeScale.x = Data[0];
		mBaseAttackParticle->LifeScale.y = Data[1];

		EATER_GET_LIST_CHOICE(i, "LifeRotation");
		EATER_GET_LIST(&Data, 0);
		mBaseAttackParticle->LifeRotation.x = Data[0];
		mBaseAttackParticle->LifeRotation.y = Data[1];
	}
	EATER_CLOSE_READ_FILE();
	std::size_t Start	= Path.rfind('/') + 1;
	std::size_t End		= Path.rfind('.') - Start;
	std::string FileName = Path.substr(Start, End);

	LoadManager::ParticleList.insert({ FileName, mBaseAttackParticle });
}
