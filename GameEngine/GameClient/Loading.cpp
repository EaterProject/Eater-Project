#include "Loading.h"
#include "MainHeader.h"
#include "GameObject.h"
#include "Camera.h"
#include "Light.h"
#include "Transform.h"
#include "ParticleSystem.h"
#include "KeyInput.h"
#include "ClientNetworkManager.h"
#include "PortIPDefine.h"
#include "Camera.h"
#include "PlayerCamera.h"
#include "MeshFilter.h"
#include "EngineData.h"
#include "AnimationController.h"
#include "Material.h"

void Loading::Awake()
{
	//Load("../Resources/Texture/Particle");
	Load("../Assets/Texture/Particle/particle_hotCloud.png");
	//
	ParticleObj = Instance();
	obj = InstanceParticle("Particle12");
	ParticleObj->ChoiceChild(obj);
	particles = obj->GetComponent<ParticleSystem>();
	particles->SetMeshName("Quad");
	particles->SetRenderType(PARTICLE_RENDER_OPTION::VERTICAL_BILLBOARD);
	particles->SetDiffuseName("particle_hotCloud");
	particles->SetStartLifeTime(1.5f, 1.8f);
	particles->SetStartScale(4.0f, 7.0f);
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

	
	LoadEnvironment("../Resources/Texture/Environment/Night.dds");
	LoadTerrainMesh("../Assets/Model/TerrainModel/Terrain.fbx", "../Assets/Texture/Terrain/Terrain_RGB.png", SCALING);
}


void Loading::Update()
{
	if (ThreadRun == false)
	{
		ChoiceScene("intro");
	}
}

void Loading::End()
{


}

void Loading::ThreadFunction()
{
	Load("../Assets/Texture/ModelTexture");
	Load("../Assets/Texture/Terrain");
	Load("../Assets/Texture/Material");
	Load("../Assets/Texture/Particle");

	Load("../Assets/Model/Animation");
	Load("../Assets/Model/MeshBuffer");
	Load("../Assets/Model/ModelData");
	
	
	LoadEnvironment("../Assets/Texture/Environment/Night.dds");
	SetEnvironment(true);
}


