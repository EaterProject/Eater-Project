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
	Option->DebugOption = DEBUG_OPTION::DEBUG_MODE;
	RenderSetting();

	//���ҽ��� �ε�
	Load("../Assets/Texture/ModelTexture");
	Load("../Assets/Texture/Terrain");
	Load("../Assets/Texture/Material");
	Load("../Assets/Texture/Particle");
	Load("../Assets/Texture/Environment");
	Load("../Assets/Sound/BGM");
	Load("../Assets/Sound/SFX");

	Load("../Assets/Model/Animation");
	Load("../Assets/Model/MeshBuffer");
	Load("../Assets/Model/ModelData");
	Load("../Assets/Model/Prefap");
	Load("../Assets/Model/Particle");

	
	
	//�⺻ �±� ����
	DebugCamObject = GetMainCamera();
	TagList.insert({ 0, "Default" });
	TagList.insert({ 1, "MainCam" });
	TagList.insert({ 2, "Point" });
	TagList.insert({ 3, "Player" });

	//�⺻ ������Ʈ ����
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
	BakeSkyLightMap("SkyLight_0", false);
	BakeSkyLightMap("SkyLight_1", false);

	SetSkyLight("SkyLight_0", 0);
	SetSkyLight("SkyLight_1", 1);

	SetSkyCube("SkyCube");
}

GameObject* EditorToolScene::Create_GameObject()
{
	GameObject* Object	= Instance("None_GameObject");
	Object->Name		= FindMeshName("None_GameObject");
	ObjectList.insert({ Object->Name,Object });
	return Object;
}

GameObject* EditorToolScene::Create_Object(std::string MeshName)
{
	std::string ObjectName = FindMeshName(MeshName);

	if (MeshName.rfind('+') == std::string::npos)
	{
		//�⺻ �޽� ����
		return CreateBaseObject(ObjectName, MeshName);
	}
	else
	{
		//��Ű�� �Ž� ����
		return CreateSkinObject(ObjectName, MeshName);
	}
}

GameObject* EditorToolScene::Create_Camera()
{
	GameObject* Cam = Instance_Camera("Camera");
	Cam->Name = FindMeshName("Camera");
	ObjectList.insert({ Cam->Name, Cam });
	return Cam;
}

GameObject* EditorToolScene::Create_Particle(std::string Name)
{
	GameObject* mParticle	= Instance_Particle("Particle",Name);
	mParticle->Name			= FindMeshName(Name);

	ParticleSystem* system = mParticle->GetComponent<ParticleSystem>();
	system->Play();

	ObjectList.insert({ mParticle->Name, mParticle });

	return mParticle;
}

GameObject* EditorToolScene::Create_Box()
{
	GameObject* box = Instance("Box");
	MeshFilter* mMeshFileter = box->AddComponent<MeshFilter>();
	mMeshFileter->SetModelName("Box");
	box->Name = FindMeshName("Box");
	ObjectList.insert({ box->Name,box });
	return box;
}

GameObject* EditorToolScene::Create_Sphere()
{
	GameObject* box = Instance("Sphere");
	MeshFilter* mMeshFileter = box->AddComponent<MeshFilter>();
	mMeshFileter->SetModelName("Sphere");
	box->Name = FindMeshName("Sphere");
	ObjectList.insert({ box->Name,box });
	return box;
}

GameObject* EditorToolScene::Create_MonKey()
{
	GameObject* box = Instance("Monkey");
	MeshFilter* mMeshFileter = box->AddComponent<MeshFilter>();
	mMeshFileter->SetModelName("Monkey");
	box->Name = FindMeshName("Monkey");
	ObjectList.insert({ box->Name,box });
	return box;
}

GameObject* EditorToolScene::Create_Grid()
{
	GameObject* box = Instance("Grid");
	MeshFilter* mMeshFileter = box->AddComponent<MeshFilter>();
	mMeshFileter->SetMeshName("Grid_0");
	mMeshFileter->SetModelName("Grid");
	box->Name = FindMeshName("Grid");
	ObjectList.insert({ box->Name,box });
	return box;
}

