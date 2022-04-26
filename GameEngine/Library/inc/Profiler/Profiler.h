#pragma once
#include "ProfilerDLL.h"

/// 2022/04/02 22:26
/// SeoKyuHwang
///
/// # Profiler Header
///
/// 1. Console, VS_Output, Log_File ���·� Profiling ����
/// 2. Profile Output Type
///  - LOG_FILE : �ش� ��¥ Log File�� �̾ Log ���
///  - CONSOLE : Console â�� �ش� Log ���
///  - VS_CODE : Visual Studio ��� â�� Log ���
/// 3. Debug ���� ���� ���� Profile ���
/// 4. Release ��� ����� �ʿ��Ѱ�� Header Include �ϱ� �� RELEASE_PROFILE Define �ϸ� ���
///	   #define RELEASE_PROFILE
///    #include "Profile.h"

#if defined(DEBUG) || defined(_DEBUG)
// Log Profiling
// # �ش� Log Message ���.
// #Prameters
//  1. out		: PROFILE_OUTPUT Type
//  2. message	: Log Message
#define PROFILE_LOG(out, message, ...)				Log(out, 1, __FILE__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)

// COM Result Profiling
// # �ش� COM Result Message ���.
// # COM ���� HRESULT Macro + �߰� Message ���.
// #Prameters
//  1. out		: PROFILE_OUTPUT Type
//  2. x		: Function Result
//  3. message	: Log Message
#define PROFILE_RESULT(out, x, message, ...)		Log(out, x, __FILE__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)

// Timer Start Profiling
// # �ش� Key Timer ���� ����.
// # Timer Log ��µ� ���� Frame ����.
// #Prameters
//  1. out		: PROFILE_OUTPUT Type
//  2. frame	: Output Timer Total Frame Rate
//  3. key		: Timer Key Name
#define PROFILE_TIMER_START(out, frame, key, ...)	TimerStart(out, __FILE__, __FUNCTION__, __LINE__, frame, key, ##__VA_ARGS__);

// - Timer Start Profiling
// # �ش� Key Timer ���� ����.
// # ������ Total Frame�� �����ϸ� ���.
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