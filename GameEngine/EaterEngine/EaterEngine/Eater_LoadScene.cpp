#include "Eater_LoadScene.h"
#include "Camera.h"
#include "Light.h"
#include "EaterEngineAPI.h"
#include "EaterHeader.h"


#include "MeshFilter.h"
#include "SkinningFilter.h"
#include "GameObject.h"
#include "AnimationController.h"
#include "Transform.h"
#include "Collider.h"
#include "PhysCollider.h"
#include "ObjectManager.h"
#include "Rigidbody.h"
#include "ParticleSystem.h"
#include "GameObject.h"
#include "LoadManager.h"

Eater_LoadScene::Eater_LoadScene()
{

}

Eater_LoadScene::~Eater_LoadScene()
{

}

void Eater_LoadScene::LoadData(std::string& FilePath)
{
	EATER_OPEN_READ_FILE(FilePath);
	int Count = EATER_GET_NODE_COUNT();
	for (int i = 0; i < Count; i++)
	{
		std::string NodeName = EATER_GET_NODE_NAME(i);
		if (NodeName == "GAMEOBJECT")
		{
			GameObject* Obj = Instance();
			Obj->SetTag(std::stoi(EATER_GET_MAP(i, "TAG")));
			Obj->Name = EATER_GET_MAP(i, "NAME");

			//트랜스폼은 기본
			Load_Component_Transform(i, Obj);

			int Find = 0;
			//매쉬필터
			Find = EATER_GET_LIST_CHOICE(i, "MeshFilter");
			if (Find != -1)
			{
				Load_Component_MeshFilter(i, Obj);
			}

			//애니메이션
			Find = EATER_GET_LIST_CHOICE(i, "Animation");
			if (Find != -1)
			{
				Load_Component_Animation(i, Obj);
			}

			//라이트
			Find = EATER_GET_LIST_CHOICE(i, "Light");
			if (Find != -1)
			{
				Load_Component_Light(i, Obj);
			}

			Find = EATER_GET_LIST_CHOICE(i, "Rigidbody");
			if (Find != -1)
			{
				Load_Component_Rigidbody(i, Obj);
			}

			Find = EATER_GET_LIST_CHOICE(i, "Collider");
			if (Find != -1)
			{
				Load_Component_Collider(i, Obj);
			}


			//카메라
			Find = EATER_GET_LIST_CHOICE(i, "Camera");
			if (Find != -1)
			{
				Load_Component_Camera(i, Obj);
			}

			//파티클
			Find = EATER_GET_LIST_CHOICE(i, "Particle");
			if (Find != -1)
			{
				Load_Component_Particle(i, Obj);
			}

			//LoadObject->insert({ Obj->Name,Obj });
		}
		else if (NodeName == "SCENE")
		{
			int Find = EATER_GET_LIST_CHOICE(i, "TAG");
			for (int i = 0; i < Find; i++)
			{
				std::vector<std::string> Data;
				EATER_GET_LIST(&Data, i);
				 ObjectManager::AddTag(std::stoi(Data[0]), Data[1]);
			}

			RenderOption* mOption = GetRenderOptionData();
			Find = EATER_GET_LIST_CHOICE(i, "OPTION");
			std::vector<float> Data;
			EATER_GET_LIST(&Data, i);
			mOption->AO_Radius = Data[0];
			mOption->AO_SurfaceEpsilon = Data[1];
			mOption->AO_BlurCount = Data[2];
			mOption->FOG_Color.x = Data[3];
			mOption->FOG_Color.y = Data[4];
			mOption->FOG_Color.z = Data[5];
			mOption->FOG_MoveSpeed = Data[6];
			mOption->FOG_StartDistance = Data[7];
			mOption->FOG_DistanceOffset = Data[8];
			mOption->FOG_DistanceValue = Data[9];
			mOption->FOG_HeightOffset = Data[10];
			mOption->FOG_HeightValue = Data[11];
			mOption->SkyCube_Size = Data[12];
			mOption->BLOOM_Threshold_Min = Data[13];
			mOption->BLOOM_Threshold_Max = Data[14];
			mOption->BLOOM_Factor = Data[15];
			mOption->SkyLight_Factor = Data[16];
			RenderSetting();
		}
	}
	EATER_CLOSE_READ_FILE();
}

