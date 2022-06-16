#include "EaterEngineAPI.h"
#include "TestScene.h"
#include "GameObject.h"
#include "MeshFilter.h"
#include "SkinningFilter.h"
#include "AnimationController.h"
#include "Transform.h"
#include "AI.h"
#include "Player.h"
#include "Camera.h"
#include "Light.h"
#include "Terrain.h"
#include "Rigidbody.h"
#include "ClientNetworkManager.h"
#include "PhysData.h"
#include "PortIPDefine.h"
#include "PlayerCamera.h"
#include "Player.h"
#include "ParticleSystem.h"
#include "Collider.h"
#include "Image.h"
#include "Button.h"
#include "RectTransform.h"
#include "EngineData.h"

#include "./Profiler/Profiler.h"

void TestScene::Awake()
{
	LoadTerrainMesh("../Assets/Model/TerrainModel/Terrain.fbx", "../Assets/Texture/Terrain/Terrain_RGB_1.png", "../Assets/Texture/Terrain/Terrain_RGB_2.png", SCALING);
	
	//Load("../Assets/Texture/Particle/particle_hotCloud.png");

	PROFILE_TIMER_START(PROFILE_OUTPUT::CONSOLE, 1, "Load Folder");
	//Load("../Assets/Texture/Terrain");
	//Load("../Assets/Texture/Environment");
	//Load("../Assets/Texture/Bake");
	//Load("../Assets/Texture/Particle");
	//Load("../Assets/Texture/ModelTexture");
	//Load("../Assets/Texture/Material");
	//Load("../Assets/Texture/UI");
	//Load("../Assets/Model/MeshBuffer");
	//Load("../Assets/Model/ModelData");
	//Load("../Assets/Model/Animation");
	PROFILE_TIMER_END("Load Folder"); 

	BakeSkyLightMap("SkyLight_HDRI", false);
	//BakeSkyLightMap("Day", false);
	//BakeSkyLightMap("Night", false);
	//BakeSkyLightMap("skybox1", false);
	//BakeSkyLightMap("TestSky", false);

	//AddOccluder("Dome_Occluder_0");


	CreateMap();

	//CreateUI();


	//CreateParticle(0,0,0);
	//SetSkyLight("Day");
	SetSkyLight("SkyLight_HDRI", 0);
	//SetSkyCube("Day");
	SetSkyCube("SkyCube_HDRI");

	//Load("../Assets/Scene/test.Scene");
}

void TestScene::Update()
{
	//PROFILE_TIMER_START(PROFILE_OUTPUT::CONSOLE, "Update", 60);
	
	//static int renderCount = 0;
	//PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "Update Count %d", renderCount++);
	//PROFILE_LOG(PROFILE_OUTPUT::VS_CODE, "Update Count %d", renderCount++);

	

	ChangeCubeMap();
	//DebugDrawLine(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 50.0f, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	
	//PROFILE_TIMER_END("Update");
}

void TestScene::End()
{

}

