#include "SceneSave.h"

#include "MainHeader.h"
#include "EaterHeader.h"
#include "GameObject.h"
#include "EngineData.h"
#include "Demo.h"

#include "Light.h"
#include "Camera.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "ParticleSystem.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "AnimationController.h"
#include "PhysData.h"

SceneSave::SceneSave()
{

}

SceneSave::~SceneSave()
{

}

void SceneSave::Initialize(std::map<std::string, GameObject*>* ObjectList)
{
	SaveList = ObjectList;
}

void SceneSave::Save(std::string SaveFilePath, std::string SaveFileName)
{
	EATER_OPEN_WRITE_FILE(SaveFileName, SaveFilePath,".Scene");
	std::map<std::string, GameObject*>::iterator Start_it	= SaveList->begin();
	std::map<std::string, GameObject*>::iterator End_it		= SaveList->end();
	
	//씬의 옵션 저장
	SceneOption();

	for (Start_it; Start_it != End_it; Start_it++)
	{
		GameObject* Object		= Start_it->second;
		MeshData*	OneMesh		= Object->OneMeshData;

		//어떤 컨퍼넌트를 들어있는지 몰라서 모두 찾는다
		Transform*	mTransform	= Object->GetComponent<Transform>();
		MeshFilter* mMeshFilter = Object->GetComponent<MeshFilter>();
		Light*		mLight		= Object->GetComponent<Light>();
		Camera*		mCamera		= Object->GetComponent<Camera>();
		Collider*	mCollider	= Object->GetComponent<Collider>();
		Rigidbody*	mRigidbody	= Object->GetComponent<Rigidbody>();
		AnimationController*	mAnimationController	= Object->GetComponent<AnimationController>();
		ParticleSystem*			mParticleSystem			= Object->GetComponent<ParticleSystem>();


		//오브젝트 저장 시작
		EATER_SET_NODE("GAMEOBJECT");

		//공통 데이터 저장
		EATER_SET_MAP("NAME", Object->Name);
		EATER_SET_MAP("TAG", std::to_string(Object->GetTag()));

		//컨퍼넌트 저장
		SaveTransform(mTransform);
		if (mMeshFilter != nullptr) {SaveMeshFilter(mMeshFilter);}
		if (mLight != nullptr) { SaveLight(mLight);}
		if (mCamera != nullptr) { SaveCamera(mCamera);}
		if (mCollider != nullptr) { SaveCollider(mCollider);}
		if (mRigidbody != nullptr) { SaveRigidbody(mRigidbody);}
		if (mAnimationController != nullptr) { SaveAnimation(mAnimationController);}
		if (mParticleSystem != nullptr) { SaveParticle(mParticleSystem);}	
	}

	//저장 종료
	EATER_CLOSE_WRITE_FILE();
}



void SceneSave::SceneOption()
{
	EATER_SET_NODE("SCENE");
	std::map<int, std::string>::iterator Start_it	= Demo::TagList.begin();
	std::map<int, std::string>::iterator End_it		= Demo::TagList.end();

	int Size = (int)Demo::TagList.size();
	EATER_SET_LIST_START("TAG", Size, 2);
	for (Start_it; Start_it != End_it; Start_it++)
	{
		EATER_SET_LIST(Start_it->first);
		EATER_SET_LIST(Start_it->second, true);
	}
}

void SceneSave::SaveTransform(Transform* mTransform)
{
	EATER_SET_LIST_START("Transform", 1, 9);
	EATER_SET_LIST(mTransform->Position.x);
	EATER_SET_LIST(mTransform->Position.y);
	EATER_SET_LIST(mTransform->Position.z);
	EATER_SET_LIST(mTransform->Rotation.x);
	EATER_SET_LIST(mTransform->Rotation.y);
	EATER_SET_LIST(mTransform->Rotation.z);
	EATER_SET_LIST(mTransform->Scale.x);
	EATER_SET_LIST(mTransform->Scale.y);
	EATER_SET_LIST(mTransform->Scale.z, true);
}