void Eater_LoadScene::LoadPrefap(std::string Path)
{
	

	EATER_OPEN_READ_FILE(Path);
	int Count = EATER_GET_NODE_COUNT();
	for (int i = 0; i < Count; i++)
	{
		std::string NodeName = EATER_GET_NODE_NAME(i);
		if (NodeName == "GAMEOBJECT")
		{
			GameObject* Obj = Instance();
			Obj->SetTag(std::stoi(EATER_GET_MAP(i, "TAG")));
			Obj->Name = EATER_GET_MAP(i, "NAME");

			//트랜스폼은 기본
			Load_Component_Transform(i, Obj);

			int Find = 0;
			//매쉬필터
			Find = EATER_GET_LIST_CHOICE(i, "MeshFilter");
			if (Find != -1)
			{
				Load_Component_MeshFilter(i, Obj);
			}

			//애니메이션
			Find = EATER_GET_LIST_CHOICE(i, "Animation");
			if (Find != -1)
			{
				Load_Component_Animation(i, Obj);
			}

			//라이트
			Find = EATER_GET_LIST_CHOICE(i, "Light");
			if (Find != -1)
			{
				Load_Component_Light(i, Obj);
			}

			Find = EATER_GET_LIST_CHOICE(i, "Rigidbody");
			if (Find != -1)
			{
				Load_Component_Rigidbody(i, Obj);
			}

			Find = EATER_GET_LIST_CHOICE(i, "Collider");
			if (Find != -1)
			{
				Load_Component_Collider(i, Obj);
			}


			//카메라
			Find = EATER_GET_LIST_CHOICE(i, "Camera");
			if (Find != -1)
			{
				Load_Component_Camera(i, Obj);
			}

			//파티클
			Find = EATER_GET_LIST_CHOICE(i, "Particle");
			if (Find != -1)
			{
				Load_Component_Particle(i, Obj);
			}
		}
		LoadManager::PrefapList.insert({ CutStr(Path),nullptr});
	}
}

void Eater_LoadScene::Load_Component_Light(int index, GameObject* Object)
{
	Light* mLight = Object->AddComponent<Light>();
	std::vector<std::string> Data;
	EATER_GET_LIST(&Data, 0);
	std::string mType = Data[0];
	if (mType == "NONE")
	{
		mLight->SetType(LIGHT_TYPE::NONE_LIGHT);
	}
	else if (mType == "DIRECTION")
	{
		mLight->SetType(LIGHT_TYPE::DIRECTION_LIGHT);
	}
	else if (mType == "POINT")
	{
		mLight->SetType(LIGHT_TYPE::POINT_LIGHT);
	}
	else if (mType == "SPOT")
	{
		mLight->SetType(LIGHT_TYPE::SPOT_LIGHT);
	}

	mLight->SetAngle(std::stof(Data[1]) * 180 / 3.141592f);
	mLight->SetAttenuate(std::stof(Data[2]));
	mLight->SetRange(std::stof(Data[3]));
	mLight->SetPower(std::stof(Data[4]));

	float R, G, B;
	R = std::stof(Data[5]);
	G = std::stof(Data[6]);
	B = std::stof(Data[7]);
	mLight->SetColor(R, G, B);
}

void Eater_LoadScene::Load_Component_Particle(int index, GameObject* Object)
{
	ParticleSystem* mParticle = Object->AddComponent<ParticleSystem>();
	std::vector<std::string> Data;
	EATER_GET_LIST(&Data, 0);
	mParticle->SetDiffuseName(Data[0]);
	std::string Type = Data[1];
	if (Type == "BILLBOARD") { mParticle->SetRenderType(BILLBOARD); }
	else if (Type == "VERTICAL") { mParticle->SetRenderType(VERTICAL_BILLBOARD); }
	else if (Type == "HORIZONTAL") { mParticle->SetRenderType(HORIZONTAL_BILLBOARD); }
	else if (Type == "MESH") { mParticle->SetRenderType(MESH); }
	mParticle->SetMaxParticles(std::stoi(Data[2]));
	mParticle->SetDelayTime(std::stof(Data[3]));
	mParticle->SetShapeRadius(std::stof(Data[4]), std::stof(Data[5]), std::stof(Data[6]));
	mParticle->SetTextureTiling(std::stoi(Data[7]), std::stoi(Data[8]));

	Vector3 Min = { std::stof(Data[9]),std::stof(Data[10]),std::stof(Data[11]) };
	Vector3 Max = { std::stof(Data[12]),std::stof(Data[13]),std::stof(Data[14]) };
	mParticle->SetStartForce(Min, Max);

	Vector4 Min4 = { std::stof(Data[15]) ,std::stof(Data[16]),std::stof(Data[17]),std::stof(Data[18]) };
	Vector4 Max4 = { std::stof(Data[19]) ,std::stof(Data[20]),std::stof(Data[21]),std::stof(Data[22]) };
	mParticle->SetStartColor(Min4, Max4);
	mParticle->SetStartLifeTime(std::stof(Data[23]), std::stof(Data[24]));


	EATER_GET_LIST(&Data, 1);
	mParticle->SetStartScale(std::stof(Data[0]), std::stof(Data[1]));
	mParticle->SetStartRotation(std::stof(Data[2]), std::stof(Data[3]));

	Min = { std::stof(Data[4]),std::stof(Data[5]),std::stof(Data[6]) };
	Max = { std::stof(Data[7]),std::stof(Data[8]),std::stof(Data[9]) };
	mParticle->SetLifeTimeForce(Min, Max);

	Min4 = { std::stof(Data[10]) ,std::stof(Data[11]),std::stof(Data[12]),std::stof(Data[13]) };
	Max4 = { std::stof(Data[14]) ,std::stof(Data[15]),std::stof(Data[16]),std::stof(Data[17]) };
	Type = Data[18];
	if (Type == "NONE") { mParticle->SetLifeTimeColor(Min4, Max4, NONE); }
	else if (Type == "UP") { mParticle->SetLifeTimeColor(Min4, Max4, UP); }
	else if (Type == "DOWN") { mParticle->SetLifeTimeColor(Min4, Max4, DOWN); }
	else if (Type == "UPDOWN") { mParticle->SetLifeTimeColor(Min4, Max4, UPDOWN); }

	Vector2 Scale = { std::stof(Data[19]), std::stof(Data[20]) };
	Type = Data[21];
	if (Type == "NONE") { mParticle->SetLifeTimeScale(Scale.x, Scale.y, NONE); }
	else if (Type == "UP") { mParticle->SetLifeTimeScale(Scale.x, Scale.y, UP); }
	else if (Type == "DOWN") { mParticle->SetLifeTimeScale(Scale.x, Scale.y, DOWN); }
	else if (Type == "UPDOWN") { mParticle->SetLifeTimeScale(Scale.x, Scale.y, UPDOWN); }
	mParticle->SetLifeTimeRotation(std::stof(Data[22]), std::stof(Data[23]));
	mParticle->SetRateOverTime(std::stof(Data[24]));
	mParticle->SetMeshName("Quad");
	mParticle->SetPlayTime(1, true);
	mParticle->Play();
	mParticle->DataUpdate();
}

