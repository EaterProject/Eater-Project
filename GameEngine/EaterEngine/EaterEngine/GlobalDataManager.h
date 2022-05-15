#pragma once

/// 2022/04/07 22:26
/// SeoKyuHwang
///
/// # GlobalDataManager Class
///
/// - Renderer ���� Global Data ������ ���� Manager
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

