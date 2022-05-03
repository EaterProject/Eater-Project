#include "pch.h"
#include "Editor.h"
#include "DialogFactory.h"
#include "Loading.h"
#include "FileOption.h"
#include "AssetsDialog.h"
#include "CreateMaterial.h"
#include "CamAnimation.h"
#include "SceneSaveDialog.h"



#include "CTAP_Animation.h";
#include "CTAP_Collider.h";
#include "CTAP_Light.h";
#include "CTAP_MeshFilter.h";
#include "CTAP_Particle.h";
#include "CTAP_Rigidbody.h";
#include "CTAP_Transform.h";


DialogFactory* DialogFactory::instance = nullptr;
DialogFactory::DialogFactory()
{
	mLoading_Dlog		= nullptr;
	mFileOption_Dlog	= nullptr;
	mRightOption_Dlog	= nullptr;
	mAssets_Dlog		= nullptr;
}

DialogFactory::~DialogFactory()
{



}

void DialogFactory::Initialize()
{
	mLoading_Dlog = new Loading();
	mLoading_Dlog->Create(IDD_LOADING);
	mLoading_Dlog->ShowWindow(SW_HIDE);

	mMaterial_Dlog = new CreateMaterial();
	mMaterial_Dlog->Create(IDD_CREATE_MATERIAL);
	mMaterial_Dlog->ShowWindow(SW_HIDE);

	mCamAnimation_Dlog = new CamAnimation();
	mCamAnimation_Dlog->Create(IDD_CAM_ANIMATION);
	mCamAnimation_Dlog->ShowWindow(SW_HIDE);

	mFileOption_Dlog = new FileOption();
	mFileOption_Dlog->Create(IDD_FILE_OPTION);
	mFileOption_Dlog->ShowWindow(SW_HIDE);

	mSceneSave = new SceneSaveDialog();
	
	
	mTransform	= new CTAP_Transform();
	mAnimation	= new CTAP_Animation();
	mMeshFilter = new CTAP_MeshFilter();
	mPrticle	= new CTAP_Particle();
	mLight		= new CTAP_Light();
	mRigidbody	= new CTAP_Rigidbody();
	mCollider	= new CTAP_Collider();
}

void DialogFactory::Release()
{



}

DialogFactory* DialogFactory::GetFactory()
{
	if (instance == nullptr)
	{
		instance = new DialogFactory();
		instance->Initialize();
	}
	return instance;
}