void TestScene::CreateMap()
{
	Transform* Tr = nullptr;
	MeshFilter* filter = nullptr;
	Light* light = nullptr;

	//
	//ParticleObj = Instance();
	//ParticleObj->GetTransform()->Position.y += 50;
	//Object = InstanceParticle("Particle12");
	//ParticleObj->ChoiceChild(Object);
	//ParticleSystem* particles = Object->GetComponent<ParticleSystem>();
	//particles->SetMeshName("Quad");
	//particles->SetRenderType(PARTICLE_RENDER_OPTION::VERTICAL_BILLBOARD);
	//particles->SetDiffuseName("particle_hotCloud");
	//particles->SetStartLifeTime(1.5f, 1.8f);
	//particles->SetStartScale(4.0f, 7.0f);
	//particles->SetStartRotation(-360, 360);
	//particles->SetStartColor(Vector4(255, 174, 73, 28), Vector4(255, 111, 53, 255));
	//particles->SetMaxParticles(60);
	//particles->SetRateOverTime(25.0f);
	//particles->SetShapeRadius(0.1875f);
	//particles->SetStartForce(Vector3(0, 5, 0));
	//particles->SetLifeTimeRotation(-15.0f, 15.0f);
	//particles->SetTextureTiling(8, 8);
	//particles->SetPlayTime(1, false);
	//particles->Play();

	//Object = InstanceLight("Light", LIGHT_TYPE::POINT_LIGHT);
	//Object->GetTransform()->Position.x += 10.0f;
	//Object->GetTransform()->Position.y += 10.0f;
	//
	//Object = InstanceLight("Light", LIGHT_TYPE::SPOT_LIGHT);
	//Object->GetTransform()->Position.x -= 10.0f;
	//Object->GetTransform()->Position.y += 10.0f;

	Object = Instance();
	filter = Object->AddComponent<MeshFilter>();
	filter->SetModelName("Sphere1");
	Object->GetTransform()->Position.x -= 30.0f;

	Object = Instance();
	meshfilter = Object->AddComponent<MeshFilter>();
	meshfilter->SetModelName("Sphere1");
	Object->GetTransform()->Position.x -= 10.0f;

	Object = Instance();
	filter = Object->AddComponent<MeshFilter>();
	filter->SetModelName("Sphere1");
	Object->GetTransform()->Position.x += 10.0f;

	Object = Instance();
	filter = Object->AddComponent<MeshFilter>();
	filter->SetModelName("Sphere1");
	Object->GetTransform()->Position.x += 30.0f;


	//Object = Instance();
	//filter = Object->AddComponent<MeshFilter>();
	//filter->SetModelName("BossB");
	//filter->SetAnimationName("BossB");
	//Object->GetTransform()->Position.x -= 30;
	//AC = Object->AddComponent<AnimationController>();
	//AC->Choice("weak");
	//
	//Object = Instance();
	//filter = Object->AddComponent<MeshFilter>();
	//filter->SetModelName("BossB");
	//filter->SetAnimationName("BossB");
	//AC = Object->AddComponent<AnimationController>();
	//AC->Choice("weak");
	//
	//Object = Instance();
	//filter = Object->AddComponent<MeshFilter>();
	//filter->SetModelName("BossB");
	//filter->SetAnimationName("BossB");
	//Object->GetTransform()->Position.x += 30;
	//AC = Object->AddComponent<AnimationController>();
	//AC->Choice("weak");

	//Object = Instance();
	//filter = Object->AddComponent<MeshFilter>();
	//filter->SetModelName("weapone");
	//filter->SetAnimationName("weapone");
	//Object->GetTransform()->Position.z += 20;
	//AC = Object->AddComponent<AnimationController>();
	//AC->Choice("attack");
	
	//Object = Instance();
	//filter = Object->AddComponent<MeshFilter>();
	//filter->SetModelName("MonsterA");
	//filter->SetAnimationName("MonsterA");
	//Object->GetTransform()->Scale = { 10.0f, 10.0f, 10.0f };
	//Object->GetTransform()->Position.z -= 20;
	//AC = Object->AddComponent<AnimationController>();
	//AC->Choice("die");
	
	//Object = Instance();
	//filter = Object->AddComponent<MeshFilter>();
	//filter->SetModelName("MonsterA");
	//filter->SetAnimationName("MonsterA");
	//Object->GetTransform()->Position.x += 20;
	//Object->GetTransform()->Position.z -= 20;
	//AC = Object->AddComponent<AnimationController>();
	//AC->Choice("idle");
	
	//Object = Instance();
	//filter = Object->AddComponent<MeshFilter>();
	//filter->SetModelName("Dome_Occluder");
	//
	//Object = Instance();
	//filter = Object->AddComponent<MeshFilter>();
	//filter->SetModelName("Dome");
	//
	//Object = Instance();
	//filter = Object->AddComponent<MeshFilter>();
	//filter->SetModelName("Outside_Grass");

	//Object = Instance();
	//filter = Object->AddComponent<MeshFilter>();
	//filter->SetModelName("organic_cactus");

	//Object = Instance();
	//filter = Object->AddComponent<MeshFilter>();
	//filter->SetModelName("Outside_Rock");
	//
	//Object = Instance();
	//filter = Object->AddComponent<MeshFilter>();
	//filter->SetModelName("Outside_bossOBJ");
	//
	//Object = Instance();
	//filter = Object->AddComponent<MeshFilter>();
	//filter->SetModelName("Outside_Other");
	//
	//Object = Instance();
	//filter = Object->AddComponent<MeshFilter>();
	//filter->SetModelName("Outside_Pebble");

	testobj = Instance_Terrain("Terrain");
	Terrain* mTerrain = testobj->GetComponent<Terrain>();
	mTerrain->SetLayerName("terrain_ground_A_BaseColor", "terrain_ground_A_Normal", "terrain_ground_A_ORM");
	mTerrain->SetLayerName("terrain_ground_B_BaseColor", "terrain_ground_B_Normal", "terrain_ground_B_ORM");
	mTerrain->SetLayerName("terrain_ground_C_BaseColor", "terrain_ground_C_Normal", "terrain_ground_C_ORM");
	mTerrain->SetLayerName("terrain_ground_D_BaseColor", "terrain_ground_D_Normal", "terrain_ground_D_ORM");
	mTerrain->SetMeshName("Terrain");
	mTerrain->SetTextureTiling(31.0f);
}

