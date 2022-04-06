#include "MainHeader.h"
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

#define RELEASE_PROFILE
#include "./Profiler/Profiler.h"

void TestScene::Awake()
{
	LoadEnvironment("../Assets/Texture/Environment/Day.dds");
	LoadTerrainMesh("../Assets/Model/TerrainModel/Terrain.fbx", "../Assets/Texture/Terrain/Terrain_RGB_1.png", "../Assets/Texture/Terrain/Terrain_RGB_2.png", SCALING);

	PROFILE_TIMER_START(PROFILE_OUTPUT::CONSOLE, 1, "Load Folder");
	Load("../Assets/Texture/Terrain");
	//Load("../Assets/Texture/Particle");
	Load("../Assets/Texture/ModelTexture");
	Load("../Assets/Texture/Material");
	Load("../Assets/Model/MeshBuffer");
	Load("../Assets/Model/ModelData");
	Load("../Assets/Model/Animation");
	PROFILE_TIMER_END("Load Folder"); 

	AddOccluder("Dome_Occluder_0");

	CreateMap();

	//CreateParticle(0,0,0);

	SetEnvironment(true);
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

	Object = InstanceLight("Light", LIGHT_TYPE::POINT_LIGHT);
	Object->GetTransform()->Position.x += 10.0f;
	Object->GetTransform()->Position.y += 10.0f;

	Object = InstanceLight("Light", LIGHT_TYPE::SPOT_LIGHT);
	Object->GetTransform()->Position.x -= 10.0f;
	Object->GetTransform()->Position.y += 10.0f;

	//Object = Instance();
	//filter = Object->AddComponent<MeshFilter>();
	//filter->SetModelName("bossb+");
	//filter->SetAnimationName("bossb+");
	//AnimationController* AC = Object->AddComponent<AnimationController>();
	//AC->Choice("idle");
	//
	//Object = Instance();
	//filter = Object->AddComponent<MeshFilter>();
	//filter->SetModelName("MonsterA+");
	//filter->SetAnimationName("MonsterA+");
	//AC = Object->AddComponent<AnimationController>();
	//AC->Choice("Move");

	//Object = Instance();
	//filter = Object->AddComponent<MeshFilter>();
	//filter->SetModelName("Dome_Occluder");

	//Object = Instance();
	//filter = Object->AddComponent<MeshFilter>();
	//filter->SetModelName("Dome2");
	//
	//Object = Instance();
	//filter = Object->AddComponent<MeshFilter>();
	//filter->SetModelName("Inside_village");
	//

	Object = Instance();
	filter = Object->AddComponent<MeshFilter>();
	Tr = Object->GetTransform();
	filter->SetModelName("Outside_Rock");
	
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
	mTerrain->SetLayerName("terrain_ground_C_BaseColor", "terrain_ground_C_Normal", "terrain_ground_C_ORM");
	mTerrain->SetLayerName("terrain_ground_D_BaseColor", "terrain_ground_D_Normal", "terrain_ground_D_ORM");
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
	//if (GetKey(VK_UP))
	//{
	//	Object->GetTransform()->Rotation.z += 1;
	//	Object1->GetTransform()->Position.y += 0.1f;
	//}
	//if (GetKey(VK_DOWN))
	//{
	//	Object->GetTransform()->Rotation.z -= 1;
	//	Object1->GetTransform()->Position.y -= 0.1f;
	//}
	//if (GetKey(VK_LEFT))
	//{
	//	Object->GetTransform()->Rotation.x -= 1;
	//	Object1->GetTransform()->Position.x -= 0.1f;
	//}
	//if (GetKey(VK_RIGHT))
	//{
	//	Object->GetTransform()->Rotation.x += 1;
	//	Object1->GetTransform()->Position.x += 0.1f;
	//}

	if (GetKeyUp('1'))
	{
		LoadEnvironment("../Assets/Texture/Environment/Day.dds");
		SetEnvironment(true);
	}
	if (GetKeyUp('2'))
	{
		LoadEnvironment("../Assets/Texture/Environment/Night.dds");
		SetEnvironment(true);
	}
	if (GetKeyUp('3'))
	{
		LoadEnvironment("../Assets/Texture/Environment/skybox1.dds");
		SetEnvironment(true);
	}
	if (GetKeyUp('4'))
	{
		LoadEnvironment("../Assets/Texture/Environment/TestSky.dds");
		SetEnvironment(true);
	}
}
