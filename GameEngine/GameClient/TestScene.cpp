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
#include "RectTransform.h"
#include "Image.h"
#include "Button.h"
#include "RectTransform.h"
#include "Slider.h"
#include "ParticleController.h"

#include "EngineData.h"

#include "MiniMapSystem.h"

#include "./Profiler/Profiler.h"

void TestScene::Awake()
{
	LoadTerrainMesh("../Assets/Model/TerrainModel/Terrain.fbx", "../Assets/Texture/Terrain/Terrain_RGB_1.png", "../Assets/Texture/Terrain/Terrain_RGB_2.png", SCALING);

	BakeSkyLightMap("SkyLight_0", false);
	BakeSkyLightMap("SkyLight_1", false);

	//AddOccluder("Dome_Occluder_0");


	CreateMap();
	CreateDissolve();

	BakeSkyLightMap("SkyLight_0", false);
	BakeSkyLightMap("SkyLight_1", false);
	SetSkyLight("SkyLight_0", 0);
	SetSkyLight("SkyLight_1", 1);

	SetSkyCube("SkyCube");

	//Load("../Assets/Scene/TestScene.Scene");

	RenderOption* option = GetRenderOptionData();
	//option->RenderingOption ^= RENDER_OPTION::RENDER_FOG;
	option->RenderingOption ^= RENDER_OPTION::RENDER_SHADOW;
	RenderSetting();
}

void TestScene::Update()
{
	ChangeCubeMap();
}

void TestScene::End()
{

}

void TestScene::CreateMap()
{
	Object = Instance_Terrain("Terrain");
	Terrain* mTerrain = Object->GetComponent<Terrain>();
	mTerrain->SetLayerName("terrain_ground_A_BaseColor", "terrain_ground_A_Normal", "terrain_ground_A_ORM");
	mTerrain->SetLayerName("terrain_ground_B_BaseColor", "terrain_ground_B_Normal", "terrain_ground_B_ORM");
	mTerrain->SetLayerName("terrain_ground_C_BaseColor", "terrain_ground_C_Normal", "terrain_ground_C_ORM");
	mTerrain->SetLayerName("terrain_ground_D_BaseColor", "terrain_ground_D_Normal", "terrain_ground_D_ORM");
	mTerrain->SetMeshName("Terrain");
	mTerrain->SetTextureTiling(31.0f);
}

void TestScene::InButton()
{
	button->SetTexture("in_button");
}

void TestScene::OutButton()
{
	button->SetTexture("out_button");
}

void TestScene::DownButton()
{
	button->SetTexture("down_button");
}

void TestScene::UpButton()
{
	button->SetTexture("up_button");
}

void TestScene::PressDownButton()
{
	button->SetTexture("pressdown_button");
}

void TestScene::CreateMiniMap()
{
	/// 미니맵 설정
	m_MiniMap = MiniMapSystem::Get();
	m_MiniMap->CreateMiniMap("ingame_minimap", PIVOT_RIGHT_TOP, ROTATE_90, Vector2(186.0f), Vector2(-25.0f));


	m_Boss = Instance();
	Filter = m_Boss->AddComponent<MeshFilter>();
	Filter->SetModelName("BossB+");
	Filter->SetAnimationName("BossB+");
	m_Boss->GetTransform()->SetPosition(0, 7, -15);
	m_Boss->GetTransform()->SetScale(1.5f);
	AC = m_Boss->AddComponent<AnimationController>();
	ACList.push_back(AC);
	TRList.push_back(m_Boss->GetTransform());

	/// 미니맵 아이콘 추가
	m_MiniMap->CreateIcon("Minimap_Boss", m_Boss, false);


	m_MonsterA = Instance();
	Filter = m_MonsterA->AddComponent<MeshFilter>();
	Filter->SetModelName("MonsterA+");
	Filter->SetAnimationName("MonsterA+");
	m_MonsterA->GetTransform()->SetPosition(5, 7, -15);
	m_MonsterA->GetTransform()->SetScale(1.2f, 1.2f, 1.2f);
	AC = m_MonsterA->AddComponent<AnimationController>();
	ACList.push_back(AC);
	TRList.push_back(m_MonsterA->GetTransform());

	/// 미니맵 아이콘 추가
	m_MiniMap->CreateIcon("Minimap_Player", m_MonsterA, true);


	m_MonsterB = Instance();
	Filter = m_MonsterB->AddComponent<MeshFilter>();
	Filter->SetModelName("MonsterB+");
	Filter->SetAnimationName("MonsterB+");
	m_MonsterB->GetTransform()->SetPosition(-5, 7, -15);
	m_MonsterB->GetTransform()->SetScale(3.0f, 3.0f, 3.0f);
	AC = m_MonsterB->AddComponent<AnimationController>();
	ACList.push_back(AC);
	TRList.push_back(m_MonsterB->GetTransform());

	/// 미니맵 아이콘 추가
	m_MiniMap->CreateIcon("Minimap_Player", m_MonsterB, true);
}

