#pragma once
#include <windows.h>
#include <string>
#include "EaterEngineDLL.h"
#include "TypeOptionHeader.h"

/// <summary>
/// ��ü ���� ������ �Ѱ��ϴ� ���ӿ��� Ŭ����
/// Ŀ�ٶ� ���ӿ��� Ʋ
/// </summary>

namespace flatbuffers
{
	class FlatBufferBuilder;
}


//���� �Ŵ�����
class LoadManager;
class ObjectManager;
class SceneManager;
class KeyinputManager;
class GraphicEngineManager;
class TimeManager;
class MaterialManager;
class LightManager;
class PhysManager;
class NetworkManager;
class Scene;
class NetworkManagerComponent;
class NavigationManager;
//���� ������Ʈ
class GameObject;
class ModelData;
class Material;
class RenderOption;

class GameEngine
{
private:
	CRITICAL_SECTION g_CS;


public:
	GameEngine();
	~GameEngine();
	
public:
	///Ŭ���̾�Ʈ ���� ���� �Լ���
	///���� ���� ���� �Լ���
	void Initialize(HWND gHwnd,bool mConsoleDebug = true);	//�ʱ�ȭ
	void Start();											//�߰� �۾�
	void Update();											//���� ����
	void Finish();											//���� ����
	void OnResize(int Change_Width, int Change_Height);		//��������
	static int WinSizeWidth;	//������ ������ ����
	static int WinSizeHeight;	//������ ������ ����
public:
	///������Ʈ ����(������Ʈ �Ŵ���)
	GameObject* Instance(std::string ObjName = "GameObject");			//obj ���� (�� ���ӿ�����Ʈ)
	GameObject* InstanceTerrain(std::string ObjName = "Terrain");
	GameObject* InstanceParticle(std::string ObjName = "Particle");
	GameObject* InstanceCamera(std::string ObjName = "Camera");
	GameObject* InstanceLight(std::string ObjName = "Light", LIGHT_TYPE type = LIGHT_TYPE::POINT_LIGHT);

	Material* InstanceMaterial(std::string matName = "Material");

	GameObject* FindGameObjectTag(std::string& TagName);
	void FindGameObjectTags(std::string& TagName,std::vector<GameObject*>* ObjectList);
	int	 FindTagNumber(std::string& TagName);
	GameObject* FindGameObjectName(std::string& ObjName);

	void Destroy(GameObject* obj);		//obj ����
	void DestroyAll();
public:
	///��ũ�� ����
	void PushScene(Scene* mScene, std::string name);	//�� ����		
	void ChoiceScene(std::string name);					//�� ����
public:
	///�ε� ���� 
	void Load(std::string& Path, UINT MODE);
	void LoadTerrain(std::string mMeshName, std::string mMaskName1, std::string mMaskName2, UINT parsingMode);

	int  LoadMeshCount();
	int  LoadTextureCount();
	int  LoadAnimationCount();
	int	 LoadBufferCount();
	int  LoadMaterialCount();
	ModelData* GetLoadMeshData(std::string& Path);
public:
	///����ũ ����
	void BakeSkyLightMap(std::string& Path, bool hdri);
	void BakeAnimation();
	
	void BakeConvertSkyLightMap(std::string& Path, float angle, float threshold, bool hdri);
	void BakeConvertSkyCubeMap(std::string& Path, float angle, float threshold, bool hdri);

	///���� ����
	void SaveConvertCubeMap(std::string& Path);

public:
	///�ؽ��� ���� ����
	void SetSkyCube(std::string& Path);
	void SetSkyLight(std::string& Path);

	///�浹üũ ����
	void AddOccluder(std::string mMeshName);
public:
	///Ű���� 
	bool  GetKeyDown(byte number);	//Ű�ٿ�
	bool  GetKeyUp(byte number);	//Ű��
	bool  GetKey(byte number);		//Ű������������
	bool  GetTogle(byte number);	//Ű on/off
	float GetMousePosX();			//���콺 ��ġ X
	float GetMousePosY();			//���콺 ��ġ Y
	void  SetMousePos(float x,float y);
	void  SetMousePosCenter();
	void  ShowMouseCursor(bool Show);
	void  MouseCursorClip(bool Clip);

	///������ ����
	void SetFocus(bool focus);
	void* Picking(int x, int y);
public:
	///ī�޶� ����
	GameObject* GetMainCamera();
public:
	///�ð� ����
	float GetdeltaTime();
public:
	///��Ʈ��ũ ����
	void SetNetworkManager(NetworkManagerComponent* Manager);
	void NETWORK_SEND(flatbuffers::FlatBufferBuilder* Builder, int Type);
	void NETWORK_LOADING_COMPLETE(unsigned int Number);
	void NETWORK_CONNECT(int ServerPort, std::string  Local_Connect_IP);
public:
	///Scene Setting
	RenderOption* GetRenderOptionData();
	void RenderSetting();
private:
	GameObject* CreateInstance();
	Material* CreateMaterial();
	void CreateObject();			//�⺻ ���� ������Ʈ
	void RenderOptionCheck();

private:
	///���� �Ŵ�����
	ObjectManager*			mObjectManager;		// ������Ʈ ���� �Ŵ���
	LoadManager*			mLoadManager;		// �ε� ���� �Ŵ���
	SceneManager*			mSceneManager;		// ��ũ�� ���� �Ŵ���
	KeyinputManager*		mKeyManager;		// Ű��ǲ ���� �Ŵ���
	GraphicEngineManager*	mGraphicManager;	// �׷��ȿ��� ���� �Ŵ���
	TimeManager*			mTimeManager;		// �ð� ���� Ÿ�ӸŴ���
	LightManager*			mLightManager;		// ����Ʈ ���� �Ŵ���
	PhysManager*			mPhysManager;		// ���� ���� �Ŵ���
	NetworkManager*			mNetworkManager;	// ��Ʈ��ũ �Ŵ���
	//NavigationManager*		mNavigationManager;	// �׺���̼� �Ŵ���
private:
	///Ŭ���ʿ��� �޾ƾ� �ϴ� ������
	HWND mHwnd; //�ڵ�
	bool ConsoleDebug;
private:
	RenderOption* mRenderOption;
};

