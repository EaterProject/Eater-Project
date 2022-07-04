#include "PlayerPortal.h"



#include "MeshFilter.h"
#include "Transform.h"
#include "AnimationController.h"
#include "GameObject.h"

PlayerCapsule::PlayerCapsule()
{


}

PlayerCapsule::~PlayerCapsule()
{



}

void PlayerCapsule::Awake()
{
	mAnime		= gameobject->GetComponent<AnimationController>();
	mMeshFilter = gameobject->GetComponent<MeshFilter>();
	mTransform	= gameobject->GetTransform();
}

void PlayerCapsule::SetUp()
{


}


void PlayerCapsule::Start()
{



}
