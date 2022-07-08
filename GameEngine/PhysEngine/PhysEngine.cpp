#include "PhysEngine.h"
#include "PxPhysicsAPI.h"
#include <vector>
#include <ctype.h>
#include "PhysData.h"
#include "Factory.h"
#include "RaycastManager.h"
#include "BaseEventCallBack.h"


#include "PhysCollider.h"


#pragma comment(lib,"PhysX_64.lib")
#pragma comment(lib,"PhysXFoundation_64.lib")
#pragma comment(lib,"PhysXExtensions_static_64.lib")
#pragma comment(lib,"PhysXCooking_64.lib")
#pragma comment(lib,"PhysXCommon_64.lib")
#pragma comment(lib,"PhysXPvdSDK_static_64.lib")


//#pragma comment(lib,"PhysXVehicle_static_64.lib")				//차량
//#pragma comment(lib,"PhysXCharacterKinematic_static_64.lib")	//캐릭터

using namespace physx;
PhysEngine::PhysEngine()
{
	m_Allocator				= nullptr;
	m_ErrorCallback			= nullptr;
	m_TolerancesScale		= nullptr;
	m_CudaContextManager	= nullptr;

	m_Foundation	= nullptr;
	m_Physics		= nullptr;
	m_Dispatcher	= nullptr;
	m_Scene			= nullptr;
	m_Pvd			= nullptr;
	m_Cooking		= nullptr;

	m_Factory		= nullptr;
	m_RayManager	= nullptr;
	m_BaseEvent		= nullptr;
}

PhysEngine::~PhysEngine()
{

}

bool PhysEngine::Initialize(int ThreadCount, PhysSceneData* SceneData, bool Debug)
{
	/// <summary>
	/// https://gameworksdocs.nvidia.com/PhysX/4.0/documentation/PhysXGuide/Manual/Startup.html
	/// </summary>
	
	OnDebug = Debug;
	if (Debug == true)
	{
		Initialize_Debug(ThreadCount);
	}
	else
	{
		Initialize_Release(ThreadCount);
	}

	CreateScene(SceneData);
	
	//펙토리 생성
	m_Factory = new Factory();
	m_Factory->Initialize(m_Physics,m_Scene, m_Cooking);

	m_RayManager = new RaycastManager();
	m_RayManager->Initialize(m_Scene);

	return true;
}

void PhysEngine::Release()
{
	//생성의 역순으로 삭제
	m_Dispatcher->release();
	m_Physics->release();
	m_Foundation->release();
	m_CudaContextManager->release();

	delete m_Allocator;
	delete m_ErrorCallback;
	delete m_TolerancesScale;

	if (m_BaseEvent != nullptr)
	{
		delete m_BaseEvent;
	}

	//디버깅일때 삭제
	if (m_Pvd != nullptr)
	{
		PxCloseExtensions();
		PxPvdTransport* transport = m_Pvd->getTransport();
		m_Pvd->release();	m_Pvd = NULL;
		transport->release();
	}
}

void PhysEngine::Update(float m_time)
{
	if (Start == false)
	{
		m_Scene->simulate(0);
		m_Scene->fetchResults(true);
		Start = true;
	}
	else
	{
		m_Scene->simulate(m_time);
		m_Scene->fetchResults(true);
	}
}

PhysData* PhysEngine::Create_PhysData()
{
	return new PhysData();
}

void PhysEngine::Create_Actor(PhysData* data)
{
	m_Factory->CreateActoer(data);
}

void  PhysEngine::Update_Actor(PhysData* data)
{
	if (data == nullptr) { return; }

	if (data->isDinamic == true)
	{
		PxRigidDynamic* Dynamic = reinterpret_cast<PxRigidDynamic*>(data->ActorObj);
		if (Dynamic == nullptr) { return; }
		if (data->isPosition == true){UpdateDynamicPosition(Dynamic, data);}
		if (data->isForce == true){UpdateDynamicForce(Dynamic, data);}
		if (data->isVelocity == true){UpdateDynamicVelocity(Dynamic, data);}

		//받아온 Data를 Actor와 동기화시켜준다
		PxTransform Tr = Dynamic->getGlobalPose();
		data->WorldPosition.x = Tr.p.x;
		data->WorldPosition.y = Tr.p.y;
		data->WorldPosition.z = Tr.p.z;
		data->Rotation.x = Tr.q.x;
		data->Rotation.y = Tr.q.y;
		data->Rotation.z = Tr.q.z;
		data->Rotation.w = Tr.q.w;
	}
	else
	{
		PxRigidStatic*	Static = reinterpret_cast<PxRigidStatic*>(data->ActorObj);
		if (data->isPosition == true) { UpdateStaticPosition(Static, data); }
		if (Static == nullptr) { return; }

		PxTransform Tr = Static->getGlobalPose();
		data->WorldPosition.x = Tr.p.x;
		data->WorldPosition.y = Tr.p.y;
		data->WorldPosition.z = Tr.p.z;
		data->Rotation.x = Tr.q.x;
		data->Rotation.y = Tr.q.y;
		data->Rotation.z = Tr.q.z;
		data->Rotation.w = Tr.q.w;
	}

}

