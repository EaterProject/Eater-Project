#include "MainHeader.h"
#include "TestScene.h"
#include "GameObject.h"
#include "MeshFilter.h"
#include "SkinningFilter.h"
#include "AnimationController.h"
#include "Transform.h"
#include "AI.h"
#include "Player.h"
#include "KeyInput.h"
#include "Camera.h"
#include "Light.h"
#include "Terrain.h"
#include "Rigidbody.h"
#include "ClientNetworkManager.h"
#include "PhysData.h"
#include "PortIPDefine.h"
#include "PlayerCamera.h"
#include "TestPlayer.h"
#include "Monster.h"
#include "ParticleSystem.h"
#include "Collider.h"

void TestScene::Awake()
{
	LoadEnvironment("../Assets/Texture/Environment/Day.dds");
	LoadTerrainMesh("../Assets/Model/TerrainModel/Terrain.fbx", "../Assets/Texture/Terrain/Terrain_RGB.png", SCALING);

	Load("../Assets/Texture/ModelTexture");
	Load("../Assets/Texture/Terrain");
	//Load("../Assets/Texture/Particle");
	Load("../Assets/Texture/Material");
	Load("../Assets/Model/MeshBuffer");
	Load("../Assets/Model/ModelData");

	//CreateTestObject();
	CreateMap();
	//CreateParticle(0,0,0);

	SetEnvironment(true);
}

void TestScene::Update()
{
	ChangeCubeMap();
	//DebugDrawLine(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 50.0f, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
}

void TestScene::End()
{

}

void TestScene::CreateTestObject()
{
	MeshFilter* MF;
	for (int i = 0; i < 10; i++)
	{
		std::string num = std::to_string(9 - i);
	
		testobj = Instance("box");
		testobj->AddComponent<MeshFilter>()->SetModelName("TestSphere");
		MF = testobj->GetComponent<MeshFilter>();
		MF->SetDiffuseTextureName("Albedo_" + num);
		MF->SetNormalTextureName("Normal_" + num);
		MF->SetORMTextureName("ORM_" + num);
		testobj->GetTransform()->Position = { -150.0f + 30.0f * i, 150.0f, 0.0f };
		testobj->GetTransform()->Rotation = { 90.0f, 0.0f, 0.0f };
		testobj->GetTransform()->Scale = { 5.0f, 5.0f, 5.0f };
	}


	//testobj = Instance("LampA");
	//testobj->AddComponent<MeshFilter>()->SetModelName("LampA");
	//testobj->GetTransform()->Position = { 0.0f, 85.0f, 0.0f };
	//testobj->GetTransform()->Scale = { 10.0f, 10.0f, 10.0f };
	//testobj->GetTransform()->Rotation = { 0.0f, 90.0f, 0.0f };
}

