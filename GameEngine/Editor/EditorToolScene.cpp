#include "EditorToolScene.h"
#include "GameObject.h"
#include "EaterEngineAPI.h"
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

std::map<std::string, GameObject*> EditorToolScene::ObjectList;
SceneSave* EditorToolScene::mSaveManager = nullptr;
Eater_LoadScene* EditorToolScene::mLoadManager = nullptr;
GameObject* EditorToolScene::CamObject = nullptr;
GameObject* EditorToolScene::DebugCamObject = nullptr;
std::map<int, std::string> EditorToolScene::TagList;
bool EditorToolScene::ThreadLoading = false;
EditorToolScene::EditorToolScene()
{

}

EditorToolScene::~EditorToolScene()
{

}

void EditorToolScene::Awake()
{
	ThreadLoading = false;
	mSaveManager = new SceneSave();
	mLoadManager = new Eater_LoadScene();
	mSaveManager->Initialize(&ObjectList);
	mLoadManager->Initialize(&ObjectList);

	RenderOption* Option = GetRenderOptionData();
	Option->DebugOption ^= DEBUG_RENDERTARGET;
	RenderSetting();

	//리소스를 로드
	Load("../Assets/Texture/ModelTexture");
	Load("../Assets/Texture/Terrain");
	Load("../Assets/Texture/Material");
	Load("../Assets/Texture/Particle");
	Load("../Assets/Texture/Environment");

	Load("../Assets/Model/Animation");
	Load("../Assets/Model/MeshBuffer");
	Load("../Assets/Model/ModelData");
	Load("../Assets/Model/Prefap");

	//기본 태그 설정
	DebugCamObject = GetMainCamera();
	TagList.insert({ 0, "Default" });
	TagList.insert({ 1, "MainCam" });
	TagList.insert({ 2, "Point" });
	TagList.insert({ 3, "Player" });

	//기본 오브젝트 셋팅
	GameObject* mCameraObject = FindGameObjectName("DebugCamera");
	GameObject* mDiractionObject = FindGameObjectName("DirectionLight");
	ObjectList.insert({ "DebugCamera",mCameraObject });
	ObjectList.insert({ "DirectionLight",mDiractionObject });
}
void EditorToolScene::Update()
{
	if (ThreadRun == true)
	{
		return;
	}
	ThreadLoading = true;
}

void EditorToolScene::End()
{


}

void EditorToolScene::ThreadFunction()
{
	//BakeSkyLightMap("Day");
	BakeSkyLightMap("HDRI_2_CubeMap", false);

	//SetSkyLight("Day");
	SetSkyLight("HDRI_2_CubeMap");
	//SetEnvironment("Day");
	SetEnvironment("HDRI_2_CubeMap");
}

GameObject* EditorToolScene::Create_GameObject()
{
	GameObject* Object = Instance("None_GameObject");
	Object->Name = FindMeshName("None_GameObject");
	ObjectList.insert({ Object->Name,Object });
	return Object;
}

GameObject* EditorToolScene::Create_Object(std::string MeshName)
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

GameObject* EditorToolScene::Create_Camera()
{
	GameObject* Cam = InstanceCamera("Camera");
	Cam->Name = FindMeshName("Camera");
	ObjectList.insert({ Cam->Name, Cam });
	return Cam;
}

GameObject* EditorToolScene::Create_Box()
{
	GameObject* box = Instance("box");
	MeshFilter* mMeshFileter = box->AddComponent<MeshFilter>();
	mMeshFileter->SetMeshName("box_0");
	mMeshFileter->SetModelName("box");
	box->Name = FindMeshName("box");
	ObjectList.insert({ box->Name,box });
	return box;
}

GameObject* EditorToolScene::Create_Sphere()
{
	return nullptr;
}

GameObject* EditorToolScene::CreateBaseObject(std::string ObjectName, std::string MeshName)
{
	GameObject* Object = Instance(ObjectName);
	MeshFilter* mMeshFilter = Object->AddComponent<MeshFilter>();
	Transform* mTransform = Object->GetTransform();

	mMeshFilter->SetModelName(MeshName);
	ObjectList.insert({ Object->Name, Object });
	return Object;
}

GameObject* EditorToolScene::CreateSkinObject(std::string ObjectName, std::string MeshName)
{
	GameObject* Skin = Instance(ObjectName);
	MeshFilter* MF = Skin->AddComponent<MeshFilter>();
	Transform* TR = Skin->GetTransform();
	AnimationController* AC = Skin->AddComponent<AnimationController>();

	MF->SetModelName(MeshName);
	MF->SetAnimationName(MeshName);
	ObjectList.insert({ ObjectName, Skin });
	return Skin;
}

void EditorToolScene::MeshLoad(std::string Path, UINT Option)
{
	Load(Path, Option);
}

bool EditorToolScene::DeleteObject(std::string MeshName)
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

bool EditorToolScene::GetThreadLoading()
{
	return ThreadLoading;
}

void EditorToolScene::SaveScene(std::string SaveFilePath, std::string SaveFileName)
{
	mSaveManager->Scene_Save(SaveFilePath, SaveFileName);
}

void EditorToolScene::LoadScene(std::string LoadScenePath)
{
	std::string Path = "../Assets/Scene/" + LoadScenePath;
	mLoadManager->Load(Path);
}

void EditorToolScene::SavePrefap(std::string SaveFilePath, std::string SaveFileName, std::string ObjectName)
{
	mSaveManager->Prefap_Save(SaveFilePath, SaveFileName, FindMesh(ObjectName));
}