void PhysEngine::Delete_Actor(PhysData* data)
{
	//한개의 엑터 삭제
	if (data->ActorObj != nullptr)
	{
		if (data->mCollider != nullptr)
		{
			delete data->mCollider;
			data->mCollider = nullptr;
		}

		if (data->mMeterial != nullptr)
		{
			delete data->mMeterial;
			data->mMeterial = nullptr;
		}
		PxRigidStatic* rig = reinterpret_cast<PxRigidStatic*>(data->ActorObj);
		m_Scene->removeActor(*rig);

		rig->release();
		rig = nullptr;
	}

	data->ActorObj = nullptr;
	data->EaterObj = nullptr;

	delete data;
	data = nullptr;

}

bool PhysEngine::RayCast(PhysRayCast* ray)
{
	return m_RayManager->RayCast(ray);
}

bool PhysEngine::MultiRayCast(PhysRayCast* ray)
{
	return m_RayManager->RayCastMultiple(ray);
}

PxFilterFlags SampleSubmarineFilterShader
(
	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	if (PxFilterObjectIsTrigger(attributes1) == true || PxFilterObjectIsTrigger(attributes0) == true)
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}

	pairFlags = PxPairFlag::eCONTACT_DEFAULT;


	return PxFilterFlag::eDEFAULT;
}

bool PhysEngine::CreateScene(PhysSceneData* SceneData)
{
	//gpu 사용
	PxCudaContextManagerDesc cudaContextManagerDesc;
	m_CudaContextManager = PxCreateCudaContextManager(*m_Foundation, cudaContextManagerDesc, PxGetProfilerCallback());
	m_BaseEvent = new BaseEventCallBack();

	PxSceneDesc sceneDesc				= PxSceneDesc(m_Physics->getTolerancesScale());
	sceneDesc.gravity					= PxVec3(0.0f, -9.8f, 0.0f);
	sceneDesc.cpuDispatcher				= m_Dispatcher;
	sceneDesc.simulationEventCallback	= m_BaseEvent;
	sceneDesc.filterShader				= SampleSubmarineFilterShader;
	//sceneDesc.filterShader				= PxDefaultSimulationFilterShader;
	sceneDesc.cudaContextManager		= m_CudaContextManager;
	sceneDesc.broadPhaseType			= PxBroadPhaseType::eGPU;
	//sceneDesc.broadPhaseType			= PxBroadPhaseType::eMBP;
	
	sceneDesc.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;
	//sceneDesc.flags |= PxSceneFlag::eENABLE_CCD;
	sceneDesc.flags |= PxSceneFlag::eENABLE_PCM; 
	

	m_Scene = m_Physics->createScene(sceneDesc);

	if (OnDebug == true)
	{
		PxPvdSceneClient* pvdClient = m_Scene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS,	true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS,		true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES,	true);
		}
	}

	float size = 100;
	///Phys Engine 충돌될 범위를 지정해준다
	PxBroadPhaseRegion regions[4] =
	{
		{	PxBounds3(PxVec3(-size, -size, -size),  PxVec3(0, size,   0)),	reinterpret_cast<void*>(1) },
		{	PxBounds3(PxVec3(-size, -size,    0),  PxVec3(0, size, size)),	reinterpret_cast<void*>(2) },
		{	PxBounds3(PxVec3(0, -size, -size),	PxVec3(size, size,   0)),	reinterpret_cast<void*>(3) },
		{	PxBounds3(PxVec3(0, -size,    0),	PxVec3(size, size, size)),	reinterpret_cast<void*>(4) }
	};

	for (PxU32 i = 0; i < 4; i++)
	{
		m_Scene->addBroadPhaseRegion(regions[i]);
	}

	return false;
}

