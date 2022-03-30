#include "Rigidbody.h"
#include "Transform.h"
#include "GameObject.h"
#include "PhysData.h"
#include "PhysX_API.h"
#include "PhysData.h"
#include "EngineData.h"
#include "Collider.h"

Rigidbody::Rigidbody()
{
	//컨퍼넌트 실행 순서를 변경
	Component::TransformUpdate_Order	= Component::FUNCTION_ORDER_FIRST;
	Component::SetUp_Order				= Component::FUNCTION_ORDER_LAST;
}

Rigidbody::~Rigidbody()
{
	
}

void Rigidbody::Awake()
{
	Collider* mCollider = gameobject->GetComponent<Collider>();
	mTransform			= gameobject->GetTransform();
	if (mCollider != nullptr)
	{
		RigidbodyData = mCollider->mPhysData;
	}
}

void Rigidbody::Start()
{
	//if (ColliderName != "")
	//{
	//	//이것은 트라이엥글 매쉬를 만들때 필요하다
	//	ModelData* data = LoadManager::GetMesh(ColliderName);
	//
	//	TriangleMeshData* Triangle = new TriangleMeshData();
	//	int IndexSize	= data->TopMeshList[0]->m_OriginIndexListCount;
	//	int VertexSize	= data->TopMeshList[0]->m_OriginVertexListCount;
	//
	//	Triangle->VertexList		= data->TopMeshList[0]->m_OriginVertexList;
	//	Triangle->VertexListSize	= VertexSize;
	//
	//	Triangle->IndexList		= data->TopMeshList[0]->m_OriginIndexList;
	//	Triangle->IndexListSize = IndexSize;
	//
	//	RigidbodyData->CreateTriangleCollider(Triangle,VertexSize,IndexSize);
	//} 
	////RigidbodyData->Rotation = {0,0,0,1};
	////RigidbodyData->SetWorldPosition(Tr->Position.x, Tr->Position.y, Tr->Position.z);
	//PhysX_Create_Actor(RigidbodyData);
	////Tr->Load_Local._41 += RigidbodyData->CenterPoint.x;
	////Tr->Load_Local._42 += RigidbodyData->CenterPoint.y;
	////Tr->Load_Local._43 += RigidbodyData->CenterPoint.z;
	//
	//// Collider Data 설정..
	//gameobject->OneMeshData->Collider_Data = Collider_Data;
}

void Rigidbody::SetUp()
{
	/// 시작 값을 넣어준다
	//Tr = gameobject->GetTransform();
}

void Rigidbody::PhysicsUpdate()
{
	if (isCreate == false) { return; }
	
	if (mTransform->Position != RigidbodyData->WorldPosition)
	{
		RigidbodyData->SetWorldPosition(mTransform->Position.x, mTransform->Position.y, mTransform->Position.z);
		mTransform->Position = RigidbodyData->WorldPosition;
	}
	PhysX_Update_Actor(RigidbodyData);
	
	float CenterX = RigidbodyData->CenterPoint.x;
	float CenterY = RigidbodyData->CenterPoint.y;
	float CenterZ = RigidbodyData->CenterPoint.z;
	
	mTransform->Position.x = RigidbodyData->WorldPosition.x;
	mTransform->Position.y = RigidbodyData->WorldPosition.y;
	mTransform->Position.z = RigidbodyData->WorldPosition.z;
	
	mTransform->Q_Rotation.x = RigidbodyData->Rotation.x;
	mTransform->Q_Rotation.y = RigidbodyData->Rotation.y;
	mTransform->Q_Rotation.z = RigidbodyData->Rotation.z;
	mTransform->Q_Rotation.w = RigidbodyData->Rotation.w;
}

void Rigidbody::SetRotation(float x, float y, float z, float w)
{
	RigidbodyData->Rotation = Vector4(x, y, z, w);
}

void Rigidbody::SetRotation(float x, float y, float z)
{
	Quaternion Q = Quaternion::CreateFromYawPitchRoll(y, x, z);

	RigidbodyData->Rotation = Vector4(Q.x, Q.y, Q.z, Q.w);
}

void Rigidbody::SetPosition(float x, float y, float z)
{
	RigidbodyData->SetWorldPosition(x, y, z);
}

 bool Rigidbody::GetTriggerEnter()
{
	 return RigidbodyData->GetTriggerEnter();
}

 bool Rigidbody::GetTriggerStay()
{
	return RigidbodyData->GetTriggerStay();
}

 bool Rigidbody::GetTriggerExit()
{
	return RigidbodyData->GetTriggerExit();
}

int Rigidbody::GetTriggerCount()
{
	return RigidbodyData->TriggerCount;
}

 void Rigidbody::SetType(bool Dinamic)
{
	 //객체의 타입을 설정한다 (Dinamic = 움직이는거 , Static = 안움직임 )
	 //자세한건 PhysData.h 에 주석으로 써넣옴
	 RigidbodyData->isDinamic = Dinamic;
}
void Rigidbody::SetGrvity(bool grvity)
{
	//중력 작용 여부를 설정한다 
	RigidbodyData->isGrvity = grvity;
}

void Rigidbody::SetKinematic(bool kinematic)
{
	RigidbodyData->isKinematic = kinematic;
}

void Rigidbody::SetMass(float mass)
{
	RigidbodyData->MT_Mass = mass;
}

void Rigidbody::SetFreezePosition(bool x, bool y, bool z)
{
	RigidbodyData->SetLockAxis_Position(x, y, z);
}

void Rigidbody::SetFreezeRotation(bool x, bool y, bool z)
{
	RigidbodyData->SetLockAxis_Rotation(x, y, z);
}

void Rigidbody::SetVelocity(float x, float y, float z)
{
	RigidbodyData->SetVelocity(x, y, z);
}

Vector3 Rigidbody::GetVelocity()
{
	return RigidbodyData->GetVelocity();
}

 void Rigidbody::SetAddForce(float x, float y, float z)
 {
	 RigidbodyData->AddForce(x, y, z);
 }

 void Rigidbody::SetCenterPoint(float x, float y, float z)
 {
	 RigidbodyData->CenterPoint = {x,y,z}; 
 }