void Eater_LoadScene::Load_Component_Collider(int index, GameObject* Object)
{
	Collider* mCollider = Object->AddComponent<Collider>();
	PhysCollider* mPhys = mCollider->GetCollider();
	std::vector<std::string> Data;
	EATER_GET_LIST(&Data, 0);

	int Type = std::stoi(Data[0]);
	switch (Type)
	{
	case 0:
		mCollider->SetBoxCollider(std::stof(Data[1]), std::stof(Data[2]), std::stof(Data[3]));
		break;
	case 1:
		mCollider->SetSphereCollider(std::stof(Data[1]));
		break;
	case 2:
		mCollider->SetCapsuleCollider(std::stof(Data[1]), std::stof(Data[2]));
		break;
	}

	mPhys->SetTrigger(std::stoi(Data[4]));
	mPhys->SetCenter(std::stof(Data[5]), std::stof(Data[6]), std::stof(Data[7]));

	mCollider->SetMaterial_Dynamic(std::stof(Data[8]));
	mCollider->SetMaterial_Restitution(std::stof(Data[9]));
	mCollider->SetMaterial_Static(std::stof(Data[10]));
	if (Type == 3)
	{
		mCollider->SetTriangleCollider(Data[11]);
	}
	//mCollider->CreatePhys();
}

void Eater_LoadScene::Load_Component_Rigidbody(int index, GameObject* Object)
{
	Rigidbody* mRigidbody = Object->AddComponent<Rigidbody>();
	std::vector<std::string> Data;
	EATER_GET_LIST(&Data, 0);

	mRigidbody->SetMass(std::stof(Data[0]));
	mRigidbody->SetGravity(std::stoi(Data[1]));
	mRigidbody->SetKinematic(std::stoi(Data[2]));


	bool PosX = std::stoi(Data[3]);
	bool PosY = std::stoi(Data[4]);
	bool PosZ = std::stoi(Data[5]);
	mRigidbody->SetFreezePosition(PosX, PosY, PosZ);

	bool RotX = std::stoi(Data[6]);
	bool RotY = std::stoi(Data[7]);
	bool RotZ = std::stoi(Data[8]);
	mRigidbody->SetFreezeRotation(RotX, RotY, RotZ);
}

void Eater_LoadScene::Load_Component_Camera(int index, GameObject* Object)
{
	Object->AddComponent<Camera>();
}

void Eater_LoadScene::Load_Component_Animation(int index, GameObject* Object)
{
	MeshFilter* MF = Object->GetComponent<MeshFilter>();
	AnimationController* AC = Object->AddComponent<AnimationController>();
	std::vector<std::string> Data;
	EATER_GET_LIST(&Data, 0);
	MF->SetAnimationName(MF->GetModelName());
	AC->Choice(Data[0]);
	AC->Play();
}

void Eater_LoadScene::Load_Component_Transform(int index, GameObject* Object)
{
	Transform* mTransform = Object->GetTransform();
	EATER_GET_LIST_CHOICE(index, "Transform");
	std::vector<float> Data;
	EATER_GET_LIST(&Data, 0);
	mTransform->Position = { Data[0],Data[1] ,Data[2] };
	mTransform->Rotation = { Data[3],Data[4] ,Data[5] };
	mTransform->Scale = { Data[6],Data[7] ,Data[8] };
}

void Eater_LoadScene::Load_Component_MeshFilter(int index, GameObject* Object)
{
	MeshFilter* mMeshFilter = Object->AddComponent<MeshFilter>();
	std::vector<std::string> Data;
	EATER_GET_LIST(&Data, 0);
	if (Data[0] != "NO")
	{
		mMeshFilter->SetModelName(Data[0]);
	}

	if (Data[1] != "NO")
	{
		mMeshFilter->SetMaterialName(Data[1]);
	}

	if (Data[2] != "NO")
	{
		mMeshFilter->SetMeshName(Data[2]);
	}
}
