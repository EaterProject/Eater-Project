#include "E_ParticleManager.h"
#include "MeshFilter.h"
#include "ParticleSystem.h"
#include "GameObject.h"
#include "EaterHeader.h"
#include "TypeOptionHeader.h"
E_ParticleManager::E_ParticleManager()
{
}

E_ParticleManager::~E_ParticleManager()
{
}

void E_ParticleManager::Initialize()
{


}

void E_ParticleManager::LoadData()
{


}

void E_ParticleManager::Release()
{
}

void E_ParticleManager::ChangeEaterFile(GameObject* Obj)
{
	



}

void E_ParticleManager::CreateEaterFile(GameObject* Obj)
{
	EATER_OPEN_WRITE_FILE(FileName, FilePath, FileType);

	ParticleSystem* mParticleSystem = Obj->GetComponent<ParticleSystem>();
	FM_PARTICLE* Data = GET_FORMAT_PARTICLE();
	Data->Particle_Render_Type			= (int)mParticleSystem->GetRenderType();
	Data->Particle_LifeTime_Scale_Type	= (int)mParticleSystem->GetLifeTimeScaleOption();
	Data->Particle_LifeTime_Color_Type	= (int)mParticleSystem->GetLifeTimeColorOption();

	Data->PlayTime	= mParticleSystem->GetPlayTime();
	Data->Loop		=  mParticleSystem->GetLoop();

	Data->TextureName = mParticleSystem->GetTextureName();
	Data->MaxParticle = mParticleSystem->GetMaxParticles();
	Data->DelayTime = mParticleSystem->GetDelayTime();
	auto ShapeRadius = mParticleSystem->GetShapeRadius();
	Data->ShapeRadius_X = ShapeRadius.x;
	Data->ShapeRadius_Y = ShapeRadius.y;
	Data->ShapeRadius_Z = ShapeRadius.z;

	auto mTiling = mParticleSystem->GetTextureTiling();
	Data->TextureTiling_X = mTiling.m_Width;
	Data->TextureTiling_Y = mTiling.m_Height;
	Data->Strength = mParticleSystem->GetStrength();

	auto StartForce = mParticleSystem->GetStartForce();
	Data->StartForce_Min_X = StartForce.m_Min.x;
	Data->StartForce_Min_Y = StartForce.m_Min.y;
	Data->StartForce_Min_Z = StartForce.m_Min.z;
	Data->StartForce_Max_X = StartForce.m_Max.x;
	Data->StartForce_Max_Y = StartForce.m_Max.y;
	Data->StartForce_Max_Z = StartForce.m_Max.z;

	auto StartColor = mParticleSystem->GetStartColor();
	Data->StartColor_Min_R = StartColor.m_Min.x;
	Data->StartColor_Min_G = StartColor.m_Min.y;
	Data->StartColor_Min_B = StartColor.m_Min.z;
	Data->StartColor_Min_A = StartColor.m_Min.w;
	Data->StartColor_Max_R = StartColor.m_Max.x;
	Data->StartColor_Max_G = StartColor.m_Max.y;
	Data->StartColor_Max_B = StartColor.m_Max.z;
	Data->StartColor_Max_A = StartColor.m_Max.w;

	auto StartLifeTime = mParticleSystem->GetStartLifeTime();
	Data->StartLifeTime_Min = StartLifeTime.m_Min;
	Data->StartLifeTime_Max = StartLifeTime.m_Max;

	auto StartScale = mParticleSystem->GetStartScale();
	Data->StartScale_Min = StartScale.m_Min;
	Data->StartScale_Max = StartScale.m_Max;

	auto StartRotation = mParticleSystem->GetStartRotation();
	Data->StartRotation_Min = StartRotation.m_Min;
	Data->StartRotation_Max = StartRotation.m_Max;

	auto LifeForce = mParticleSystem->GetLifeTimeForce();
	Data->LifeForce_Min_X = LifeForce.m_Min.x;
	Data->LifeForce_Min_Y = LifeForce.m_Min.y;
	Data->LifeForce_Min_Z = LifeForce.m_Min.z;
	Data->LifeForce_Max_X = LifeForce.m_Max.x;
	Data->LifeForce_Max_Y = LifeForce.m_Max.y;
	Data->LifeForce_Max_Z = LifeForce.m_Max.z;

	auto LifeColor = mParticleSystem->GetLifeTimeColor();
	Data->LifeColor_Min_R = LifeColor.m_Min.x;
	Data->LifeColor_Min_G = LifeColor.m_Min.y;
	Data->LifeColor_Min_B = LifeColor.m_Min.z;
	Data->LifeColor_Min_A = LifeColor.m_Min.w;
	Data->LifeColor_Max_R = LifeColor.m_Max.x;
	Data->LifeColor_Max_G = LifeColor.m_Max.y;
	Data->LifeColor_Max_B = LifeColor.m_Max.z;
	Data->LifeColor_Max_A = LifeColor.m_Max.w;

	auto LifeScale = mParticleSystem->GetLifeTimeScale();
	Data->LifeScale_Min = LifeScale.m_Min;
	Data->LifeScale_Max = LifeScale.m_Max;

	auto LifeRotation = mParticleSystem->GetLifeTimeRotation();
	Data->LifeRotation_Min = LifeRotation.m_Min;
	Data->LifeRotation_Max = LifeRotation.m_Max;
	Data->RateOverTime = mParticleSystem->GetRateOverTime();
	SET_SAVE_PARTICLE();
	EATER_CLOSE_WRITE_FILE();
}

