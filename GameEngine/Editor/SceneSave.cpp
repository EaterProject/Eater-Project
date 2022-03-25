#include "SceneSave.h"

#include "MainHeader.h"
#include "EaterHeader.h"
#include "GameObject.h"
#include "EngineData.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "Demo.h"
#include "AnimationController.h"

SceneSave::SceneSave()
{
}

SceneSave::~SceneSave()
{
}

void SceneSave::Save(std::string SaveFilePath, std::string SaveFileName, std::map<std::string, GameObject*>& Data)
{
	EATER_OPEN_WRITE_FILE(SaveFileName, SaveFilePath,".Scene");
	std::map<std::string, GameObject*>::iterator Start_it	= Data.begin();
	std::map<std::string, GameObject*>::iterator End_it		= Data.end();

	EATER_SET_NODE("SCENE");
	EATER_SET_LIST_START("TAG", 10, 2);
	EATER_SET_LIST(0);
	EATER_SET_LIST("Default",true);
	EATER_SET_LIST(1);
	EATER_SET_LIST("MainCam",true);
	EATER_SET_LIST(2);
	EATER_SET_LIST("Point",true);
	EATER_SET_LIST(3);
	EATER_SET_LIST("Player",true);


	for (Start_it; Start_it != End_it; Start_it++)
	{
		GameObject* Object		= Start_it->second;

		Object->OneMeshData->Object_Data->ObjType;
		MeshData*	OneMesh		= Object->OneMeshData;
		MeshFilter* mMeshFilter = Object->GetComponent<MeshFilter>();
		Transform*	mTransform	= Object->GetComponent<Transform>();

		EATER_SET_NODE("GAMEOBJECT");
		
		SaveObject(Object);
	}
	EATER_CLOSE_WRITE_FILE();
}

void SceneSave::Load(std::string FileName)
{
	EATER_OPEN_READ_FILE(FileName);
	int Count = EATER_GET_NODE_COUNT();
	for (int i = 0; i < Count; i++)
	{
		std::string NodeName = EATER_GET_NODE_NAME(i);
		if (NodeName == "GAMEOBJECT")
		{
			std::string Name = EATER_GET_MAP(i, "ModelName");
			GameObject* Object = Instance(Name);
			std::string Type = EATER_GET_MAP(i, "ModelType");

			if (Type == "BASE")
			{
				CreateStaticObject(i, Object);
			}
			else if (Type == "SKIN")
			{
				CreateSkinObject(i, Object);
			}
			Demo::ObjectList.insert({Object->Name,Object});
		}
		else if(NodeName == "SCENE")
		{

		}
	}

	EATER_CLOSE_READ_FILE();
}

void SceneSave::SaveTransform(GameObject* Obj)
{
	Transform* mTransform = Obj->GetComponent<Transform>();
	EATER_SET_LIST_START("Transform", 3, 3);
	EATER_SET_LIST(mTransform->Position.x);
	EATER_SET_LIST(mTransform->Position.y);
	EATER_SET_LIST(mTransform->Position.z, true);
	EATER_SET_LIST(mTransform->Rotation.x);
	EATER_SET_LIST(mTransform->Rotation.y);
	EATER_SET_LIST(mTransform->Rotation.z, true);
	EATER_SET_LIST(mTransform->Scale.x);
	EATER_SET_LIST(mTransform->Scale.y);
	EATER_SET_LIST(mTransform->Scale.z, true);
}
void SceneSave::CreateStaticObject(int index, GameObject* Object)
{
	Load_Component_Transform(index, Object);
	Load_Component_MeshFilter(index, Object);
}

void SceneSave::CreateSkinObject(int index, GameObject* Object)
{
	Load_Component_Transform(index, Object);
	Load_Component_Skinning(index, Object);
}

void SceneSave::Load_Component_MeshFilter(int index, GameObject* Object)
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

void SceneSave::Load_Component_Skinning(int index, GameObject* Object)
{
	AnimationController* AC = Object->AddComponent<AnimationController>();
	MeshFilter* MF			= Object->AddComponent<MeshFilter>();
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

void SceneSave::Load_Component_Transform(int index, GameObject* Object)
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

void SceneSave::SaveMeshFilter(GameObject* Obj)
{
	//매쉬필터 데이터를 저장
	MeshFilter* mMeshFilter = Obj->GetComponent<MeshFilter>();
	EATER_SET_LIST_START("MeshFilter",1,1);
	EATER_SET_LIST(mMeshFilter->GetModelName(), true);
}

void SceneSave::SaveAnimation(GameObject* Obj)
{
	AnimationController* AC = Obj->GetComponent<AnimationController>();
	
	EATER_SET_LIST_START("Animation", 1, 1);
	EATER_SET_LIST(AC->GetNowAnimationName(), true);
}

void SceneSave::SaveObject(GameObject* Obj)
{
	//이름에 +가 있다면 스키닝 오브젝트
	if (Obj->Name.rfind('+') == std::string::npos) 
	{
		MeshData* OneMesh = Obj->OneMeshData;
		switch (OneMesh->Object_Data->ObjType)
		{
		case OBJECT_TYPE::BASE:
		case OBJECT_TYPE::DEFALT:
			EATER_SET_MAP("ModelType", "BASE");
			EATER_SET_MAP("ModelName", Obj->Name);
			EATER_SET_MAP("TAG", std::to_string(Obj->GetTag()));
			SaveMeshFilter(Obj);
			SaveTransform(Obj);
			break;

		case OBJECT_TYPE::PARTICLE_SYSTEM:
			EATER_SET_MAP("ModelType", "PARTICLE");
			break;

		case OBJECT_TYPE::LIGHT:
			EATER_SET_MAP("ModelType", "LIGHT");
			break;
		}
	}
	else
	{
		EATER_SET_MAP("ModelType", "SKIN");
		EATER_SET_MAP("ModelName", Obj->Name);
		EATER_SET_MAP("TAG", std::to_string(Obj->GetTag()));
		SaveMeshFilter(Obj);
		SaveTransform(Obj);
		SaveAnimation(Obj);
	}
}