bool PhysEngine::Initialize_Release(int ThreadCount)
{
	CreateStart();

	m_Foundation	= PxCreateFoundation(PX_PHYSICS_VERSION, *m_Allocator, *m_ErrorCallback);
	m_Physics		= PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, *m_TolerancesScale);
	m_Dispatcher	= PxDefaultCpuDispatcherCreate(ThreadCount);
	m_Cooking		= PxCreateCooking(PX_PHYSICS_VERSION, *m_Foundation, PxTolerancesScale());

	if (m_Foundation	== nullptr) { return false; }
	if (m_Physics		== nullptr) { return false; }
	if (m_Dispatcher	== nullptr) { return false; }

	return true;
}

bool PhysEngine::Initialize_Debug(int ThreadCount)
{
	CreateStart();

	m_Foundation= PxCreateFoundation(PX_PHYSICS_VERSION, *m_Allocator, *m_ErrorCallback);
	if (m_Foundation == nullptr) { return false; }

	///PhysX visual Debugger 와 연결 준비를 한다(디버거 툴이 먼저 켜져있어야한다)
	m_Pvd		= PxCreatePvd(*m_Foundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	m_Pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
	
	///physX 생성
	m_Physics	= PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, PxTolerancesScale(), true, m_Pvd);
	if (m_Physics == nullptr) { return false; }
	PxInitExtensions(*m_Physics, m_Pvd);
	
	///Cooking 생성
	m_Cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_Foundation, PxTolerancesScale());

	///쓰레드 개수만큼 Phys 를 돌림
	m_Dispatcher = PxDefaultCpuDispatcherCreate(ThreadCount);
	if (m_Dispatcher == nullptr) { return false; }

	return true;
}

void PhysEngine::CreateStart()
{
	m_Allocator			= new PxDefaultAllocator();
	m_ErrorCallback		= new PxDefaultErrorCallback();
	m_TolerancesScale	= new PxTolerancesScale();
}

void PhysEngine::UpdateDynamicPosition(PxRigidDynamic* Dynamic, PhysData* Data)
{
	if (Data == nullptr) { return; }
	//위치값이 변경되었다면 변경된 값으로 업데이트
	PxQuat Q = PxQuat(Data->Rotation.x, Data->Rotation.y, Data->Rotation.z, Data->Rotation.w);
	PxVec3 P = PxVec3(Data->WorldPosition.x, Data->WorldPosition.y, Data->WorldPosition.z);
	Dynamic->setGlobalPose(PxTransform(P, Q));
	Data->isPosition = false;
}

void PhysEngine::UpdateDynamicForce(PxRigidDynamic* Dynamic, PhysData* Data)
{
	if (Data == nullptr) { return; }

	//포스값이 변경되었다면 변경된 값으로 업데이트
	PxVec3 Force = PxVec3(Data->Force.x, Data->Force.y, Data->Force.z);
	PxVec3 Pos = PxVec3(Data->WorldPosition.x, Data->WorldPosition.y, Data->WorldPosition.z);
	PxRigidBodyExt::addForceAtPos(*Dynamic, Force, Pos);
	Data->isForce = false;
}

void PhysEngine::UpdateDynamicVelocity(PxRigidDynamic* Dynamic, PhysData* Data)
{
	if (Data == nullptr) { return; }

	//속력값이 변경되었다면 변경된 값으로 업데이트
	PxVec3 Velocity = Dynamic->getLinearVelocity();
	PxVec3 Pox;
	if (Data->Velocity.y == 0.0f)
	{
		Pox = PxVec3(Data->Velocity.x, Data->Velocity.y, Data->Velocity.z);
	}
	else
	{
		Pox = PxVec3(Data->Velocity.x, Velocity.y, Data->Velocity.z);
	}
	//PxVec3 Pox = PxVec3(data->Velocity.x, data->Velocity.y, data->Velocity.z);
	Dynamic->setLinearVelocity(Pox);
	Data->isVelocity = false;
}

void PhysEngine::UpdateStaticPosition(PxRigidStatic* Static, PhysData* Data)
{
	if (Data == nullptr) { return; }

	PxQuat Q = PxQuat(Data->Rotation.x, Data->Rotation.y, Data->Rotation.z, Data->Rotation.w);
	PxVec3 P = PxVec3(Data->WorldPosition.x, Data->WorldPosition.y, Data->WorldPosition.z);
	Static->setGlobalPose(PxTransform(P, Q));
	Data->isPosition = false;
}


