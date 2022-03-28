#include "Demo.h"
#include "GameObject.h"
#include "MainHeader.h"
#include "TypeOptionHeader.h"
#include "EngineData.h"

#include "SceneSave.h"
#include "SceneLoad.h"

#include "Transform.h"
#include "MeshFilter.h"
#include "AnimationController.h"
#include "Terrain.h"
#include "ParticleSystem.h"
#include "Light.h"
#include "Camera.h"

std::map<std::string, GameObject*> Demo::ObjectList;

SceneSave*		Demo::mSaveManager = nullptr;
Eater_LoadScene*		Demo::mLoadManager = nullptr;

GameObject*		Demo::CamObject		= nullptr;
GameObject*		Demo::DebugCamObject = nullptr;
Demo::Demo()
{

}
                                          
Demo::~Demo()
{

}

void Demo::Awake()
{
	mSaveManager = new SceneSave();
	mLoadManager = new Eater_LoadScene();
	mSaveManager->Initialize(&ObjectList);
	mLoadManager->Initialize(&ObjectList);

	//텍스쳐를 로드
	Load("../Assets/Texture/ModelTexture");
	Load("../Assets/Texture/Terrain");
	Load("../Assets/Texture/Material");
	Load("../Assets/Texture/Particle");

	DebugCamObject = GetMainCamera();
}

void Demo::Update()
{
	if (ThreadRun == true) 
	{
		return;
	}
}

void Demo::End()
{


}

void Demo::ThreadFunction()
{
	Load("../Assets/Model/Animation");
	Load("../Assets/Model/MeshBuffer");
	Load("../Assets/Model/ModelData");
	
	LoadEnvironment("../Assets/Texture/Environment/Night.dds");
	SetEnvironment(true);
}

GameObject* Demo::Create_GameObject()
{
	GameObject* Object = Instance("None_GameObject");
	Object->Name = FindMeshName("None_GameObject");
	ObjectList.insert({ Object->Name,Object });
	return Object;
}

GameObject* Demo::Create_Object(std::string MeshName)
{
	std::string ObjectName = FindMeshName(MeshName);

	if (MeshName.rfind('+') == std::string::npos)
	{
		//기본 메쉬 생성
		return CreateBaseObject(ObjectName, MeshName);
	}
	else
	{
		//스키닝 매쉬 생성
		return CreateSkinObject(ObjectName, MeshName);
	}
}

GameObject* Demo::Create_Camera()
{
	GameObject* Cam = InstanceCamera("Camera");
	Cam->Name = FindMeshName("Camera");
	ObjectList.insert({Cam->Name,Cam});
	return Cam;
}

GameObject* Demo::CreateBaseObject(std::string ObjectName, std::string MeshName)
{
	GameObject* Object = Instance(ObjectName);
	MeshFilter* mMeshFilter = Object->AddComponent<MeshFilter>();
	Transform* mTransform = Object->GetTransform();

	mMeshFilter->SetModelName(MeshName);
	ObjectList.insert({ Object->Name, Object });
	return Object;
}

GameObject* Demo::CreateSkinObject(std::string ObjectName, std::string MeshName)
{
	GameObject* Skin = Instance(ObjectName);
	MeshFilter* MF = Skin->AddComponent<MeshFilter>();
	Transform* TR = Skin->GetTransform();
	AnimationController* AC = Skin->AddComponent<AnimationController>();

	MF->SetModelName(MeshName);
	MF->SetAnimationName(MeshName);
	ObjectList.insert({ ObjectName,Skin });
	return Skin;
}

void Demo::MeshLoad(std::string Path)
{
	Load(Path);
}

bool Demo::DeleteObject(std::string MeshName)
{
	GameObject* obj = ObjectList[MeshName];
	if (obj != nullptr)
	{
		Destroy(obj);
		ObjectList.erase(MeshName);
		return true;
	}
	else
	{
		return false;
	}
}

void Demo::SaveScene(std::string SaveFilePath,std::string SaveFileName)
{
	mSaveManager->Save(SaveFilePath, SaveFileName);
}

void Demo::LoadScene(std::string LoadScenePath)
{
	std::string Path = "../Assets/Scene/" + LoadScenePath;
	mLoadManager->Load(Path);
}

GameObject* Demo::FindMesh(std::string MeshName)
{
	return ObjectList[MeshName];
}

GameObject* Demo::FindMesh(std::string MeshName, std::string ParentName)
{
	GameObject* Parent =  FindMesh(ParentName);
	GameObject* Child	= Parent->GetChildObject(MeshName);
	return Child;
}

std::string Demo::FindMeshName(std::string MeshName)
{
	std::string ObjectName = MeshName;
	std::map<std::string, GameObject*>::iterator Start_it		= ObjectList.end();
	int Meshindex = 0;
	while (true)
	{
		std::map<std::string, GameObject*>::iterator End_it		= ObjectList.find(ObjectName);
		if (Start_it == End_it)
		{
			return ObjectName;
		}
		else
		{
			Meshindex++;
			ObjectName = MeshName + "(" + std::to_string(Meshindex) + ")";
		}
	}
}

GameObject* Demo::FindMainCamera()
{
	return GetMainCamera();
}

GameObject* Demo::Create_Terrain(std::string MeshName)
{
	
	//터레인 생성
	LoadTerrainMesh("../Assets/Model/TerrainModel/Terrain.fbx", "../Assets/Texture/Terrain/Terrain_RGB.png", SCALING);

	GameObject* TerrainObect = InstanceTerrain("Terrain");

	Terrain* mTerrain = TerrainObect->GetComponent<Terrain>();
	mTerrain->SetLayerName("terrain_ground_A_BaseColor", "terrain_ground_A_Normal", "terrain_ground_A_ORM");
	mTerrain->SetLayerName("terrain_ground_B_BaseColor", "terrain_ground_B_Normal", "terrain_ground_B_ORM");
	mTerrain->SetMeshName("Terrain");
	//mTerrain->SetColliderName("TerrainDecimate");
	mTerrain->SetTextureTiling(31.0f);
	return TerrainObect;
}

GameObject* Demo::Create_Light()
{
	GameObject* LightObject = InstanceLight("Light", LIGHT_TYPE::SPOT_LIGHT);
	LightObject->Name = FindMeshName(LightObject->Name);
	ObjectList.insert({ LightObject->Name, LightObject });
	return LightObject;
}

GameObject* Demo::Create_Particle()
{
	std::string Name = FindMeshName("Particle");
	GameObject* obj = InstanceParticle(Name);

	ParticleSystem* particles = obj->GetComponent<ParticleSystem>();
	particles->SetMeshName("Quad");
	particles->SetRenderType(PARTICLE_RENDER_OPTION::VERTICAL_BILLBOARD);
	particles->SetDiffuseName("particle_hotCloud");
	particles->SetStartLifeTime(1.5f, 1.8f);
	particles->SetStartScale(0.0f, 7.0f);
	particles->SetStartRotation(-360, 360);
	particles->SetStartColor(Vector4(255, 174, 73, 28), Vector4(255, 111, 53, 255));
	particles->SetMaxParticles(60);
	particles->SetRateOverTime(25.0f);
	particles->SetShapeRadius(0.1875f);
	particles->SetStartForce(Vector3(0, 5, 0));
	particles->SetLifeTimeRotation(-15.0f, 15.0f);
	particles->SetTextureTiling(8, 8);
	particles->SetPlayTime(1, true);
	particles->Play();

	ObjectList.insert({ obj->Name, obj });
	return obj;
}