GameObject* EditorToolScene::FindMesh(std::string MeshName)
{
	return ObjectList[MeshName];
}

GameObject* EditorToolScene::FindMesh(std::string MeshName, std::string ParentName)
{
	GameObject* Parent = FindMesh(ParentName);
	GameObject* Child = Parent->GetChildObject(MeshName);
	return Child;
}

std::string EditorToolScene::FindMeshName(std::string MeshName)
{
	std::string ObjectName = MeshName;

	std::map<std::string, GameObject*>::iterator Start_it = ObjectList.end();

	int Meshindex = 0;
	while (true)
	{
		std::map<std::string, GameObject*>::iterator End_it = ObjectList.find(ObjectName);
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

GameObject* EditorToolScene::FindMainCamera()
{
	return GetMainCamera();
}

int EditorToolScene::AddTag(std::string TagName)
{
	int MaxIndex = 0;
	if (TagName == "")
	{
		return false;
	}

	//태그 리스트를 한번 순환
	std::map<int, std::string>::iterator Start_it = TagList.begin();
	std::map<int, std::string>::iterator End_it = TagList.end();
	for (Start_it; Start_it != End_it; Start_it++)
	{
		//이름이 겹치는게 있는지 체크한다
		if (Start_it->second == TagName)
		{
			return -1;
		}

		if (Start_it->first >= MaxIndex)
		{
			MaxIndex = Start_it->first;
		}
	}

	TagList.insert({ MaxIndex + 1,TagName });
	return MaxIndex + 1;
}

int EditorToolScene::DeleteTag(std::string TagName)
{
	int MaxIndex = 0;
	if (TagName == "")
	{
		return false;
	}
	//태그 리스트를 한번 순환
	std::map<int, std::string>::iterator Start_it = TagList.begin();
	std::map<int, std::string>::iterator End_it = TagList.end();

	for (Start_it; Start_it != End_it; Start_it++)
	{
		//이름이 겹치는게 있는지 체크한다
		if (Start_it->second == TagName)
		{
			MaxIndex = Start_it->first;
			TagList.erase(MaxIndex);
			return MaxIndex;
		}
	}
	return -1;
}

bool EditorToolScene::ChoiceTag(std::string TagName, GameObject* Obj)
{
	return false;
}

GameObject* EditorToolScene::Create_Terrain(std::string MeshPath, std::string mask01, std::string mask02)
{
	//터레인 생성
	LoadTerrainMesh("../Assets/Model/TerrainModel/Terrain.fbx", "../Assets/Texture/Terrain/Terrain_RGB_1.png", "../Assets/Texture/Terrain/Terrain_RGB_2.png", SCALING);

	GameObject* TerrainObect = InstanceTerrain("Terrain");

	Terrain* mTerrain = TerrainObect->GetComponent<Terrain>();
	mTerrain->SetLayerName("terrain_ground_A_BaseColor", "terrain_ground_A_Normal", "terrain_ground_A_ORM");
	mTerrain->SetLayerName("terrain_ground_B_BaseColor", "terrain_ground_B_Normal", "terrain_ground_B_ORM");
	mTerrain->SetLayerName("terrain_ground_C_BaseColor", "terrain_ground_C_Normal", "terrain_ground_C_ORM");
	mTerrain->SetLayerName("terrain_ground_D_BaseColor", "terrain_ground_D_Normal", "terrain_ground_D_ORM");
	mTerrain->SetMeshName("Terrain");
	//mTerrain->SetColliderName("TerrainDecimate");
	mTerrain->SetTextureTiling(31.0f);

	ObjectList.insert({ "Terrain",mTerrain->gameobject });
	return nullptr;
}

GameObject* EditorToolScene::Create_Light()
{
	GameObject* LightObject = InstanceLight("Light", LIGHT_TYPE::SPOT_LIGHT);
	LightObject->Name = FindMeshName(LightObject->Name);
	ObjectList.insert({ LightObject->Name, LightObject });
	return LightObject;
}

GameObject* EditorToolScene::Create_Particle()
{
	std::string Name = FindMeshName("Particle");

	GameObject* obj = InstanceParticle(Name);

	ParticleSystem* particles = obj->GetComponent<ParticleSystem>();
	particles->SetMeshName("Quad");
	particles->SetDiffuseName("particle_hotCloud");
	particles->SetRenderType(PARTICLE_RENDER_OPTION::BILLBOARD);
	particles->SetStartLifeTime(1.5f, 1.8f);
	particles->SetStartScale(1.0f, 7.0f);
	particles->SetStartRotation(-360, 360);
	particles->SetStartColor(Vector4(255, 174, 73, 28), Vector4(255, 111, 53, 255));
	particles->SetMaxParticles(60);
	particles->SetRateOverTime(25.0f);
	particles->SetShapeRadius(0.1875f);
	particles->SetStartForce(Vector3(0, 5, 0));
	particles->SetLifeTimeRotation(-15.0f, 15.0f);
	particles->SetLifeTimeScale(1, 1, PARTICLE_LIFETIME_OPTION::NONE);
	particles->SetLifeTimeColor(Vector4(255, 255, 255, 255), Vector4(255, 255, 255, 255), PARTICLE_LIFETIME_OPTION::NONE);
	particles->SetTextureTiling(8, 8);
	particles->SetPlayTime(1, true);
	particles->Play();

	ObjectList.insert({ obj->Name, obj });
	return obj;
}


