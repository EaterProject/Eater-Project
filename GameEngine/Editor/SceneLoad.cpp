#include "SceneLoad.h"
#include "Light.h"
#include "Demo.h"
#include "MainHeader.h"
#include "EaterHeader.h"

#include "MeshFilter.h"
#include "SkinningFilter.h"
#include "GameObject.h"
#include "AnimationController.h"
#include "Transform.h"
#include "Camera.h"

SceneLoad::SceneLoad()
{
}

SceneLoad::~SceneLoad()
{
}

void SceneLoad::Initialize(std::map<std::string, GameObject*>* ObjectList)
{
	LoadObject = ObjectList;
}

void SceneLoad::Load(std::string FilePath)
{
	EATER_OPEN_READ_FILE(FilePath);
	int Count = EATER_GET_NODE_COUNT();
	for (int i = 0; i < Count; i++)
	{
		std::string NodeName = EATER_GET_NODE_NAME(i);
		if (NodeName == "GAMEOBJECT")
		{
			GameObject* Obj = Instance();
			Obj->SetTag(EATER_GET_MAP(i, "TAG"));
			Obj->Name = EATER_GET_MAP(i, "NAME");

			//트랜스폼은 기본
			Load_Component_Transform(i, Obj);
			
			int Find = 0;
			//매쉬필터
			Find = EATER_GET_LIST_CHOICE(i, "MeshFilter");
			if (Find != -1)
			{
				Load_Component_MeshFilter(i, Obj);
			}

			//애니메이션
			Find = EATER_GET_LIST_CHOICE(i, "Animation");
			if (Find != -1)
			{
				Load_Component_Animation(i, Obj);
			}

			//라이트
			Find = EATER_GET_LIST_CHOICE(i, "Light");
			if (Find != -1)
			{
				Load_Component_Light(i, Obj);
			}

			Find = EATER_GET_LIST_CHOICE(i, "Collider");
			if (Find != -1)
			{
				Load_Component_Collider(i, Obj);
			}

			Find = EATER_GET_LIST_CHOICE(i, "Camera");
			if (Find != -1)
			{
				Load_Component_Camera(i, Obj);
			}

			Find = EATER_GET_LIST_CHOICE(i, "Particle");
			if (Find != -1)
			{
				Load_Component_Particle(i, Obj);
			}
			
			LoadObject->insert({ Obj->Name,Obj });
		}
		else if (NodeName == "SCENE")
		{

		}
	}

	EATER_CLOSE_READ_FILE();
}

void SceneLoad::CreateStaticObject(int index, GameObject* Object)
{
	Load_Component_Transform(index, Object);
	Load_Component_MeshFilter(index, Object);
}

void SceneLoad::CreateSkinObject(int index, GameObject* Object)
{
	Load_Component_Transform(index, Object);
	Load_Component_Skinning(index, Object);
}

void SceneLoad::Load_Component_MeshFilter(int index, GameObject* Object)
{
	MeshFilter* mMeshFilter = Object->AddComponent<MeshFilter>();
	int Count = EATER_GET_LIST_CHOICE(index, "MeshFilter");
	for (int i = 0; i < Count; i++)
	{
		std::vector<std::string> Data;
		EATER_GET_LIST(&Data, i);
		mMeshFilter->SetModelName(Data[0]);
		mMeshFilter->SetMaterialName(Data[1]);
		mMeshFilter->SetMeshName(Data[2]);
	}
}

void SceneLoad::Load_Component_Skinning(int index, GameObject* Object)
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

void SceneLoad::Load_Component_Light(int index, GameObject* Object)
{
	//Object->AddComponent<Light>();

}

void SceneLoad::Load_Component_Particle(int index, GameObject* Object)
{
}

void SceneLoad::Load_Component_Collider(int index, GameObject* Object)
{
}

void SceneLoad::Load_Component_Rigidbody(int index, GameObject* Object)
{
}

void SceneLoad::Load_Component_Camera(int index, GameObject* Object)
{
	Object->AddComponent<Camera>();
}

void SceneLoad::Load_Component_Animation(int index, GameObject* Object)
{
	MeshFilter* MF = Object->GetComponent<MeshFilter>();
	Object->AddComponent<AnimationController>();
	std::vector<std::string>Data;
	EATER_GET_LIST(&Data, index);
	MF->SetAnimationName(MF->GetModelName());
}

void SceneLoad::Load_Component_Transform(int index, GameObject* Object)
{
	Transform* mTransform = Object->GetTransform();
	EATER_GET_LIST_CHOICE(index, "Transform");
	std::vector<float> Data;
	EATER_GET_LIST(&Data, 0);
	mTransform->Position	= { Data[0],Data[1] ,Data[2] };
	mTransform->Rotation	= { Data[3],Data[4] ,Data[5] };
	mTransform->Scale		= { Data[6],Data[7] ,Data[8] };
}
