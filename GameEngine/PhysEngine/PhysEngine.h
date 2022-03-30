#pragma once
#include "PhysEngineDLL.h"
#include <vector>
#include "PxSimulationEventCallback.h"
using namespace physx;

namespace physx
{
	class PxDefaultAllocator;
	class PxDefaultErrorCallback;
	class PxFoundation;
	class PxPhysics;
	class PxDefaultCpuDispatcher;
	class PxScene;
	class PxCooking;
	class PxPvd;
	class PxTolerancesScale;
	class PxCudaContextManager;
}

class PhysData;
class PhysRayCast;
class RaycastManager;
class Factory;
class BaseEventCallBack;
struct PhysSceneData;

class PhysEngine
{
public:
	PhysEngine();
	~PhysEngine();

	//초기화
	bool Initialize(int ThreadCount, PhysSceneData* SceneData, bool Debug);
	//씬 생성
	bool CreateScene(PhysSceneData* SceneData);
	//전체 삭제
	void Release();
	//씬 업데이트
	void Update(float m_time);

	//엑터 생성
	PhysData* Create_PhysData();
	void Create_Actor(PhysData* data);
	//엑터 업데이트
	void Update_Actor(PhysData* data);
	//엑터 삭제
	void Delete_Actor(PhysData* data);
	
	//한개의 Raycast충돌 여부를 반환
	bool RayCast(PhysRayCast* ray);
	
	bool MultiRayCast(PhysRayCast* ray);

	//virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs);
private:
	bool Initialize_Release(int ThreadCount);
	bool Initialize_Debug(int ThreadCount);
	void CreateStart();

	bool Start = false;

	physx::PxDefaultAllocator*		m_Allocator;
	physx::PxDefaultErrorCallback*	m_ErrorCallback;
	physx::PxTolerancesScale*		m_TolerancesScale;

	physx::PxCudaContextManager*	m_CudaContextManager;
	physx::PxCooking*				m_Cooking;
	physx::PxFoundation*			m_Foundation;
	physx::PxPhysics*				m_Physics;
	physx::PxDefaultCpuDispatcher*	m_Dispatcher;
	physx::PxScene*					m_Scene;
	physx::PxPvd*					m_Pvd;				//디버깅을 사용하기위해
	BaseEventCallBack*				m_BaseEvent;

	bool OnDebug = false;	//디버깅 사용여부

	Factory*		m_Factory;
	RaycastManager* m_RayManager;




	
};