#include "Eater_LoadScene.h"

#include "EaterHeader.h"
#include "MainHeader.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "AnimationController.h"
#include "GameObject.h"
Eater_LoadScene::Eater_LoadScene()
{

}

Eater_LoadScene::~Eater_LoadScene()
{

}

void Eater_LoadScene::LoadData(std::string& Path)
{
	EATER_OPEN_READ_FILE(Path);
	int Count = EATER_GET_NODE_COUNT();
	for (int i = 0; i < Count; i++)
	{
		std::string NodeName = EATER_GET_NODE_NAME(i);
		if (NodeName == "GAMEOBJECT")
		{
			LoadGameObject(i);
		}
	}
}
void Eater_LoadScene::CreateStaticObject(int index, GameObject* Object)
{
	Load_Component_Transform(index, Object);
	Load_Component_MeshFilter(index, Object);
}

void Eater_LoadScene::CreateSkinObject(int index, GameObject* Object)
{
	Load_Component_Transform(index, Object);
	Load_Component_Skinning(index, Object);
}
void Eater_LoadScene::CreateParticleObject(int index, GameObject* Object)
{

}
void Eater_LoadScene::Load_Component_MeshFilter(int index, GameObject* Object)
{
	MeshFilter* mMeshFilter = Object->AddComponent<MeshFilter>();
	int Count = EATER_GET_LIST_CHOICE(index, "MeshFilter");
	for (int i = 0; i < Count; i++)
	{
		std::vector<std::string> Data;
		EATER_GET_LIST(&Data, i);
		mMeshFilter->SetModelName(Data[0]);
	}
}

void Eater_LoadScene::Load_Component_Transform(int index, GameObject* Object)
{
	Transform* mTransform = Object->GetTransform();
	EATER_GET_LIST_CHOICE(index, "Transform");
	std::vector<float> Data;
	EATER_GET_LIST(&Data, 0);
	mTransform->Position = { Data[0],Data[1] ,Data[2] };
	EATER_GET_LIST(&Data, 1);
	mTransform->Rotation = { Data[0],Data[1] ,Data[2] };
	EATER_GET_LIST(&Data, 2);
	mTransform->Scale = { Data[0],Data[1] ,Data[2] };
}

void Eater_LoadScene::LoadGameObject(int index)
{
	GameObject* Object = Instance();
	std::string Type = EATER_GET_MAP(index, "ModelType");

	if (Type == "BASE")
	{
		CreateStaticObject(index, Object);
	}
	else if (Type == "SKIN")
	{
		CreateSkinObject(index, Object);
	}
}

void Eater_LoadScene::Load_Component_Skinning(int index, GameObject* Object)
{
	AnimationController* AC = Object->AddComponent<AnimationController>();
	MeshFilter* MF = Object->AddComponent<MeshFilter>();
	std::string MeshName;

	int Count = EATER_GET_LIST_CHOICE(index, "MeshFilter");
	for (int i = 0; i < Count; i++)
	{
		std::vector<std::string> Data;
		EATER_GET_LIST(&Data, i);
		MF->SetModelName(Data[0]);
		MeshName = Data[0];
	}

	Count = EATER_GET_LIST_CHOICE(index, "Animation");
	for (int i = 0; i < Count; i++)
	{
		std::vector<std::string> Data;
		EATER_GET_LIST(&Data, 0);
		MF->SetAnimationName(MeshName);
		AC->Choice(Data[0]);
		AC->Play(1, true);
	}
}

