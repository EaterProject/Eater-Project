#pragma once
#include "HashBase.h"

#define UNORDERED_ACCESS_VIEW(ClassName) CREATE_HASH_CLASS(ClassName, RESOURCE_TYPE::UAV) RESOURCE_PUSH(ClassName, RESOURCE_TYPE::UAV)

/// <summary>
/// UnorderedAccessView Resource Struct
/// </summary>
/// 
/// - Shader ������ UnorderedAccessView Resource�� 1:1 �����ϴ� Struct
/// - Shader���� ������ �������� ��ȣ�� ���۰� ��ġ�ϴ� �Ѱ��� UnorderedAccessView
/// - D3DCompiler���� ���� Shader���� UnorderedAccessView�� ������ �ξ ������� ������
///   Register Slot�� �ø��� �ʴ´�..
/// 
/// - ������
///  1) ���� struct�� �̸��� ���� ������ Shader ������ UnorderedAccessView�� ��ġ�ؾ��Ѵ� (Struct Name�� ���� UnorderedAccessView�� ã�� ����)

///////////////////////////////////////////////////////////////////////////////////////////
// Blur UnorderedAccessView Resource
///////////////////////////////////////////////////////////////////////////////////////////

UNORDERED_ACCESS_VIEW(gOutputUAV)

///////////////////////////////////////////////////////////////////////////////////////////
// Hierachical Z-Map Occlusion Culling UnorderedAccessView Resource
///////////////////////////////////////////////////////////////////////////////////////////

UNORDERED_ACCESS_VIEW(gCullingBuffer)

/// Resource Hash ���� ���� Define
RESOURCE_DEFINE(DEFINE_UAV)