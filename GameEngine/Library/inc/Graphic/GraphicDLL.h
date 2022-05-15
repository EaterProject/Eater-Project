#pragma once

#ifndef KH_STDCALL
	#define KH_STDCALL		__stdcall
#endif

#ifndef KH_CCALL
	#define KH_CCALL		__cdecl
#endif

#ifndef KH_VECTORCALL
	#define KH_VECTORCALL	__vectorcall
#endif

#ifdef GRAPHIC_EXPORT
	#define GRAPHIC_DLL		__declspec(dllexport)
#else
	#define GRAPHIC_DLL		__declspec(dllimport)		

	#ifdef _WIN64
		#ifdef _DEBUG
			#pragma comment(lib, "KHGraphic_x64d")		// 64bit + Debug
		#else
			#pragma comment(lib, "KHGraphic_x64r")		// 64bit
		#endif
	#else
		#ifdef _DEBUG
			#pragma comment(lib, "KHGraphic_x86d")		// 32bit + Debug
		#else
			#pragma comment(lib, "KHGraphic_x86r")		// 32bit
		#endif
	#endif
#endif
 