#pragma once
#include "ConstantBufferDefine.h"

/// 2022/06/25 16:51
/// SeoKyuHwang
///
/// # ConstantBufferManager Class
///
/// - Shader�� ���ε��Ǵ� ��� Constant Buffer�� �����ϱ� ���� ���� Ŭ����
/// - RenderPass ���� ���������� ���

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

