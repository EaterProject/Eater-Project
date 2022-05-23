#pragma once
#include "EditorDefine.h"

class EditorManager;
class Loading;
class FileOption;
class RightOption;
class AssetsDialog;
class CreateMaterial;
class CamAnimation;
class SceneSaveDialog;
class LoadTerrain;
class LoadNavMesh;
class SceneSetting;
class SkySetting;


class CTAP_Animation;
class CTAP_Collider;
class CTAP_Light;
class CTAP_MeshFilter;
class CTAP_Particle;
class CTAP_Rigidbody;
class CTAP_Transform;


class CustomDialog;
class DialogFactory
{
public: 
	DialogFactory();
	~DialogFactory();

	void Initialize();
	void Release();

public:
	///GET
	GETTER(SkySetting,		mSkySetting);
	GETTER(LoadTerrain,		mLoadTerrain);
	GETTER(LoadNavMesh,		mLoadNavMesh);
	GETTER(SceneSetting,	mSceneSetting);
	GETTER(Loading,			mLoading_Dlog);
	GETTER(FileOption,		mFileOption_Dlog);
	GETTER(CamAnimation,	mCamAnimation_Dlog);
	GETTER(CreateMaterial,	mMaterial_Dlog);
	GETTER(SceneSaveDialog, mSceneSave);
	GETTER(CTAP_Transform,	mTransform);
	GETTER(CTAP_Rigidbody,	mRigidbody);
	GETTER(CTAP_Collider,	mCollider);
	GETTER(CTAP_Animation,	mAnimation);
	GETTER(CTAP_MeshFilter, mMeshFilter);
	GETTER(CTAP_Particle,	mPrticle);
	GETTER(CTAP_Light,		mLight);

	GETTER(RightOption,		mRightOption_Dlog);
	GETTER(AssetsDialog,	mAssets_Dlog);
	GETTER(EditorManager,   mEditorManager)

	///SET
	SETTER(RightOption, mRightOption_Dlog);
	SETTER(AssetsDialog, mAssets_Dlog);
public:
	static DialogFactory* GetFactory();
private:
	static DialogFactory* instance;
private:
	//옵션 다이얼로그
	SceneSetting*		mSceneSetting;
	Loading*			mLoading_Dlog;		//로딩을 위한 다이얼로그
	FileOption*			mFileOption_Dlog;	//
	CreateMaterial*		mMaterial_Dlog;		//메테리얼 생성 다이얼 로그
	CamAnimation*		mCamAnimation_Dlog;	//카메라 애니메이션 다이얼 로그
	SceneSaveDialog*	mSceneSave;
	LoadTerrain*		mLoadTerrain;
	LoadNavMesh*		mLoadNavMesh;
	SkySetting*			mSkySetting;
private:
	//알아서 생성되는 다이얼로그
	RightOption*	mRightOption_Dlog;	//View의 오른쪽 모든 정보들을 담당하는 다이얼 로그
	AssetsDialog*	mAssets_Dlog;		//Assets 관련 다이얼 로그
private:
	//텝에서 보여줄 Component View
	CTAP_Rigidbody*		mRigidbody;		//rigidbody를 보여줄 탭
	CTAP_Collider*		mCollider;		//Collider를 보여줄 탭
	CTAP_Transform*		mTransform;		//Transform를 보여줄 탭
	CTAP_Animation*		mAnimation;		//Animation를 보여줄 탭
	CTAP_MeshFilter*	mMeshFilter;	//MeshFiltet를 보여줄 탭
	CTAP_Particle*		mPrticle;		//MeshFiltet를 보여줄 탭
	CTAP_Light*			mLight;			//Light 를 보여줄 탭
private:
	EditorManager*		mEditorManager;
private:
	void CustomDlgDelete(CustomDialog* Dlg);
};
