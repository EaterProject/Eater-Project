#pragma once

#include <functional>
#include <string>
#include <windows.h>
#include "EaterEngineDLL.h"
#include "TypeOptionHeader.h"

namespace flatbuffers 
{
	class FlatBufferBuilder;
}

class GameObject;
class Scene;
class PhysRayCast;
class NetworkManagerComponent;
class LoadMeshData;
class ModelData;
class RenderOption;

extern "C" EATER_ENGINEDLL void EngineInitialize(HWND _g_hWnd,bool ON_DEBUG);	//GameEngine ����
extern "C" EATER_ENGINEDLL void EndEngine();						//GameEngine ����
extern "C" EATER_ENGINEDLL void UpdateEngine();						//GameEngine ������Ʈ
extern "C" EATER_ENGINEDLL void OnReSize(int X,int Y);				//������ ������ ������	

///���� ������Ʈ ����
extern "C" EATER_ENGINEDLL GameObject*	Instance(std::string ObjName= "GameObject");		//������Ʈ ����
extern "C" EATER_ENGINEDLL GameObject*	Instance_Terrain(std::string ObjName= "GameObject");	//�ͷ��� ������Ʈ ����
extern "C" EATER_ENGINEDLL GameObject*	Instance_Particle(std::string ObjName= "GameObject");//��ƼŬ ������Ʈ ����
extern "C" EATER_ENGINEDLL GameObject*	Instance_Camera(std::string ObjName= "GameObject");//������Ʈ ����
extern "C" EATER_ENGINEDLL GameObject*	InstanceLight(std::string ObjName= "Light", LIGHT_TYPE type = LIGHT_TYPE::DIRECTION_LIGHT);//������Ʈ ����
extern "C" EATER_ENGINEDLL GameObject*	InstanceUI(std::string ObjName= "UI");//������Ʈ ����

///���� ������Ʈ ã��
extern "C" EATER_ENGINEDLL GameObject*	FindGameObjectTag(std::string TagName);
extern "C" EATER_ENGINEDLL	void		FindGameObjectTags(std::string TagName,std::vector<GameObject*>*ObjectList);
extern "C" EATER_ENGINEDLL	int			FindTagNumber(std::string TagName);

extern "C" EATER_ENGINEDLL GameObject*	FindGameObjectName(std::string ObjectName);
extern "C" EATER_ENGINEDLL void			Destroy(GameObject* obj);//������Ʈ ����
extern "C" EATER_ENGINEDLL void			DestroyAll();//������Ʈ ����

///��ũ�� �����ϱ�
template<typename T>
void CreateScene(std::string Name);
extern "C" EATER_ENGINEDLL void ChoiceScene(std::string name);					//��ũ�� ����
EATER_ENGINEDLL void CreateSceneSub(Scene* mSceneTemp,std::string SceneName);	//��ũ�� ����

///�ε� ����
extern "C" EATER_ENGINEDLL void LoadTerrainMesh(std::string mMeshPath, std::string maskName1, std::string maskName2, UINT parsingMode = 0);	//�Ž� �ε�
extern "C" EATER_ENGINEDLL void Load(std::string mPath,UINT Mode = 0);
extern "C" EATER_ENGINEDLL int	LoadAssetsCount();

///����ŷ ����
extern "C" EATER_ENGINEDLL void BakeSkyLightMap(std::string mPath, bool hdri);
extern "C" EATER_ENGINEDLL void BakeConvertSkyCubeMap(std::string mPath, float angle, float threshold, bool hdri);
extern "C" EATER_ENGINEDLL void BakeConvertSkyLightMap(std::string mPath, float angle, float threshold, bool hdri);
extern "C" EATER_ENGINEDLL void BakeAnimation();

///���� ����
extern "C" EATER_ENGINEDLL void SaveConvertSkyLightMap(std::string mPath, std::string SaveName);
extern "C" EATER_ENGINEDLL void SaveConvertSkyCubeMap(std::string mPath, std::string SaveName);

///�� ����
extern "C" EATER_ENGINEDLL void SetSkyCube(std::string mPath);
extern "C" EATER_ENGINEDLL void SetSkyLight(std::string mPath);
extern "C" EATER_ENGINEDLL int GetLoadMeshCount();		//�ε�� �޽� ī����
extern "C" EATER_ENGINEDLL int GetLoadTextureCount();	//�ε�� �ؽ��� ī����
extern "C" EATER_ENGINEDLL int GetLoadAnimationCount();	//�ε�� �ִϸ��̼� ī����
extern "C" EATER_ENGINEDLL int GetLoadMaterialCount();	//�ε�� ���׸��� ī����
extern "C" EATER_ENGINEDLL int GetLoadBufferCount();	//�ε�� ���� ī����

extern "C" EATER_ENGINEDLL void AddOccluder(std::string mMeshName);	//�浹 ���� �Ž� ����

///Ű�Է�
extern "C" EATER_ENGINEDLL bool  GetKeyDown(byte number);		//Ű�ٿ�
extern "C" EATER_ENGINEDLL bool  GetKeyUp(byte number);			//Ű��
extern "C" EATER_ENGINEDLL bool  GetKey(byte number);			//Ű������������
extern "C" EATER_ENGINEDLL bool  GetTogle(byte number);			//Ű ���
extern "C" EATER_ENGINEDLL int GetMousePosX();				//���콺 ��ġ X
extern "C" EATER_ENGINEDLL int GetMousePosY();				//���콺 ��ġ Y
extern "C" EATER_ENGINEDLL void  SetMousePos(float x,float y);	//���콺 ��ġ Y
extern "C" EATER_ENGINEDLL void  SetMousePosCenter();			//���콺 ��ġ Y
extern "C" EATER_ENGINEDLL void ShowMouseCursor(bool Cursor);	//���콺 Ŀ�� ���̱⿩��
extern "C" EATER_ENGINEDLL void MouseCursorClip(bool Clip);		//���콺 Ŀ�� ���̱⿩��

///������ ����
extern "C" EATER_ENGINEDLL void WindowFocus(bool focus);		//���� ������ ��Ŀ�� ����
extern "C" EATER_ENGINEDLL GameObject* Picking(int x, int y);	//���� ������ Ŭ�� ����

///ī�޶� ����
extern "C" EATER_ENGINEDLL GameObject * GetMainCamera();	//���� ���� ī�޶� �����´�
extern "C" EATER_ENGINEDLL GameObject * GetDebugCamera();	//���� ���� ī�޶� �����´�

///�ð� ����
extern "C" EATER_ENGINEDLL float GetDeltaTime();

///���� ����
extern "C" EATER_ENGINEDLL bool RayCast(PhysRayCast* ray);

/// �����
EATER_ENGINEDLL void RenderSetting();
EATER_ENGINEDLL RenderOption* GetRenderOptionData();
EATER_ENGINEDLL void DebugDrawLine(Vector3 start, Vector3 end, Vector3 color);
EATER_ENGINEDLL void DebugDrawLine(Vector3 start, Vector3 dir, float distance, Vector3 color);
EATER_ENGINEDLL void DebugDrawCircle(float scale, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 rot, DirectX::SimpleMath::Vector3 color);
EATER_ENGINEDLL void DebugDrawCircle(float scale, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Quaternion rot, DirectX::SimpleMath::Vector3 color);
EATER_ENGINEDLL void DebugDrawBox(DirectX::SimpleMath::Vector3 scale, DirectX::SimpleMath::Vector3 rot, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 color);
EATER_ENGINEDLL void DebugDrawBox(DirectX::SimpleMath::Vector3 scale, DirectX::SimpleMath::Quaternion rot, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 color);
EATER_ENGINEDLL void DebugDrawSphere(float scale, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 color);
EATER_ENGINEDLL void DebugPrint(const char* message, ...);

///��Ʈ��ũ ����
//�����ʿ��� ����� ��絥���͸� ���⿡�� ���� �Ŵ����� Ŭ��� ����Ѵ�
extern "C" EATER_ENGINEDLL void Network_SetManager(NetworkManagerComponent * Manager);
//Ŭ���̾�Ʈ ���� �޼����� ������ �̰��� ���
extern "C" EATER_ENGINEDLL void Network_Send(flatbuffers::FlatBufferBuilder * Builder,int Type);
//
extern "C" EATER_ENGINEDLL void Network_Connect(int ServerPort, std::string  Local_Connect_IP);
//�ε��� �Ϸ�Ǿ����� ������ �޼���
extern "C" EATER_ENGINEDLL void Network_Loading_complete(unsigned int Number);


template<typename T>
inline void CreateScene(std::string Name)
{
	T* temp = new T();
	CreateSceneSub(temp, Name);
}