void TestScene::CreateButton()
{
	Object = Instance_Button();
	Rect_Tr = Object->GetComponent<RectTransform>();
	button = Object->GetComponent<Button>();
	button->SetBoundaryOffset(-50.0f, 50.0f, 50.0f, -50.0f);
	button->SetTexture("out_button");
	button->SetPivot(PIVOT_TYPE::PIVOT_MIDDLE_CENTER);
	button->SetPosition(50, -50);
	button->PushTextureList("down_button");
	button->PushTextureList("in_button");
	button->PushTextureList("out_button");
	button->PushTextureList("pressdown_button");
	button->PushTextureList("up_button");
	button->PushEvent(Eater::Bind(&TestScene::InButton, this), Button::IN_BUTTON);
	button->PushEvent(Eater::Bind(&TestScene::OutButton, this), Button::OUT_BUTTON);
	button->PushEvent(Eater::Bind(&TestScene::DownButton, this), Button::DOWN_BUTTON);
	button->PushEvent(Eater::Bind(&TestScene::UpButton, this), Button::UP_BUTTON);
	button->PushEvent(Eater::Bind(&TestScene::PressDownButton, this), Button::PRESS_DOWN_BUTTON);
}

void TestScene::CreateParicleController()
{
	GameObject* particle_1 = Instance_Particle("Particle1", "BossPush_circle");
	GameObject* particle_2 = Instance_Particle("Particle2", "BossPush_magical");
	GameObject* particle_3 = Instance_Particle("Particle3", "BossProjectile_aura");
	GameObject* particle_4 = Instance_Particle("Particle4", "BossProjectile_circle");
	GameObject* particle_5 = Instance_Particle("Particle5", "BossProjectile_dot");

	Object = Instance();
	m_Controller = Object->GetTransform();
	m_ParticleController = Object->AddComponent<ParticleController>();
	m_ParticleController->PushParticle("Particle_1", particle_1->GetComponent<ParticleSystem>(), 0.0f);
	m_ParticleController->PushParticle("Particle_2", particle_2->GetComponent<ParticleSystem>(), 0.0f);
	m_ParticleController->PushParticle("Particle_3", particle_3->GetComponent<ParticleSystem>(), 1.0f);
	m_ParticleController->PushParticle("Particle_4", particle_4->GetComponent<ParticleSystem>(), 1.0f);
	m_ParticleController->PushParticle("Particle_5", particle_5->GetComponent<ParticleSystem>(), 1.0f);
	m_ParticleController->PushParticle("Particle_6", particle_1->GetComponent<ParticleSystem>(), 1.5f);
	m_ParticleController->PushParticle("Particle_7", particle_2->GetComponent<ParticleSystem>(), 1.5f);
}