void TestScene::CreateMap()
{
	Transform* Tr = nullptr;
	MeshFilter* filter = nullptr;
	Light* light = nullptr;

	Object = Instance();
	Object->GetTransform()->Scale = { 0.1f, 0.1f, 0.1f };
	filter = Object->AddComponent<MeshFilter>();
	filter->SetModelName("bossB");

	Object = InstanceLight("Light4", SPOT_LIGHT);
	Object->GetTransform()->Position.y += 10;
	Object->GetTransform()->Position.z += 30;
	light = Object->GetComponent<Light>();
	light->SetRange(10.0f);
	light->SetPower(10.0f);
	light->SetColor(1.0f, 0.0f, 0.0f);
	light->SetInAngle(15.0f);
	light->SetOutAngle(25.0f);

	Object = InstanceLight("Light5", SPOT_LIGHT);
	Object->GetTransform()->Position.y += 10;
	Object->GetTransform()->Position.x += 10;
	Object->GetTransform()->Position.z += 30;
	light = Object->GetComponent<Light>();
	light->SetRange(10.0f);
	light->SetPower(10.0f);
	light->SetColor(1.0f, 1.0f, 0.0f);
	light->SetInAngle(15.0f);
	light->SetOutAngle(25.0f);
	
	Object = InstanceLight("Light6", SPOT_LIGHT);
	Object->GetTransform()->Position.y += 10;
	Object->GetTransform()->Position.x -= 10;
	Object->GetTransform()->Position.z += 30;
	light = Object->GetComponent<Light>();
	light->SetRange(10.0f);
	light->SetPower(10.0f);
	light->SetColor(1.0f, 0.0f, 1.0f);
	light->SetInAngle(15.0f);
	light->SetOutAngle(25.0f);
	
	Object1 = InstanceLight("Light", POINT_LIGHT);
	Object1->GetTransform()->Position.y += 10;
	light = Object1->GetComponent<Light>();
	light->SetRange(10.0f);
	light->SetPower(10.0f);
	light->SetColor(1.0f, 0.0f, 0.0f);

	Object1 = InstanceLight("Light1", POINT_LIGHT);
	Object1->GetTransform()->Position.y += 10;
	Object1->GetTransform()->Position.x += 10;
	light = Object1->GetComponent<Light>();
	light->SetRange(10.0f);
	light->SetPower(10.0f);
	light->SetColor(0.0f, 1.0f, 0.0f);

	Object1 = InstanceLight("Light2", POINT_LIGHT);
	Object1->GetTransform()->Position.y += 10;
	Object1->GetTransform()->Position.x -= 10;
	light = Object1->GetComponent<Light>();
	light->SetRange(10.0f);
	light->SetPower(10.0f);
	light->SetColor(0.0f, 0.0f, 1.0f);

	//Object = Instance();
	//Object->GetTransform()->Scale = { 0.01f, 0.01f, 0.01f };
	//filter = Object->AddComponent<MeshFilter>();
	//filter->SetModelName("Outside_Rock");
	
	//Object = Instance();
	//Object->GetTransform()->Scale = { 0.01f, 0.01f, 0.01f };
	//filter = Object->AddComponent<MeshFilter>();
	//filter->SetModelName("Outside_Other");
	//
	//Object = Instance();
	//Object->GetTransform()->Scale = { 0.01f, 0.01f, 0.01f };
	//filter = Object->AddComponent<MeshFilter>();
	//filter->SetModelName("Outside_bossOBJ");
	//
	//Object = Instance();
	//Object->GetTransform()->Scale = { 0.01f, 0.01f, 0.01f };
	//filter = Object->AddComponent<MeshFilter>();
	//filter->SetModelName("bossB");


	//for (int i = 0; i < 100; i++)
	//{
	//	Object = Instance();
	//	Object->GetTransform()->Position = { 0.0f, i * 100.0f, 0.0f };
	//	filter = Object->AddComponent<MeshFilter>();
	//	filter->SetModelName("DayRock3");
	//}

	//Object = Instance();
	//filter = Object->AddComponent<MeshFilter>();
	//filter->SetModelName("Inside_village");
	//
	//Object = Instance();
	//filter = Object->AddComponent<MeshFilter>();
	//Tr = Object->GetTransform();
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

	testobj = InstanceTerrain("Terrain");
	Terrain* mTerrain = testobj->GetComponent<Terrain>();
	mTerrain->SetLayerName("terrain_ground_A_BaseColor", "terrain_ground_A_Normal", "terrain_ground_A_ORM");
	mTerrain->SetLayerName("terrain_ground_B_BaseColor", "terrain_ground_B_Normal", "terrain_ground_B_ORM");
	mTerrain->SetMeshName("Terrain");
	mTerrain->SetTextureTiling(31.0f);
}

void TestScene::CreateParticle(float x, float y, float z)
{
	ParticleObj = Instance();
	ParticleObj->GetTransform()->Position = { -10.0f + x, 2.5f + y, -10.0f + z };
	testobj = InstanceParticle("Particle1");
	ParticleObj->ChoiceChild(testobj);
	ParticleSystem* particles = testobj->GetComponent<ParticleSystem>();
	particles->SetMeshName("Quad");
	particles->SetRenderType(PARTICLE_RENDER_OPTION::VERTICAL_BILLBOARD);
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

	testobj = InstanceParticle("Particle2");
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
	testobj = InstanceParticle("Particle1");
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

	testobj = InstanceParticle("Particle2");
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
	if (GetKey(VK_UP))
	{
		Object1->GetTransform()->Position.y += 0.1f;
		Object->GetTransform()->Position.y += 0.1f;
	}
	if (GetKey(VK_DOWN))
	{
		Object1->GetTransform()->Position.y -= 0.1f;
		Object->GetTransform()->Position.y -= 0.1f;
	}
	if (GetKey(VK_LEFT))
	{
		Object1->GetTransform()->Position.x -= 0.1f;
		Object->GetTransform()->Position.x -= 0.1f;
	}
	if (GetKey(VK_RIGHT))
	{
		Object1->GetTransform()->Position.x += 0.1f;
		Object->GetTransform()->Position.x += 0.1f;
	}

	if (GetKeyUp('1'))
	{
		LoadEnvironment("../Resources/Texture/Environment/Day.dds");
		SetEnvironment(true);
	}
	if (GetKeyUp('2'))
	{
		LoadEnvironment("../Resources/Texture/Environment/Night.dds");
		SetEnvironment(true);
	}
	if (GetKeyUp('3'))
	{
		LoadEnvironment("../Resources/Texture/Environment/skybox1.dds");
		SetEnvironment(true);
	}
	if (GetKeyUp('4'))
	{
		LoadEnvironment("../Resources/Texture/Environment/TestSky.dds");
		SetEnvironment(true);
	}
}
