#pragma once
#include "ConstantBufferDefine.h"

/// 2022/06/25 16:51
/// SeoKyuHwang
///
/// # ConstantBufferManager Class
///
/// - Shader에 바인딩되는 모든 Constant Buffer를 관리하기 위해 만든 클래스
/// - RenderPass 한정 전역적으로 사용

class GlobalData;
class RenderOption;

class ConstantBufferManager
{
public:
	friend class RenderPassBase;

public:
	static void Update();

private:
	static GlobalData* g_GlobalData;
	static RenderOption* g_RenderOption;

public:
	static CB_Light		LIGHT_BUFFER;
	static CB_LightSub	LIGHT_SUB_BUFFER;
};

