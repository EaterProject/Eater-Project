#include "Demo.h"
#include "GameObject.h"
#include "MainHeader.h"
#include "TypeOptionHeader.h"
#include "EngineData.h"
#include "SceneSave.h"

#include "Transform.h"
#include "MeshFilter.h"
#include "AnimationController.h"
#include "Terrain.h"
#include "ParticleSystem.h"
#include "Light.h"


std::map<std::string, GameObject*> Demo::ObjectList;
//std::map<std::string, AnimationController*> Demo::SkinList;

SceneSave*		Demo::SaveManager = nullptr;
GameObject*		Demo::Object		= nullptr;
Demo::Demo()
{
	Object = nullptr;
}
                                            
Demo::~Demo()
{

}

void Demo::Awake()
{
	SaveManager = new SceneSave();
	//텍스쳐를 로드
	Load("../Assets/Texture/Icon");
	Load("../Assets/Texture/Load");
	Load("../Assets/Texture/Particle");
	Load("../Assets/Texture/Terrain");
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
	Load("../Assets/Mesh/Base");
	Load("../Assets/Mesh/Animation");

	LoadEnvironment("../Assets/Texture/Base/Night.dds");
	SetEnvironment(true);
}

GameObject* Demo::CreateObject(std::string MeshName)
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
		Object = nullptr;
		return true;
	}
	else
	{
		return false;
	}
}

void Demo::DeleteScene()
{
	
}

void Demo::SaveScene(std::string SaveFilePath,std::string SaveFileName)
{
	SaveManager->Save(SaveFilePath, SaveFileName,ObjectList);
}

void Demo::LoadScene(std::string LoadScenePath)
{
	DestroyAll();
	std::string Path = "../Assets/Scene/" + LoadScenePath;
	SaveManager->Load(Path);
}

GameObject* Demo::FindMesh(std::string MeshName)
{
	return ObjectList[MeshName];
}

std::string Demo::FindMeshName(std::string MeshName)
{
	std::string ObjectName = MeshName;
	std::map<std::string, GameObject*>::iterator Start_it	= ObjectList.end();
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

GameObject* Demo::CreateBaseObject(std::string ObjectName, std::string MeshName)
{
	GameObject* Object	= Instance(ObjectName);
	MeshFilter* mMeshFilter = Object->AddComponent<MeshFilter>();
	Transform* mTransform = Object->GetTransform();

	mMeshFilter->SetMeshName(MeshName);
	ObjectList.insert({ ObjectName, Object });
	return Object;
}

GameObject* Demo::CreateSkinObject(std::string ObjectName, std::string MeshName)
{
	GameObject* Skin		= Instance(ObjectName);
	MeshFilter* MF			= Skin->AddComponent<MeshFilter>();
	Transform* TR			= Skin->GetTransform();
	AnimationController* AC = Skin->AddComponent<AnimationController>();

	std::size_t Start = 0;
	std::size_t End = MeshName.rfind('+');
	std::string SkinMeshName = MeshName.substr(Start, End);

	
	MF->SetMeshName(SkinMeshName);
	MF->SetAnimationName(SkinMeshName);
	ObjectList.insert({ ObjectName,Skin });
	return Skin;
}

GameObject* Demo::CreateTerrain(std::string MeshName)
{
	LoadTerrainMesh("../Assets/Mesh/Terrain/Terrain.fbx", "../Resources/Texture/Terrain/Terrain_RGB.png", SCALING);
	GameObject* TerrainObect = InstanceTerrain("Terrain");

	Terrain* mTerrain = TerrainObect->GetComponent<Terrain>();
	mTerrain->SetLayerName("terrain_ground_A_BaseColor", "terrain_ground_A_Normal", "terrain_ground_A_ORM");
	mTerrain->SetLayerName("terrain_ground_B_BaseColor", "terrain_ground_B_Normal", "terrain_ground_B_ORM");
	mTerrain->SetMeshName("Terrain");
	//mTerrain->SetColliderName("TerrainDecimate");
	mTerrain->SetTextureTiling(1.0f / 31.0f);
	return TerrainObect;
}

GameObject* Demo::CreateLight()
{
	return nullptr;
}

GameObject* Demo::CreateParticle()
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
