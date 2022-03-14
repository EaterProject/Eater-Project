#pragma once

#include "PhysEngineDLL.h"
#include "PhysData.h"


///ENGINE 생성
//1. 사용할 쓰레드 개수
//2. 씬 데이터 nullptr을 넣으면 디폴트 값으로 씬을 실행
//3. PhysX에서 제공하는 Debug툴을 사용할것인지
extern "C" PHYS_ENGINEDLL void PhysX_Initialize(int ThreadCount,PhysSceneData* SceneData, bool OnDebug = false);

///ENGINE 전체 삭제
extern "C" PHYS_ENGINEDLL void PhysX_Release();

///ENGINE 업데이트
//1.프레임단위 시간 (델타 타임)
extern "C" PHYS_ENGINEDLL void PhysX_Update(float m_time);	//실행

///물리 객체 생성
//1.생성할 데이터 정보를 담은 PhysData 구조체
extern "C"  PHYS_ENGINEDLL void PhysX_Create_Actor(PhysData* data);

///물리 객체 업데이트
//담아올 PhysData를 포인터로 넣어줌
extern "C"	PHYS_ENGINEDLL void PhysX_Update_Actor(PhysData* data);

///물리 객체 삭제
//삭제할 physData를 포함한 PhysX내부에 객체도 삭제
extern "C"	PHYS_ENGINEDLL void PhysX_Delete_Actor(PhysData* data);

///RayCast
//충돌 여부 반환
extern "C"	PHYS_ENGINEDLL bool PhysX_RayCast(PhysRayCast* ray);
