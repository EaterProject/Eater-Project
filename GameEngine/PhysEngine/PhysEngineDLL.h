#pragma once
#ifdef PHYS_INTERFACE
#define PHYS_ENGINEDLL __declspec(dllexport)
#else
#define PHYS_ENGINEDLL __declspec(dllimport)

	#ifdef _DEBUG
		#pragma comment(lib,"PhysEngine_x64d")
	#else
		#pragma comment(lib,"PhysEngine_x64r")
	#endif
#endif

//�ʿ�dll��
/*
	�׻��ʿ�
	PhysX_x64.dll
	PhysXCommon_*.dll
	PhysXFoundation_*.dll
*/

/*
* https://gameworksdocs.nvidia.com/PhysX/4.0/documentation/PhysXGuide/Manual/BuildingWithPhysX.html#header-files
	//Ư�� ��Ȳ��
	PhysXCooking_*.dll
	PhysXGPU_*.dll
*/

//�ֻ��� �������
