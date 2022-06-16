#pragma once

#include "PhysEngineDLL.h"
#include "PhysData.h"
#include "PhysScene.h"
#include "PhysMaterial.h"
#include "PhysCollider.h"
#include "PhysRay.h"


//#include "Phys_Collider_Box.h"
//#include "Phys_Collider_Capsule.h"
//#include "Phys_Collider_Sphere.h"
#include "Phys_Collider_Triangle.h"


///ENGINE ����
//1. ����� ������ ����
//2. �� ������ nullptr�� ������ ����Ʈ ������ ���� ����
//3. PhysX���� �����ϴ� Debug���� ����Ұ�����
extern "C" PHYS_ENGINEDLL void PhysX_Initialize(int ThreadCount, bool OnDebug = false);

///ENGINE ��ü ����
extern "C" PHYS_ENGINEDLL void PhysX_Release();

///ENGINE ������Ʈ
//1.�����Ӵ��� �ð� (��Ÿ Ÿ��)
extern "C" PHYS_ENGINEDLL void PhysX_Update(float m_time);	//����

///���� ��ü ����
//1.������ ������ ������ ���� PhysData ����ü
extern "C"  PHYS_ENGINEDLL void PhysX_Create_Actor(PhysData* data);
extern "C"  PHYS_ENGINEDLL PhysData* PhysX_Create_Data();

///���� ��ü ������Ʈ
//��ƿ� PhysData�� �����ͷ� �־���
extern "C"	PHYS_ENGINEDLL void PhysX_Update_Actor(PhysData* data);

///���� ��ü ����
//������ physData�� ������ PhysX���ο� ��ü�� ����
extern "C"	PHYS_ENGINEDLL void PhysX_Delete_Actor(PhysData* data);

///RayCast
//�浹 ���� ��ȯ
extern "C"	PHYS_ENGINEDLL bool PhysX_RayCast(PhysRayCast* ray);

extern "C" PHYS_ENGINEDLL Phys_Collider_Triangle* CreateCollider_Triangle();

