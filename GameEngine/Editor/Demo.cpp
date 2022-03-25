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
#include "Camera.h"

std::map<std::string, GameObject*> Demo::ObjectList;

SceneSave*		Demo::SaveManager = nullptr;
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
	SaveManager = new SceneSave();
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
	Object->SetName("GameObject");
	ObjectList.insert({ Object->GetName(), Object });
	return Object;
}

GameObject* Demo::Create_Object(std::string MeshName)
{
	if (MeshName.rfind('+') == std::string::npos)
	{
		//기본 메쉬 생성
		return CreateBaseObject(MeshName);
	}
	else
	{
		//스키닝 매쉬 생성
		return CreateSkinObject(MeshName);
	}
}

GameObject* Demo::Create_Camera()
{
	GameObject* Cam = InstanceCamera("Camera");
	ObjectList.insert({Cam->GetName(), Cam});
	return Cam;
}

GameObject* Demo::CreateBaseObject(std::string MeshName)
{
	GameObject* Object = Instance(MeshName);
	MeshFilter* mMeshFilter = Object->AddComponent<MeshFilter>();
	Transform* mTransform = Object->GetTransform();

	mMeshFilter->SetModelName(MeshName);
	std::string test = Object->GetName();
	ObjectList.insert({ test, Object });
	return Object;
}

GameObject* Demo::CreateSkinObject(std::string MeshName)
{
	GameObject* Skin = Instance(MeshName);
	MeshFilter* MF = Skin->AddComponent<MeshFilter>();
	Transform* TR = Skin->GetTransform();
	AnimationController* AC = Skin->AddComponent<AnimationController>();

	MF->SetModelName(MeshName);
	MF->SetAnimationName(MeshName);
	ObjectList.insert({ Skin->GetName(), Skin });
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
	SaveManager->Save(SaveFilePath, SaveFileName,ObjectList);
}

void Demo::LoadScene(std::string LoadScenePath)
{
	std::string Path = "../Assets/Scene/" + LoadScenePath;
	SaveManager->Load(Path);
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
	ObjectList.insert({ LightObject->GetName(), LightObject });
	return LightObject;
}

GameObject* Demo::Create_Particle()
{
	GameObject* obj = InstanceParticle("Particle");

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

	ObjectList.insert({ obj->GetName(), obj });
	return obj;
}