GameObject* EditorToolScene::Create_Cylinder()
{
	GameObject* box = Instance("Cylinder");
	MeshFilter* mMeshFileter = box->AddComponent<MeshFilter>();
	mMeshFileter->SetMeshName("Cylinder_0");
	mMeshFileter->SetModelName("Cylinder");
	box->Name = FindMeshName("Cylinder");
	ObjectList.insert({ box->Name,box });
	return box;
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

void EditorToolScene::LoadParticle(std::string SaveFilePath, std::string SaveFileName, std::string ObjectName)
{
	
}

GameObject* EditorToolScene::FindMesh(std::string MeshName)
{
	return ObjectList[MeshName];
}

GameObject* EditorToolScene::FindMeshChild(std::string MeshName, std::string ParentName)
{
	GameObject* Parent = FindMesh(ParentName);
	GameObject* Child = Parent->GetChildObject(MeshName);
	return Child;
}

GameObject* EditorToolScene::FindMeshParent(std::string MeshName, std::string ParentName)
{
	GameObject* Parent = FindMesh(ParentName);
	return Parent;
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

	//�±� ����Ʈ�� �ѹ� ��ȯ
	std::map<int, std::string>::iterator Start_it = TagList.begin();
	std::map<int, std::string>::iterator End_it = TagList.end();
	for (Start_it; Start_it != End_it; Start_it++)
	{
		//�̸��� ��ġ�°� �ִ��� üũ�Ѵ�
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
	//�±� ����Ʈ�� �ѹ� ��ȯ
	std::map<int, std::string>::iterator Start_it = TagList.begin();
	std::map<int, std::string>::iterator End_it = TagList.end();

	for (Start_it; Start_it != End_it; Start_it++)
	{
		//�̸��� ��ġ�°� �ִ��� üũ�Ѵ�
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
	//�ͷ��� ����
	LoadTerrainMesh("../Assets/Model/TerrainModel/Terrain.fbx", "../Assets/Texture/Terrain/Terrain_RGB_1.png", "../Assets/Texture/Terrain/Terrain_RGB_2.png", SCALING);

	GameObject* TerrainObect = Instance_Terrain("Terrain");

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
	GameObject* LightObject = Instance_Light("Light", LIGHT_TYPE::SPOT_LIGHT);
	LightObject->Name = FindMeshName(LightObject->Name);
	ObjectList.insert({ LightObject->Name, LightObject });
	return LightObject;
}

GameObject* EditorToolScene::Create_BaseParticle()
{
	std::string Name = FindMeshName("Particle");

	GameObject* obj = Instance_Particle(Name);

	ParticleSystem* particles = obj->GetComponent<ParticleSystem>();
	particles->SetMeshName("Quad");
	particles->SetRenderType(PARTICLE_RENDER_OPTION::BILLBOARD);
	particles->SetDiffuseName("particle_hotCloud");
	particles->SetMaxParticles(60);
	particles->SetDelayTime(0.0f);
	particles->SetRateOverTime(25.0f);
	particles->SetShapeRadius(0.1875f, 0.1875f, 0.1875f);
	particles->SetTextureTiling(8, 8);
	particles->SetStartForce(Vector3(0, 5, 0));
	particles->SetStartColor(Vector4(255, 174, 73, 255), Vector4(255, 111, 53, 255));
	particles->SetStartLifeTime(1.5f, 1.8f);
	particles->SetStartScale(0.5f, 0.5f);
	particles->SetStartRotation(-360, 360);
	particles->SetLifeTimeForce(Vector3(0, 0, 0));
	particles->SetLifeTimeRotation(-15.0f, 15.0f);
	particles->SetLifeTimeScale(1, 1, PARTICLE_LIFETIME_OPTION::NONE);
	particles->SetLifeTimeColor(Vector4(255, 255, 255, 255), Vector4(255, 255, 255, 255), PARTICLE_LIFETIME_OPTION::NONE);
	particles->SetPlayTime(1);
	particles->SetLoop(true);
	particles->Play();

	ObjectList.insert({ obj->Name, obj });
	return obj;
}


