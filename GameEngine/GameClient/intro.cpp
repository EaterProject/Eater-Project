#include "MainHeader.h"
#include "intro.h"
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
#include "MonsterA.h"
#include "ParticleSystem.h"
#include "Collider.h"

void intro::Awake()
{
	Load("../Assets/Scene/intro.Scene");
	
	Test01 = FindGameObjectTag("Test01");
	Test02 = FindGameObjectTag("Test02");
}

void intro::Update()
{
	Test02->GetTransform()->isTest = true;
	Test01->GetTransform()->Rotation = { 45,45,45 };
	Test02->GetTransform()->Rotation = {45,45,45};
}

void intro::End()
{

}


