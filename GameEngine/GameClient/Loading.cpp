#include "Loading.h"
#include "EaterEngineAPI.h"
#include "GameObject.h"
#include "Camera.h"
#include "Light.h"
#include "Transform.h"
#include "ParticleSystem.h"
#include "ClientNetworkManager.h"
#include "PortIPDefine.h"
#include "Camera.h"
#include "PlayerCamera.h"
#include "MeshFilter.h"
#include "EngineData.h"
#include "AnimationController.h"
#include "Material.h"
#include "LoadImage.h"

void Loading::Awake()
{
	Load("../Assets/Texture/UI");

	RenderOption* option = GetRenderOptionData();
	option->RenderingOption ^= RENDER_OPTION::RENDER_FOG;
	RenderSetting();

	GameObject* Loading_UI = Instance("Loading_UI");
	Loading_UI->AddComponent<LoadingImage>();
}

void Loading::Update()
{
	if (ThreadRun == false)
	{
		ChoiceScene("inGame");
		//ChoiceScene("TestScene");
	}
}

void Loading::End()
{
	 
}

void Loading::ThreadFunction()
{
	Load("../Assets/Texture/ModelTexture");
	Load("../Assets/Texture/Environment");
	Load("../Assets/Texture/Terrain");
	Load("../Assets/Texture/Material");
	Load("../Assets/Texture/Particle");

	Load("../Assets/Model/Animation");
	Load("../Assets/Model/MeshBuffer");
	Load("../Assets/Model/ModelData");
	Load("../Assets/Sound");

	LoadTerrainMesh("../Assets/Model/TerrainModel/Terrain.fbx", "../Assets/Texture/Terrain/Terrain_RGB_1.png", "../Assets/Texture/Terrain/Terrain_RGB_2.png", SCALING);
}