void SceneSave::SaveMeshFilter(MeshFilter* mMeshFilter)
{
	//매쉬필터 데이터를 저장
	EATER_SET_LIST_START("MeshFilter",1,3);
	EATER_SET_LIST(mMeshFilter->GetModelName());
	EATER_SET_LIST(mMeshFilter->GetMaterialName());
	EATER_SET_LIST(mMeshFilter->GetBufferName(), true);
}

void SceneSave::SaveAnimation(AnimationController* AC)
{
	EATER_SET_LIST_START("Animation", 1, 1);
	EATER_SET_LIST(AC->GetNowAnimationName(), true);
}

void SceneSave::SaveLight(Light* mLight)
{
	EATER_SET_LIST_START("Light", 1, 8);
	LIGHT_TYPE mtype = mLight->GetType();
	switch (mtype)
	{
	case NONE_LIGHT:
		EATER_SET_LIST("NONE");
		break;
	case DIRECTION_LIGHT:
		EATER_SET_LIST("DIRECTION");
		break;
	case POINT_LIGHT:
		EATER_SET_LIST("POINT");
		break;
	case SPOT_LIGHT:
		EATER_SET_LIST("SPOT");
		break;
	}
	float Angle = mLight->GetAngle();
	EATER_SET_LIST(mLight->GetAngle());
	EATER_SET_LIST(mLight->GetAttenuate());
	EATER_SET_LIST(mLight->GetRange());
	EATER_SET_LIST(mLight->GetPower());
	Vector3 Color = mLight->GetColor();
	EATER_SET_LIST(Color.x);
	EATER_SET_LIST(Color.y);
	EATER_SET_LIST(Color.z,true);
}

void SceneSave::SaveParticle(ParticleSystem* mParticleSystem)
{

}

void SceneSave::SaveCollider(Collider* mCollider)
{
	PhysCollider* mPhys = mCollider->GetCollider();
	EATER_SET_LIST_START("Collider", 1, 12);
	
	int num = (int)mPhys->GetType();
	EATER_SET_LIST((int)mPhys->GetType());	//0.타입

	Vector3 Size = mPhys->GetSize();
	EATER_SET_LIST(Size.x);					//1.사이즈X
	EATER_SET_LIST(Size.y);					//2.사이즈Y
	EATER_SET_LIST(Size.z);					//3.사이즈Z

	EATER_SET_LIST((int)mPhys->GetTrigger()); // 4.트리거 여부
	
	Vector3 Center = mPhys->GetCenter();
	EATER_SET_LIST(Center.x);					//5.센터X
	EATER_SET_LIST(Center.y);					//6.센터Y
	EATER_SET_LIST(Center.z);					//7.센터Z
	

	EATER_SET_LIST(mCollider->GetMaterial_Dynamic());			//8. 재질
	EATER_SET_LIST(mCollider->GetMaterial_Restitution());		//9. 재질
	EATER_SET_LIST(mCollider->GetMaterial_Static());			//10. 재질
	//PhysCollider::TriangleMeshData* Data = mPhys->GetTriangleMesh();
	EATER_SET_LIST("",true);
	

}

void SceneSave::SaveRigidbody(Rigidbody* mRigidbody)
{
	EATER_SET_LIST_START("Rigidbody", 1, 9);

	float Mass		= mRigidbody->GetMass();
	bool Gravity	= mRigidbody->GetGravity();
	bool Kinematic	= mRigidbody->GetKinematic();
	Vector3 Pos = mRigidbody->GetFreezePosition();
	Vector3 Rot = mRigidbody->GetFreezeRotation();

	EATER_SET_LIST(Mass);
	EATER_SET_LIST(Gravity);
	EATER_SET_LIST(Kinematic);

	EATER_SET_LIST(Pos.x);
	EATER_SET_LIST(Pos.y);
	EATER_SET_LIST(Pos.z);

	EATER_SET_LIST(Rot.x);
	EATER_SET_LIST(Rot.y);
	EATER_SET_LIST(Rot.z,true);

}

void SceneSave::SaveCamera(Camera* mCamera)
{
	EATER_SET_LIST_START("Camera", 1, 1);
	EATER_SET_LIST("Cam",true);
}