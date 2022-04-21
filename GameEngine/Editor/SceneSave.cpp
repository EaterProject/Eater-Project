#include "SceneSave.h"

#include "MainHeader.h"
#include "EaterHeader.h"
#include "GameObject.h"
#include "EngineData.h"
#include "EditorToolScene.h"

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
		Object->Name = Start_it->first;

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
	std::map<int, std::string>::iterator Start_it	= EditorToolScene::TagList.begin();
	std::map<int, std::string>::iterator End_it		= EditorToolScene::TagList.end();

	int Size = (int)EditorToolScene::TagList.size();
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
	//46
	EATER_SET_LIST_START("Particle", 2, 23);
	switch (mParticleSystem->GetRenderType())
	{
	case PARTICLE_RENDER_OPTION::BILLBOARD:
		EATER_SET_LIST("BILLBOARD");
		break;
	case PARTICLE_RENDER_OPTION::HORIZONTAL_BILLBOARD:
		EATER_SET_LIST("HORIZONTAL");
		break;
	case PARTICLE_RENDER_OPTION::VERTICAL_BILLBOARD:
		EATER_SET_LIST("VERTICAL");
		break;
	case PARTICLE_RENDER_OPTION::MESH:
		EATER_SET_LIST("MESH");
		break;
	}
	EATER_SET_LIST(mParticleSystem->GetMaxParticles());		//1
	EATER_SET_LIST(mParticleSystem->GetDelayTime());		//2

	auto ShapeRadius = mParticleSystem->GetShapeRadius();
	EATER_SET_LIST(ShapeRadius.x);							//3
	EATER_SET_LIST(ShapeRadius.y);							//4
	EATER_SET_LIST(ShapeRadius.z);							//5

	auto StartForce = mParticleSystem->GetStartForce();
	EATER_SET_LIST(StartForce.m_Min.x);						//6
	EATER_SET_LIST(StartForce.m_Min.y);						//7
	EATER_SET_LIST(StartForce.m_Min.z);						//8
	EATER_SET_LIST(StartForce.m_Max.x);						//9
	EATER_SET_LIST(StartForce.m_Max.y);						//10
	EATER_SET_LIST(StartForce.m_Max.z);						//11

	auto StartColor = mParticleSystem->GetStartColor();
	EATER_SET_LIST(StartColor.m_Min.x);						//12
	EATER_SET_LIST(StartColor.m_Min.y);						//13
	EATER_SET_LIST(StartColor.m_Min.z);						//14
	EATER_SET_LIST(StartColor.m_Min.w);						//15
	EATER_SET_LIST(StartColor.m_Max.x);						//16
	EATER_SET_LIST(StartColor.m_Max.y);						//17
	EATER_SET_LIST(StartColor.m_Max.z);						//18
	EATER_SET_LIST(StartColor.m_Max.w);						//19

	auto StartLifeTime = mParticleSystem->GetStartLifeTime();
	EATER_SET_LIST(StartLifeTime.m_Min);					//20
	EATER_SET_LIST(StartLifeTime.m_Max);					//21
	
	auto StartScale = mParticleSystem->GetStartScale();
	EATER_SET_LIST(StartScale.m_Min,true);					//22
	EATER_SET_LIST(StartScale.m_Max);						//23
	
	auto StartRotation = mParticleSystem->GetStartRotation();
	EATER_SET_LIST(StartRotation.m_Min);					//24
	EATER_SET_LIST(StartRotation.m_Max);					//25
	
	auto LifeForce = mParticleSystem->GetLifeTimeForce();
	EATER_SET_LIST(LifeForce.m_Min.x);						//26	
	EATER_SET_LIST(LifeForce.m_Min.y);						//27
	EATER_SET_LIST(LifeForce.m_Min.z);						//28
	EATER_SET_LIST(LifeForce.m_Max.x);						//29
	EATER_SET_LIST(LifeForce.m_Max.y);						//30
	EATER_SET_LIST(LifeForce.m_Max.z);						//31
	
	auto LifeColor = mParticleSystem->GetLifeTimeColor();
	EATER_SET_LIST(LifeColor.m_Min.x);
	EATER_SET_LIST(LifeColor.m_Min.y);
	EATER_SET_LIST(LifeColor.m_Min.z);
	EATER_SET_LIST(LifeColor.m_Min.w);
	EATER_SET_LIST(LifeColor.m_Max.x);
	EATER_SET_LIST(LifeColor.m_Max.y);
	EATER_SET_LIST(LifeColor.m_Max.z);
	EATER_SET_LIST(LifeColor.m_Max.w);

	switch (mParticleSystem->GetLifeTimeColorOption())
	{
	case PARTICLE_LIFETIME_OPTION::NONE:
		EATER_SET_LIST("NONE");
		break;
	case PARTICLE_LIFETIME_OPTION::UP:
		EATER_SET_LIST("UP");
		break;
	case PARTICLE_LIFETIME_OPTION::DOWN:
		EATER_SET_LIST("DOWN");
		break;
	case PARTICLE_LIFETIME_OPTION::UPDOWN:
		EATER_SET_LIST("UPDOWN");
		break;
	}
	
	auto LifeScale = mParticleSystem->GetLifeTimeScale();
	EATER_SET_LIST(LifeScale.m_Min);
	EATER_SET_LIST(LifeScale.m_Max);
	
	switch (mParticleSystem->GetLifeTimeScaleOption())
	{
	case PARTICLE_LIFETIME_OPTION::NONE:
		EATER_SET_LIST("NONE");
		break;
	case PARTICLE_LIFETIME_OPTION::UP:
		EATER_SET_LIST("UP");
		break;
	case PARTICLE_LIFETIME_OPTION::DOWN:
		EATER_SET_LIST("DOWN");
		break;
	case PARTICLE_LIFETIME_OPTION::UPDOWN:
		EATER_SET_LIST("UPDOWN");
		break;
	}
	
	auto LifeRotation = mParticleSystem->GetLifeTimeRotation();
	EATER_SET_LIST(LifeRotation.m_Min);
	EATER_SET_LIST(LifeRotation.m_Max,true);
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
	PhysCollider::TriangleMeshData* Data = mPhys->GetTriangleMesh();
	if (Data == nullptr)
	{
		EATER_SET_LIST("NO", true);
	}
	else
	{
		EATER_SET_LIST(Data->Name,true);
	}

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