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

extern "C" EATER_ENGINEDLL void EngineInitialize(HWND _g_hWnd,bool ON_DEBUG);	//GameEngine 시작
extern "C" EATER_ENGINEDLL void EndEngine();						//GameEngine 종료
extern "C" EATER_ENGINEDLL void UpdateEngine();						//GameEngine 업데이트
extern "C" EATER_ENGINEDLL void OnReSize(int X,int Y);				//윈도우 사이즈 재조정	

///게임 오브젝트 생성
extern "C" EATER_ENGINEDLL GameObject*	Instance(std::string ObjName= "GameObject");		//오브젝트 생성
extern "C" EATER_ENGINEDLL GameObject*	Instance_Terrain(std::string ObjName= "GameObject");	//터레인 오브젝트 생성
extern "C" EATER_ENGINEDLL GameObject*	Instance_Particle(std::string ObjName= "GameObject");//파티클 오브젝트 생성
extern "C" EATER_ENGINEDLL GameObject*	Instance_Camera(std::string ObjName= "GameObject");//오브젝트 생성
extern "C" EATER_ENGINEDLL GameObject*	InstanceLight(std::string ObjName= "Light", LIGHT_TYPE type = LIGHT_TYPE::DIRECTION_LIGHT);//오브젝트 생성
extern "C" EATER_ENGINEDLL GameObject*	InstanceUI(std::string ObjName= "UI");//오브젝트 생성

///게임 오브젝트 찾기
extern "C" EATER_ENGINEDLL GameObject*	FindGameObjectTag(std::string TagName);
extern "C" EATER_ENGINEDLL	void		FindGameObjectTags(std::string TagName,std::vector<GameObject*>*ObjectList);
extern "C" EATER_ENGINEDLL	int			FindTagNumber(std::string TagName);

extern "C" EATER_ENGINEDLL GameObject*	FindGameObjectName(std::string ObjectName);
extern "C" EATER_ENGINEDLL void			Destroy(GameObject* obj);//오브젝트 삭제
extern "C" EATER_ENGINEDLL void			DestroyAll();//오브젝트 삭제

///스크린 생성하기
template<typename T>
void CreateScene(std::string Name);
extern "C" EATER_ENGINEDLL void ChoiceScene(std::string name);					//스크린 선택
EATER_ENGINEDLL void CreateSceneSub(Scene* mSceneTemp,std::string SceneName);	//스크린 생성

///로드 관련
extern "C" EATER_ENGINEDLL void LoadTerrainMesh(std::string mMeshPath, std::string maskName1, std::string maskName2, UINT parsingMode = 0);	//매쉬 로드
extern "C" EATER_ENGINEDLL void Load(std::string mPath,UINT Mode = 0);
extern "C" EATER_ENGINEDLL int	LoadAssetsCount();

///베이킹 관련
extern "C" EATER_ENGINEDLL void BakeSkyLightMap(std::string mPath, bool hdri);
extern "C" EATER_ENGINEDLL void BakeConvertSkyCubeMap(std::string mPath, float angle, float threshold, bool hdri);
extern "C" EATER_ENGINEDLL void BakeConvertSkyLightMap(std::string mPath, float angle, float threshold, bool hdri);
extern "C" EATER_ENGINEDLL void BakeAnimation();

///저장 관련
extern "C" EATER_ENGINEDLL void SaveConvertSkyLightMap(std::string mPath, std::string SaveName);
extern "C" EATER_ENGINEDLL void SaveConvertSkyCubeMap(std::string mPath, std::string SaveName);

///맵 설정
extern "C" EATER_ENGINEDLL void SetSkyCube(std::string mPath);
extern "C" EATER_ENGINEDLL void SetSkyLight(std::string mPath);
extern "C" EATER_ENGINEDLL int GetLoadMeshCount();		//로드된 메쉬 카운터
extern "C" EATER_ENGINEDLL int GetLoadTextureCount();	//로드된 텍스쳐 카운터
extern "C" EATER_ENGINEDLL int GetLoadAnimationCount();	//로드된 애니메이션 카운터
extern "C" EATER_ENGINEDLL int GetLoadMaterialCount();	//로드된 메테리얼 카운터
extern "C" EATER_ENGINEDLL int GetLoadBufferCount();	//로드된 버퍼 카운터

extern "C" EATER_ENGINEDLL void AddOccluder(std::string mMeshName);	//충돌 전용 매쉬 설정

///키입력
extern "C" EATER_ENGINEDLL bool  GetKeyDown(byte number);		//키다운
extern "C" EATER_ENGINEDLL bool  GetKeyUp(byte number);			//키업
extern "C" EATER_ENGINEDLL bool  GetKey(byte number);			//키누르고있을때
extern "C" EATER_ENGINEDLL bool  GetTogle(byte number);			//키 토글
extern "C" EATER_ENGINEDLL int GetMousePosX();				//마우스 위치 X
extern "C" EATER_ENGINEDLL int GetMousePosY();				//마우스 위치 Y
extern "C" EATER_ENGINEDLL void  SetMousePos(float x,float y);	//마우스 위치 Y
extern "C" EATER_ENGINEDLL void  SetMousePosCenter();			//마우스 위치 Y
extern "C" EATER_ENGINEDLL void ShowMouseCursor(bool Cursor);	//마우스 커서 보이기여부
extern "C" EATER_ENGINEDLL void MouseCursorClip(bool Clip);		//마우스 커서 보이기여부

///윈도우 관련
extern "C" EATER_ENGINEDLL void WindowFocus(bool focus);		//현재 윈도우 포커싱 상태
extern "C" EATER_ENGINEDLL GameObject* Picking(int x, int y);	//현재 윈도우 클릭 지점

///카메라 관련
extern "C" EATER_ENGINEDLL GameObject * GetMainCamera();	//현재 메인 카메라를 가져온다
extern "C" EATER_ENGINEDLL GameObject * GetDebugCamera();	//현재 메인 카메라를 가져온다

///시간 관련
extern "C" EATER_ENGINEDLL float GetDeltaTime();

///물리 관련
extern "C" EATER_ENGINEDLL bool RayCast(PhysRayCast* ray);

/// 디버그
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

///네트워크 관련
//엔진쪽에서 통신한 모든데이터를 여기에서 받은 매니저로 클라와 통신한다
extern "C" EATER_ENGINEDLL void Network_SetManager(NetworkManagerComponent * Manager);
//클라이언트 에서 메세지를 보낼때 이것을 사용
extern "C" EATER_ENGINEDLL void Network_Send(flatbuffers::FlatBufferBuilder * Builder,int Type);
//
extern "C" EATER_ENGINEDLL void Network_Connect(int ServerPort, std::string  Local_Connect_IP);
//로딩이 완료되었을때 보내는 메세지
extern "C" EATER_ENGINEDLL void Network_Loading_complete(unsigned int Number);


template<typename T>
inline void CreateScene(std::string Name)
{
	T* temp = new T();
	CreateSceneSub(temp, Name);
}