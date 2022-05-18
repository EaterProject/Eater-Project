#include "pch.h"
#include "Editor.h"
#include "DialogFactory.h"
#include "Loading.h"
#include "FileOption.h"
#include "AssetsDialog.h"
#include "CreateMaterial.h"
#include "CamAnimation.h"
#include "SceneSaveDialog.h"
#include "LoadNavMesh.h"
#include "LoadTerrain.h"
#include "CustomDialog.h"
#include "SceneSetting.h"
#include "SkySetting.h"



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

	mLoadNavMesh = new LoadNavMesh();
	mLoadNavMesh->Create(IDD_LOAD_NAVMESH);
	mLoadNavMesh->ShowWindow(SW_HIDE);

	mLoadTerrain = new LoadTerrain();
	mLoadTerrain->Create(IDD_LOAD_TERRAIN);
	mLoadTerrain->ShowWindow(SW_HIDE);

	mSceneSetting = new SceneSetting();
	mSceneSetting->Create(IDD_SCENE_SETTING);
	mSceneSetting->ShowWindow(SW_HIDE);

	mSkySetting = new SkySetting();
	mSkySetting->Create(IDD_SKY_SETTING);
	mSkySetting->ShowWindow(SW_HIDE);



	
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
	CustomDlgDelete(mLoading_Dlog);
	CustomDlgDelete(mMaterial_Dlog);
	CustomDlgDelete(mCamAnimation_Dlog);
	CustomDlgDelete(mFileOption_Dlog);
	CustomDlgDelete(mSceneSave);
	CustomDlgDelete(mLoadNavMesh);
	CustomDlgDelete(mLoadTerrain);

	CustomDlgDelete(mTransform);
	CustomDlgDelete(mAnimation);
	CustomDlgDelete(mMeshFilter);
	CustomDlgDelete(mPrticle);
	CustomDlgDelete(mLight);
	CustomDlgDelete(mRigidbody);
	CustomDlgDelete(mCollider);
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

void DialogFactory::CustomDlgDelete(CustomDialog* Dlg)
{
	Dlg->DestroyWindow();
	delete Dlg;
	Dlg = nullptr;
}
