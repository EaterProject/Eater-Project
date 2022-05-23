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
	//�ɼ� ���̾�α�
	SceneSetting*		mSceneSetting;
	Loading*			mLoading_Dlog;		//�ε��� ���� ���̾�α�
	FileOption*			mFileOption_Dlog;	//
	CreateMaterial*		mMaterial_Dlog;		//���׸��� ���� ���̾� �α�
	CamAnimation*		mCamAnimation_Dlog;	//ī�޶� �ִϸ��̼� ���̾� �α�
	SceneSaveDialog*	mSceneSave;
	LoadTerrain*		mLoadTerrain;
	LoadNavMesh*		mLoadNavMesh;
	SkySetting*			mSkySetting;
private:
	//�˾Ƽ� �����Ǵ� ���̾�α�
	RightOption*	mRightOption_Dlog;	//View�� ������ ��� �������� ����ϴ� ���̾� �α�
	AssetsDialog*	mAssets_Dlog;		//Assets ���� ���̾� �α�
private:
	//�ܿ��� ������ Component View
	CTAP_Rigidbody*		mRigidbody;		//rigidbody�� ������ ��
	CTAP_Collider*		mCollider;		//Collider�� ������ ��
	CTAP_Transform*		mTransform;		//Transform�� ������ ��
	CTAP_Animation*		mAnimation;		//Animation�� ������ ��
	CTAP_MeshFilter*	mMeshFilter;	//MeshFiltet�� ������ ��
	CTAP_Particle*		mPrticle;		//MeshFiltet�� ������ ��
	CTAP_Light*			mLight;			//Light �� ������ ��
private:
	EditorManager*		mEditorManager;
private:
	void CustomDlgDelete(CustomDialog* Dlg);
};
