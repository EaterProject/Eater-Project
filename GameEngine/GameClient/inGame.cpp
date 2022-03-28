#include "MainHeader.h"
#include "inGame.h"
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
#include "ParticleSystem.h"

void InGame::Awake()
{
	//LoadTexturePath("../Resources/Texture/");
	//LoadMeshPath("../Resources/Mesh/");
	//LoadMeshPath("../Resources/Mesh/Terrain/");

	//LoadMeshFile("../Resources/Mesh/BaseMesh");

	//LoadMeshFile("BaseMesh/Test", SCALING);
	//LoadTextureFile("../Resources/Texture/Basic");

	//LoadMeshFile("../Resources/Mesh/TestMesh");

	LoadTerrainMesh("../Resources/Mesh/Terrain/Terrain.fbx", "../Resources/Texture/Terrain/Terrain_RGB.png", SCALING);
	//LoadMesh("TerrainDecimate", SCALING);
	//
	//LoadTexture("ground01_Albedo.png");
	//LoadTexture("ground02_Albedo.png");
	//LoadTexture("ground01_Normal.png");
	//LoadTexture("ground02_Normal.png");

	//LoadEnvironment("../Resources/Texture/Environment/Night.dds");
	SetEnvironment(true);

	//LoadTexture("Particle/particle_hotCloud.png");
	//ParticleObj = Instance();
	//ParticleObj->GetTransform()->Position.x -= 10.0f;
	//testobj = InstanceParticle("Particle12");
	//ParticleObj->ChoiceChild(testobj);
	//particles = testobj->GetComponent<ParticleSystem>();
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
	//particles->SetPlayTime(1, true);
	//particles->Play();

	///ÅÍ·¹ÀÎ
	testobj = InstanceTerrain("Terrain");
	Terrain* mTerrain = testobj->GetComponent<Terrain>();
	mTerrain->SetLayerName("terrain_ground_A_BaseColor", "terrain_ground_A_Normal", "terrain_ground_A_ORM");
	mTerrain->SetLayerName("terrain_ground_B_BaseColor", "terrain_ground_B_Normal", "terrain_ground_B_ORM");
	mTerrain->SetMeshName("Terrain");
	//mTerrain->SetColliderName("TerrainDecimate");
	mTerrain->SetTextureTiling(31.0f);

	//PointLightObj = InstanceLight("PointLight", LIGHT_TYPE::POINT_LIGHT);
	//Light* pointLight = PointLightObj->GetComponent<Light>();
	//pointLight->SetColor(1.0f, 1.0f, 0.0f, 1.0f);
	//pointLight->SetRange(10.0f);
	//PointLightObj->transform->Position.x = -20.0f;
	//PointLightObj->transform->Position.y = 10.0f;
	//SpotLightObj = InstanceLight("SpotLight", LIGHT_TYPE::SPOT_LIGHT);
	//Light* spotLight = SpotLightObj->GetComponent<Light>();
	//spotLight->SetColor(0.0f, 1.0f, 1.0f, 1.0f);
	//spotLight->SetRange(20.0f);
	//spotLight->SetSpot(10.0f);
	//SpotLightObj->transform->Position.x = 20.0f;
	//SpotLightObj->transform->Position.y = 10.0f;

	//testobj = Instance("Player");
	//testobj->AddComponent<AnimationController>();
	//testobj->AddComponent<MeshFilter>();
	//testobj->AddComponent<TestPlayer>();

	MeshFilter* MF;

	//for (int i = 0; i < 10; i++)
	//{
	//	std::string num = std::to_string(9 - i);
	//
	//	testobj = Instance("box");
	//	testobj->AddComponent<MeshFilter>()->SetModelName("TestSphere");
	//	MF = testobj->GetComponent<MeshFilter>();
	//	MF->SetDiffuseTextureName("Albedo_" + num);
	//	MF->SetNormalTextureName("Normal_" + num);
	//	MF->SetORMTextureName("ORM_" + num);
	//	testobj->GetTransform()->Position = { -150.0f + 30.0f * i, 150.0f, 0.0f };
	//	testobj->GetTransform()->Rotation = { 90.0f, 0.0f, 0.0f };
	//	testobj->GetTransform()->Scale = { 5.0f, 5.0f, 5.0f };
	//
	//	SphereList.push_back(testobj);
	//}


	//testobj = Instance("box");
	//testobj->AddComponent<MeshFilter>()->SetModelName("TestSphere");
	//MF = testobj->GetComponent<MeshFilter>();
	//MF->SetDiffuseTextureName("Albedo_0");
	//MF->SetNormalTextureName("Normal_0");
	//MF->SetORMTextureName("ORM_0");
	//testobj->GetTransform()->Position = { 0.0f, 5.0f, 0.0f };
	//testobj->GetTransform()->Rotation = { 90.0f, 0.0f, 0.0f };
	//testobj->GetTransform()->Scale = { 2.5f, 2.5f, 2.5f };

	testobj = Instance("Scar");
	testobj->AddComponent<MeshFilter>()->SetModelName("Scar");
	testobj->GetTransform()->Position = { -100.0f, 75.0f, 0.0f };
	testobj->GetTransform()->Scale = { 0.01f, 0.01f, 0.01f };
	testobj->GetTransform()->Rotation = { -90.0f, -90.0f, -90.0f };

	ObjList.push_back(testobj);

	testobj = Instance("Gun");
	testobj->AddComponent<MeshFilter>()->SetModelName("Gun");
	testobj->GetTransform()->Position = { -60.0f, 75.0f, 0.0f };
	testobj->GetTransform()->Scale = { 1.0f, 1.0f, 1.0f };
	testobj->GetTransform()->Rotation = { -90.0f, -90.0f, -90.0f };

	ObjList.push_back(testobj);

	testobj = Instance("Sword1");
	testobj->AddComponent<MeshFilter>()->SetModelName("Sword1");
	testobj->GetTransform()->Position = { -40.0f, 75.0f, 0.0f };
	testobj->GetTransform()->Scale = { 0.1f, 0.1f, 0.1f };
	testobj->GetTransform()->Rotation = { -90.0f, 0.0f, 0.0f };

	ObjList.push_back(testobj);

	testobj = Instance("LampA");
	testobj->AddComponent<MeshFilter>()->SetModelName("LampA");
	testobj->GetTransform()->Position = { 0.0f, 85.0f, 0.0f };
	testobj->GetTransform()->Scale = { 0.1f, 0.1f, 0.1f };
	testobj->GetTransform()->Rotation = { 0.0f, 90.0f, 0.0f };

	ObjList.push_back(testobj);

	testobj = Instance("SciFiBox");
	testobj->AddComponent<MeshFilter>()->SetModelName("SciFiBox");
	testobj->GetTransform()->Position = { 50.0f, 75.0f, 0.0f };
	testobj->GetTransform()->Scale = { 0.5f, 0.5f, 0.5f };
	testobj->GetTransform()->Rotation = { 0.0f, 0.0f, 0.0f };

	ObjList.push_back(testobj);

	testobj = Instance("Sword2");
	testobj->AddComponent<MeshFilter>()->SetModelName("Sword2");
	testobj->GetTransform()->Position = { 90.0f, 75.0f, 0.0f };
	testobj->GetTransform()->Scale = { 1.0f, 1.0f, 1.0f };
	testobj->GetTransform()->Rotation = { 0.0f, 0.0f, 0.0f };

	ObjList.push_back(testobj);

	testobj = Instance("Chair");
	testobj->AddComponent<MeshFilter>()->SetModelName("Chair");
	testobj->GetTransform()->Position = { 120.0f, 50.0f, 0.0f };
	testobj->GetTransform()->Scale = { 0.01f, 0.01f, 0.01f };
	testobj->GetTransform()->Rotation = { -90.0f, 0.0f, 90.0f };

	//ObjList.push_back(testobj);

	//testobj = Instance("monster_A");
	//testobj->GetTransform()->Rotation.x -= 90.0f;
	//testobj->GetTransform()->Scale = { 1.0f, 1.0f, 1.0f };
	//testobj->AddComponent<MeshFilter>()->SetModelName("monster");
	//AC =testobj->AddComponent<AnimationController>();
	//testobj->GetComponent<MeshFilter>()->SetAnimationName("monster");

	//testobj = Instance("Dome");
	//testobj->AddComponent<MeshFilter>()->SetModelName("Dome");
	//testobj->GetComponent<MeshFilter>()->SetDiffuseTextureName("ColorMap_1");
	//testobj->GetTransform()->Scale = { 1, 1, 1 };

	//testobj = Instance("Inside");
	//testobj->AddComponent<MeshFilter>()->SetModelName("Inside");
	//testobj->GetComponent<MeshFilter>()->SetDiffuseTextureName("ColorMap_1");
	//testobj->GetTransform()->Scale = { 0.01f, 0.01f, 0.01f };


	//ParticleObj = Instance();
	//ParticleObj->GetTransform()->Position = { -10.0f, 2.5f, -10.0f };
	//testobj = InstanceParticle("Particle1");
	//ParticleObj->ChoiceChild(testobj);
	//ParticleSystem* particles = testobj->GetComponent<ParticleSystem>();
	//particles->SetMeshName("Quad");
	//particles->SetRenderType(PARTICLE_RENDER_OPTION::VERTICAL_BILLBOARD);
	//particles->SetDiffuseName("particle_hotCloud");
	//particles->SetStartLifeTime(1.5f, 1.8f);
	//particles->SetStartScale(4.0f, 7.0f);
	//particles->SetStartRotation(-360, 360);
	//particles->SetStartColor(Vector4(43, 74, 255, 120), Vector4(23, 111, 255, 255));
	//particles->SetMaxParticles(30);
	//particles->SetRateOverTime(15.0f);
	//particles->SetShapeRadius(0.1875f);
	//particles->SetStartForce(Vector3(0, 5, 0));
	//particles->SetLifeTimeRotation(-15.0f, 15.0f);
	//particles->SetLifeTimeColor(Vector4(255, 255, 255, 50), Vector4(255, 255, 255, 255), PARTICLE_LIFETIME_OPTION::UPDOWN);
	//particles->SetTextureTiling(8, 8);
	//particles->SetPlayTime(10.0f, true);
	//particles->Play();
	//
	//testobj = InstanceParticle("Particle2");
	//ParticleObj->ChoiceChild(testobj);
	//particles = testobj->GetComponent<ParticleSystem>();
	//particles->SetMeshName("Quad");
	//particles->SetDiffuseName("particle_blast4");
	//particles->SetStartLifeTime(1.0f, 2.0f);
	//particles->SetStartScale(7.0f, 9.125f);
	//particles->SetStartRotation(-360, 360);
	//particles->SetStartColor(Vector4(25, 25, 25, 50));
	//particles->SetMaxParticles(20);
	//particles->SetRateOverTime(10.0f);
	//particles->SetShapeRadius(0.1f);
	//particles->SetStartForce(Vector3(0, 7.5f, 0));
	//particles->SetLifeTimeScale(0.25f, 1.0f, PARTICLE_LIFETIME_OPTION::UP);
	//particles->SetLifeTimeColor(Vector4(255, 255, 255, 0), Vector4(255, 255, 255, 255), PARTICLE_LIFETIME_OPTION::UPDOWN);
	//particles->SetLifeTimeRotation(-15.0f, 15.0f);
	//particles->SetTextureTiling(8, 8);
	//particles->SetPlayTime(10.0f, true);
	//particles->Play(true);
	//
	//ParticleObj = Instance();
	//ParticleObj->GetTransform()->Position = { 10.0f, 2.5f, -10.0f };
	//testobj = InstanceParticle("Particle1");
	//ParticleObj->ChoiceChild(testobj);
	//particles = testobj->GetComponent<ParticleSystem>();
	//particles->SetMeshName("Quad");
	//particles->SetRenderType(PARTICLE_RENDER_OPTION::VERTICAL_BILLBOARD);
	//particles->SetDiffuseName("particle_hotCloud");
	//particles->SetStartLifeTime(1.5f, 1.8f);
	//particles->SetStartScale(4.0f, 7.0f);
	//particles->SetStartRotation(-360, 360);
	//particles->SetStartColor(Vector4(255, 74, 43, 120), Vector4(255, 111, 23, 255));
	//particles->SetMaxParticles(30);
	//particles->SetRateOverTime(15.0f);
	//particles->SetShapeRadius(0.1875f);
	//particles->SetStartForce(Vector3(0, 5, 0));
	//particles->SetLifeTimeRotation(-15.0f, 15.0f);
	//particles->SetLifeTimeColor(Vector4(255, 255, 255, 50), Vector4(255, 255, 255, 255), PARTICLE_LIFETIME_OPTION::UPDOWN);
	//particles->SetTextureTiling(8, 8);
	//particles->SetPlayTime(10.0f, true);
	//particles->Play();
	//
	//testobj = InstanceParticle("Particle2");
	//ParticleObj->ChoiceChild(testobj);
	//particles = testobj->GetComponent<ParticleSystem>();
	//particles->SetMeshName("Quad");
	//particles->SetDiffuseName("particle_blast4");
	//particles->SetStartLifeTime(1.0f, 2.0f);
	//particles->SetStartScale(7.0f, 9.125f);
	//particles->SetStartRotation(-360, 360);
	//particles->SetStartColor(Vector4(25, 25, 25, 50));
	//particles->SetMaxParticles(20);
	//particles->SetRateOverTime(10.0f);
	//particles->SetShapeRadius(0.1f);
	//particles->SetStartForce(Vector3(0, 7.5f, 0));
	//particles->SetLifeTimeScale(0.25f, 1.0f, PARTICLE_LIFETIME_OPTION::UP);
	//particles->SetLifeTimeColor(Vector4(255, 255, 255, 0), Vector4(255, 255, 255, 255), PARTICLE_LIFETIME_OPTION::UPDOWN);
	//particles->SetLifeTimeRotation(-15.0f, 15.0f);
	//particles->SetTextureTiling(8, 8);
	//particles->SetPlayTime(10.0f, true);
	//particles->Play(true);
}

void InGame::Start()
{
}

void InGame::Update()
{
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

void InGame::End()
{
}
