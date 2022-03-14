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


//필요dll들
/*
	항상필요
	PhysX_x64.dll
	PhysXCommon_*.dll
	PhysXFoundation_*.dll
*/

/*
* https://gameworksdocs.nvidia.com/PhysX/4.0/documentation/PhysXGuide/Manual/BuildingWithPhysX.html#header-files
	//특수 상황만
	PhysXCooking_*.dll
	PhysXGPU_*.dll
*/

//최상위 헤더파일