void TestScene::CreateDissolve()
{
	std::string texName("Dissolve_");

	for (int i = 1; i <= TextureCount; i++)
	{
		m_NoiseTextureList.push_back(GetTexture(texName + std::to_string(i)));
	}

	m_Boss = Instance();
	m_BossFilter = m_Boss->AddComponent<MeshFilter>();
	m_BossFilter->SetModelName("BossB+");
	m_BossFilter->SetAnimationName("BossB+");
	m_Boss->GetTransform()->SetPosition(0, 7, -15);
	m_Boss->GetTransform()->SetScale(1.5f);
	AC = m_Boss->AddComponent<AnimationController>();
	ACList.push_back(AC);
	TRList.push_back(m_Boss->GetTransform());

	/// 알파 관련 Material Property Block을 통해 생성해야함..
	//m_BossFilter->SetMaterialPropertyBlock(true, true);
	//m_BossBlock = Filter->GetMaterialPropertyBlock();
	
	/// 사용시 해당 함수 인자 모두 채워줘야함..
	/// 함수 설정시 부터 플레이 시간까지 해당 데이터로 재생..
	/// 수치 값은 기획자들이 적어줄꺼임
	//m_BossBlock->SetDissolve(DISSOLVE_OPTION::DISSOLVE_FADEOUT, m_NoiseTextureList[TextureIndex], Vector3(1.0f, 0.0f, 0.0f), 10.0f, 2.0f, 0.01f, 25.0f, 100.0f);

	m_MonsterA = Instance();
	m_MonsterAFilter = m_MonsterA->AddComponent<MeshFilter>();
	m_MonsterAFilter->SetModelName("MonsterA+");
	m_MonsterAFilter->SetAnimationName("MonsterA+");
	m_MonsterA->GetTransform()->SetPosition(5, 7, -15);
	m_MonsterA->GetTransform()->SetScale(1.2f, 1.2f, 1.2f);
	AC = m_MonsterA->AddComponent<AnimationController>();
	ACList.push_back(AC);
	TRList.push_back(m_MonsterA->GetTransform());

	/// 알파 관련 Material Property Block을 통해 생성해야함..
	//m_MonsterAFilter->SetMaterialPropertyBlock(true, true);
	//m_MonsterABlock = Filter->GetMaterialPropertyBlock();

	/// 사용시 해당 함수 인자 모두 채워줘야함..
	/// 함수 설정시 부터 플레이 시간까지 해당 데이터로 재생..
	//m_MonsterABlock->SetDissolve(DISSOLVE_OPTION::DISSOLVE_FADEIN, m_NoiseTextureList[TextureIndex], Vector3(1.0f, 0.0f, 0.0f), 10.0f, 2.0f, 0.01f, 25.0f, 100.0f);

	m_MonsterB = Instance();
	m_MonsterBFilter = m_MonsterB->AddComponent<MeshFilter>();
	m_MonsterBFilter->SetModelName("MonsterB+");
	m_MonsterBFilter->SetAnimationName("MonsterB+");
	m_MonsterB->GetTransform()->SetPosition(-5, 7, -15);
	m_MonsterB->GetTransform()->SetScale(3.0f, 3.0f, 3.0f);
	AC = m_MonsterB->AddComponent<AnimationController>();
	ACList.push_back(AC);
	TRList.push_back(m_MonsterB->GetTransform());

	/// 알파 관련 Material Property Block을 통해 생성해야함..
	//m_MonsterBFilter->SetMaterialPropertyBlock(true, true);
	//m_MonsterBBlock = Filter->GetMaterialPropertyBlock();

	/// 사용시 해당 함수 인자 모두 채워줘야함..
	/// 함수 설정시 부터 플레이 시간까지 해당 데이터로 재생..
	//m_MonsterBBlock->SetDissolve(DISSOLVE_OPTION::DISSOLVE_FADEIN, m_NoiseTextureList[TextureIndex], Vector3(1.0f, 0.0f, 0.0f), 10.0f, 2.0f, 0.01f, 25.0f, 100.0f);
}

void TestScene::SetMaterialBlock(GameObject* object, std::vector<MaterialPropertyBlock*>& blocklist, bool alpha)
{
	int Count = object->GetChildMeshCount();
	if (Count == 0)
	{
		MeshFilter* mMeshFilter = object->GetComponent<MeshFilter>();

		mMeshFilter->SetMaterialPropertyBlock(true, alpha);
		blocklist.push_back(mMeshFilter->GetMaterialPropertyBlock());
	}
	else
	{
		for (int i = 0; i < Count; i++)
		{
			//게임 오브젝트와 매쉬필터 가져오기
			GameObject* Obj = object->GetChildMesh(i);
			MeshFilter* mMeshFilter = Obj->GetComponent<MeshFilter>();

			//메터리얼 블록 추가
			mMeshFilter->SetMaterialPropertyBlock(true, alpha);
			blocklist.push_back(mMeshFilter->GetMaterialPropertyBlock());
		}
	}
}

void TestScene::ChangeCubeMap()
{
	float dTime = GetDeltaTime();

	if (GetKey('1'))
	{
		for (int i = 0; i < ACList.size(); i++)
		{
			ACList[i]->Choice("idle");
			ACList[i]->Play();
		}
	}
	//if (GetKeyUp('2'))
	//{
	//	m_ParticleController->Play();
	//}
	//if (GetKeyUp('3'))
	//{
	//	m_ParticleController->Pause();
	//}
	//if (GetKeyUp('4'))
	//{
	//	m_ParticleController->Stop();
	//}

	if (GetKeyUp('2'))
	{
		SetMaterialBlock(m_Boss, m_BossBlock, true);
		SetMaterialBlock(m_MonsterA, m_MonsterABlock, true);
		SetMaterialBlock(m_MonsterB, m_MonsterBBlock, true);
	}
	if (GetKeyUp('3'))
	{
		for (auto& block : m_BossBlock)
		{
			block->SetDissolve(DISSOLVE_OPTION::DISSOLVE_FADEOUT, m_NoiseTextureList[TextureIndex], Vector3(1.0f, 0.0f, 0.0f), 10.0f, 2.0f, 0.01f, 25.0f, 100.0f);
		}
	}
	if (GetKeyUp('4'))
	{
		for (auto& block : m_MonsterABlock)
		{
			block->SetDissolve(DISSOLVE_OPTION::DISSOLVE_FADEIN, m_NoiseTextureList[TextureIndex], Vector3(1.0f, 0.0f, 0.0f), 10.0f, 2.0f, 0.01f, 25.0f, 100.0f);
		}
	}
	if (GetKeyUp('5'))
	{
		for (auto& block : m_MonsterBBlock)
		{
			block->SetDissolve(DISSOLVE_OPTION::DISSOLVE_FADEIN, m_NoiseTextureList[TextureIndex], Vector3(1.0f, 0.0f, 0.0f), 10.0f, 2.0f, 0.01f, 25.0f, 100.0f);
		}
	}
	if (GetKeyUp('6'))
	{
		TextureIndex++;

		if (TextureIndex > TextureCount - 1)
		{
			TextureIndex = 0;
		}
	}


	//if (GetKey(VK_LEFT))
	//{
	//	for (int i = 0; i < TRList.size(); i++)
	//	{
	//		TRList[i]->AddPosition_X(-dTime * 50.0f);
	//	}
	//}
	//if (GetKey(VK_RIGHT))
	//{
	//	for (int i = 0; i < TRList.size(); i++)
	//	{
	//		TRList[i]->AddPosition_X(dTime * 50.0f);
	//	}
	//}
	//if (GetKey(VK_UP))
	//{
	//	for (int i = 0; i < TRList.size(); i++)
	//	{
	//		TRList[i]->AddPosition_Z(dTime * 50.0f);
	//	}
	//}
	//if (GetKey(VK_DOWN))
	//{
	//	for (int i = 0; i < TRList.size(); i++)
	//	{
	//		TRList[i]->AddPosition_Z(-dTime * 50.0f);
	//	}
	//}
	//if (GetKey('Q'))
	//{
	//	for (int i = 0; i < TRList.size(); i++)
	//	{
	//		TRList[i]->AddRotate_Y(-dTime * 50.0f);
	//	}
	//}
	if (GetKey('R'))
	{
		for (int i = 0; i < TRList.size(); i++)
		{
			TRList[i]->AddRotate_Y(dTime * 50.0f);
		}
	}
}
