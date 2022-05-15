#pragma once
#include "ProfilerDLL.h"

/// 2022/04/02 22:26
/// SeoKyuHwang
///
/// # Profiler Header
///
/// 1. Console, VS_Output, Log_File 형태로 Profiling 지원
/// 2. Profile Output Type
///  - LOG_FILE : 해당 날짜 Log File에 이어서 Log 출력
///  - CONSOLE : Console 창에 해당 Log 출력
///  - VS_CODE : Visual Studio 출력 창에 Log 출력
/// 3. Debug 모드는 설정 없이 Profile 출력
/// 4. Release 모드 출력이 필요한경우 Header Include 하기 전 RELEASE_PROFILE Define 하면 출력
///	   #define RELEASE_PROFILE
///    #include "Profile.h"

#if defined(DEBUG) || defined(_DEBUG)
// Log Profiling
// # 해당 Log Message 출력.
// #Prameters
//  1. out		: PROFILE_OUTPUT Type
//  2. message	: Log Message
#define PROFILE_LOG(out, message, ...)				Log(out, 1, __FILE__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)

// COM Result Profiling
// # 해당 COM Result Message 출력.
// # COM 전용 HRESULT Macro + 추가 Message 출력.
// #Prameters
//  1. out		: PROFILE_OUTPUT Type
//  2. x		: Function Result
//  3. message	: Log Message
#define PROFILE_RESULT(out, x, message, ...)		Log(out, x, __FILE__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)

// Timer Start Profiling
// # 해당 Key Timer 측정 시작.
// # Timer Log 출력될 누적 Frame 설정.
// #Prameters
//  1. out		: PROFILE_OUTPUT Type
//  2. frame	: Output Timer Total Frame Rate
//  3. key		: Timer Key Name
#define PROFILE_TIMER_START(out, frame, key, ...)	TimerStart(out, __FILE__, __FUNCTION__, __LINE__, frame, key, ##__VA_ARGS__);

// - Timer Start Profiling
// # 해당 Key Timer 측정 종료.
// # 설정한 Total Frame에 도달하면 출력.
// #Prameters
//  1. key		: Timer Key Name
#define PROFILE_TIMER_END(key, ...)					TimerEnd(key, ##__VA_ARGS__);
#else
#define PROFILE_LOG(out, message, ...)
#define PROFILE_RESULT(out, x, message, ...)
#define PROFILE_TIMER_START(out, frame, key, ...)
#define PROFILE_TIMER_END(key, ...)					
#endif

#if defined(DEBUG) || defined(_DEBUG)
PROFILER_DLL void Log(PROFILE_OUTPUT outputType, long result, const char* file, const char* func, int&& line, const char* message, ...);
PROFILER_DLL void TimerStart(PROFILE_OUTPUT outputType, const char* file, const char* func, int&& line, int&& totalFrame, const char* timerKey, ...);
PROFILER_DLL void TimerEnd(const char* timerKey, ...);
#endif