void TestScene::CreateUI()
{
	GameObject* ui_object = nullptr;
	Image* ui_image = nullptr;
	RectTransform* ui_rectTR = nullptr;

	ui_object = InstanceUI("UI");
	ui_image = ui_object->AddComponent<Image>();
	ui_image->SetTexture("ingame_minimap");
	ui_rectTR = ui_object->GetComponent<RectTransform>();
	ui_rectTR->SetPivot(RECT_PIVOT::PIVOT_RIGHT_TOP);
	ui_rectTR->AddPosition(25.0f, 75.0f);
	ui_rectTR->SetRotation(0.0f, 0.0f, -90.0f);
	ui_rectTR->SetScale(0.5f, 0.5f);

	ui_object = InstanceUI("UI");
	ui_image = ui_object->AddComponent<Image>();
	ui_image->SetTexture("ingame_combo");

	ui_rectTR = ui_object->GetComponent<RectTransform>();
	ui_rectTR->SetPivot(RECT_PIVOT::PIVOT_MIDDLE_RIGHT);
	ui_rectTR->AddPosition(-25.0f, 0.0f);
	ui_rectTR->SetScale(0.5f, 0.5f);

	ui_object = InstanceUI("UI");
	ui_image = ui_object->AddComponent<Image>();
	ui_image->SetTexture("ingame_playercounter_1");

	ui_rectTR = ui_object->GetComponent<RectTransform>();
	ui_rectTR->SetPivot(RECT_PIVOT::PIVOT_MIDDLE_CENTER);
	ui_rectTR->AddPosition(-50.0f, 0.0f);
	ui_rectTR->SetScale(0.5f, 0.5f);

	ui_object = InstanceUI("UI");
	ui_image = ui_object->AddComponent<Image>();
	ui_image->SetTexture("ingame_player_hp_back");

	ui_rectTR = ui_object->GetComponent<RectTransform>();
	ui_rectTR->SetPivot(RECT_PIVOT::PIVOT_LEFT_BOTTOM);
	ui_rectTR->AddPosition(50.0f, -50.0f);
	ui_rectTR->SetScale(0.5f, 0.5f);


	ui_object = InstanceUI("UI");
	ui_image = ui_object->AddComponent<Image>();
	ui_image->SetTexture("ingame_player_hp");
	ui_image->SetImageColor(255, 0, 0);
	ui_rectTR = ui_object->GetComponent<RectTransform>();
	ui_rectTR->SetPivot(RECT_PIVOT::PIVOT_LEFT_BOTTOM);
	ui_rectTR->AddPosition(50.0f, -50.0f);
	ui_rectTR->SetScale(0.5f, 0.5f);

	ui_object = InstanceUI("UI");
	ui_image = ui_object->AddComponent<Image>();
	ui_image->SetTexture("ingame_switch");
	ui_rectTR = ui_object->GetComponent<RectTransform>();
	ui_rectTR->SetPivot(RECT_PIVOT::PIVOT_LEFT_BOTTOM);
	ui_rectTR->AddPosition(50.0f, -150.0f);
	ui_rectTR->SetScale(0.5f, 0.5f);

	ui_object = InstanceUI("UI");
	ui_image = ui_object->AddComponent<Image>();
	ui_image->SetTexture("ingame_skill_rb");
	ui_rectTR = ui_object->GetComponent<RectTransform>();
	ui_rectTR->SetPivot(RECT_PIVOT::PIVOT_LEFT_BOTTOM);
	ui_rectTR->AddPosition(400.0f, -50.0f);
	ui_rectTR->SetScale(0.5f, 0.5f);

	ui_object = InstanceUI("UI");
	ui_image = ui_object->AddComponent<Image>();
	ui_image->SetTexture("ingame_skill_rb");
	ui_rectTR = ui_object->GetComponent<RectTransform>();
	ui_rectTR->SetPivot(RECT_PIVOT::PIVOT_LEFT_BOTTOM);
	ui_rectTR->AddPosition(400.0f, -50.0f);
	ui_rectTR->SetScale(0.5f, 0.5f);

	ui_object = InstanceUI("UI");
	ui_image = ui_object->AddComponent<Image>();
	ui_image->SetTexture("ingame_skill_space");
	ui_rectTR = ui_object->GetComponent<RectTransform>();
	ui_rectTR->SetPivot(RECT_PIVOT::PIVOT_LEFT_BOTTOM);
	ui_rectTR->AddPosition(475.0f, -50.0f);
	ui_rectTR->SetScale(0.5f, 0.5f);

	ui_object = InstanceUI("UI");
	ui_image = ui_object->AddComponent<Image>();
	ui_image->SetTexture("ingame_skill_e");
	ui_rectTR = ui_object->GetComponent<RectTransform>();
	ui_rectTR->SetPivot(RECT_PIVOT::PIVOT_LEFT_BOTTOM);
	ui_rectTR->AddPosition(550.0f, -50.0f);
	ui_rectTR->SetScale(0.5f, 0.5f);

	//ui_object = InstanceUI("UI");
	//ui_image = ui_object->AddComponent<Image>();
	//ui_image->SetTexture("ingame_player_hp");
	//
	//ui_rectTR = ui_object->GetComponent<RectTransform>();
	//ui_rectTR->SetPivot(RECT_PIVOT::PIVOT_LEFT_TOP);
	//
	//ui_object = InstanceUI("UI");
	//ui_image = ui_object->AddComponent<Image>();
	//ui_image->SetTexture("ingame_player_hp_back");
	//
	//ui_rectTR = ui_object->GetComponent<RectTransform>();
	//ui_rectTR->SetPivot(RECT_PIVOT::PIVOT_LEFT_BOTTOM);
	//
	//ui_object = InstanceUI("UI");
	//ui_image = ui_object->AddComponent<Image>();
	//ui_image->SetTexture("ingame_player_hp_back");
	//
	//ui_rectTR = ui_object->GetComponent<RectTransform>();
	//ui_rectTR->SetPivot(RECT_PIVOT::PIVOT_RIGHT_TOP);
	//ui_rectTR->AddPosition(-10.0f, 0.0f);
	//ui_rectTR->SetRotation(0.0f, 0.0f, 90.0f);
	//ui_rectTR->SetScale(0.5f, 0.5f);
	//
	//ui_object = InstanceUI("UI");
	//ui_image = ui_object->AddComponent<Image>();
	//ui_image->SetTexture("ingame_player_hp_back");
	//
	//ui_rectTR = ui_object->GetComponent<RectTransform>();
	//ui_rectTR->SetPivot(RECT_PIVOT::PIVOT_RIGHT_BOTTOM);
	//ui_rectTR->AddPosition(-10.0f, 0.0f);
	//
	//
	//ui_object = InstanceUI("UI");
	//ui_image = ui_object->AddComponent<Image>();
	//ui_image->SetTexture("ingame_minimap");
	//
	//ui_rectTR = ui_object->GetComponent<RectTransform>();
	//ui_rectTR->SetPivot(RECT_PIVOT::PIVOT_MIDDLE_TOP);
	//
	//ui_object = InstanceUI("UI");
	//ui_image = ui_object->AddComponent<Image>();
	//ui_image->SetTexture("ingame_minimap");
	//
	//ui_rectTR = ui_object->GetComponent<RectTransform>();
	//ui_rectTR->SetPivot(RECT_PIVOT::PIVOT_MIDDLE_BOTTOM);
	//
	//ui_object = InstanceUI("UI");
	//ui_image = ui_object->AddComponent<Image>();
	//ui_image->SetTexture("ingame_minimap");
	//
	//ui_rectTR = ui_object->GetComponent<RectTransform>();
	//ui_rectTR->SetPivot(RECT_PIVOT::PIVOT_MIDDLE_LEFT);
	//
	//ui_object = InstanceUI("UI");
	//ui_image = ui_object->AddComponent<Image>();
	//ui_image->SetTexture("ingame_minimap");
	//
	//ui_rectTR = ui_object->GetComponent<RectTransform>();
	//ui_rectTR->SetPivot(RECT_PIVOT::PIVOT_MIDDLE_RIGHT);
	//
	//ui_object = InstanceUI("UI");
	//ui_image = ui_object->AddComponent<Image>();
	//ui_image->SetTexture("ingame_minimap");
	//
	//ui_rectTR = ui_object->GetComponent<RectTransform>();
	//ui_rectTR->SetPivot(RECT_PIVOT::PIVOT_MIDDLE_CENTER);
}

void TestScene::CreateParticle(float x, float y, float z)
{
	ParticleObj = Instance();
	ParticleObj->GetTransform()->Position = { -10.0f + x, 2.5f + y, -10.0f + z };
	testobj = Instance_Particle("Particle1");
	ParticleObj->ChoiceChild(testobj);
	ParticleSystem* particles = testobj->GetComponent<ParticleSystem>();
	particles->SetMeshName("Quad");
	particles->SetRenderType(PARTICLE_RENDER_OPTION::BILLBOARD);
	particles->SetDiffuseName("particle_hotCloud");
	particles->SetStartLifeTime(1.5f, 1.8f);
	particles->SetStartScale(4.0f, 7.0f);
	particles->SetStartRotation(-360, 360);
	particles->SetStartColor(Vector4(43, 74, 255, 120), Vector4(23, 111, 255, 255));
	particles->SetMaxParticles(30);
	particles->SetRateOverTime(15.0f);
	particles->SetShapeRadius(0.1875f);
	particles->SetStartForce(Vector3(0, 5, 0));
	particles->SetLifeTimeRotation(-15.0f, 15.0f);
	particles->SetLifeTimeColor(Vector4(255, 255, 255, 50), Vector4(255, 255, 255, 255), PARTICLE_LIFETIME_OPTION::UPDOWN);
	particles->SetTextureTiling(8, 8);
	particles->SetPlayTime(10.0f, true);
	particles->Play();

	testobj = Instance_Particle("Particle2");
	ParticleObj->ChoiceChild(testobj);
	particles = testobj->GetComponent<ParticleSystem>();
	particles->SetMeshName("Quad");
	particles->SetDiffuseName("particle_blast4");
	particles->SetStartLifeTime(1.0f, 2.0f);
	particles->SetStartScale(7.0f, 9.125f);
	particles->SetStartRotation(-360, 360);
	particles->SetStartColor(Vector4(25, 25, 25, 50));
	particles->SetMaxParticles(20);
	particles->SetRateOverTime(10.0f);
	particles->SetShapeRadius(0.1f);
	particles->SetStartForce(Vector3(0, 7.5f, 0));
	particles->SetLifeTimeScale(0.25f, 1.0f, PARTICLE_LIFETIME_OPTION::UP);
	particles->SetLifeTimeColor(Vector4(255, 255, 255, 0), Vector4(255, 255, 255, 255), PARTICLE_LIFETIME_OPTION::UPDOWN);
	particles->SetLifeTimeRotation(-15.0f, 15.0f);
	particles->SetTextureTiling(8, 8);
	particles->SetPlayTime(10.0f, true);
	particles->Play(true);

	ParticleObj = Instance();
	ParticleObj->GetTransform()->Position = { 10.0f + x, 2.5f + y, -10.0f + z };
	testobj = Instance_Particle("Particle1");
	ParticleObj->ChoiceChild(testobj);
	particles = testobj->GetComponent<ParticleSystem>();
	particles->SetMeshName("Quad");
	particles->SetRenderType(PARTICLE_RENDER_OPTION::VERTICAL_BILLBOARD);
	particles->SetDiffuseName("particle_hotCloud");
	particles->SetStartLifeTime(1.5f, 1.8f);
	particles->SetStartScale(4.0f, 7.0f);
	particles->SetStartRotation(-360, 360);
	particles->SetStartColor(Vector4(255, 74, 43, 120), Vector4(255, 111, 23, 255));
	particles->SetMaxParticles(30);
	particles->SetRateOverTime(15.0f);
	particles->SetShapeRadius(0.1875f);
	particles->SetStartForce(Vector3(0, 5, 0));
	particles->SetLifeTimeRotation(-15.0f, 15.0f);
	particles->SetLifeTimeColor(Vector4(255, 255, 255, 50), Vector4(255, 255, 255, 255), PARTICLE_LIFETIME_OPTION::UPDOWN);
	particles->SetTextureTiling(8, 8);
	particles->SetPlayTime(10.0f, true);
	particles->Play();

	testobj = Instance_Particle("Particle2");
	ParticleObj->ChoiceChild(testobj);
	particles = testobj->GetComponent<ParticleSystem>();
	particles->SetMeshName("Quad");
	particles->SetDiffuseName("particle_blast4");
	particles->SetStartLifeTime(1.0f, 2.0f);
	particles->SetStartScale(7.0f, 9.125f);
	particles->SetStartRotation(-360, 360);
	particles->SetStartColor(Vector4(25, 25, 25, 50));
	particles->SetMaxParticles(20);
	particles->SetRateOverTime(10.0f);
	particles->SetShapeRadius(0.1f);
	particles->SetStartForce(Vector3(0, 7.5f, 0));
	particles->SetLifeTimeScale(0.25f, 1.0f, PARTICLE_LIFETIME_OPTION::UP);
	particles->SetLifeTimeColor(Vector4(255, 255, 255, 0), Vector4(255, 255, 255, 255), PARTICLE_LIFETIME_OPTION::UPDOWN);
	particles->SetLifeTimeRotation(-15.0f, 15.0f);
	particles->SetTextureTiling(8, 8);
	particles->SetPlayTime(10.0f, true);
	particles->Play(true);
}

