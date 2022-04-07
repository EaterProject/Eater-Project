#pragma once

/// 2022/04/07 22:26
/// SeoKyuHwang
///
/// # GlobalDataManager Class
///
/// - Renderer 측에 Global Data 공유를 위한 Manager
/// 

class GlobalData;

class GlobalDataManager
{
public:
	static void Initialize();
	static void Release();
	static void Update(float dTime);

public:
	static GlobalData* g_GlobalData;
};