void TestScene::ChangeCubeMap()
{

	if (GetKey('1'))
	{
		MaterialPropertyBlock* block = meshfilter->GetMaterialPropertyBlock();
	
		if (up)
		{
			block->LimLightColor.x += 0.005f;
		}
		else
		{
			block->LimLightColor.x -= 0.005f;
		}
	
		if (block->LimLightColor.x > 1.0f)
		{
			block->LimLightColor.x = 1.0f;
			up = false;
		}
		if (block->LimLightColor.x < 0.0f)
		{
			block->LimLightColor.x = 0.0f;
			up = true;
		}
	}
	if (GetKeyUp('2'))
	{
		meshfilter->SetMaterialPropertyBlock(true);
	}
	if (GetKeyUp('3'))
	{
		meshfilter->SetMaterialPropertyBlock(false);
	}
	if (GetKey('4'))
	{
		MaterialProperty* block = meshfilter->GetMaterialProperty();
		
		if (up)
		{
			block->LimLightColor.x += 0.005f;
		}
		else
		{
			block->LimLightColor.x -= 0.005f;
		}

		if (block->LimLightColor.x > 1.0f)
		{
			block->LimLightColor.x = 1.0f;
			up = false;
		}
		if (block->LimLightColor.x < 0.0f)
		{
			block->LimLightColor.x = 0.0f;
			up = true;
		}
	}
	//if (GetKeyUp('1'))
	//{
	//	SetSkyLight("SkyLight_HDRI");
	//}
	//if (GetKeyUp('2'))
	//{
	//	SetSkyLight("Day");
	//}
	//if (GetKeyUp('3'))
	//{
	//	SetSkyLight("Night");
	//}
	//if (GetKeyUp('4'))
	//{
	//	SetSkyLight("skybox1");
	//}
	//if (GetKeyUp('5'))
	//{
	//	SetSkyLight("TestSky");
	//}

}
