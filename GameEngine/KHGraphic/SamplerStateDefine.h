#pragma once
#include "HashBase.h"

#define SAMPLER_STATE(ClassName) CREATE_HASH_CLASS(ClassName, RESOURCE_TYPE::SS) RESOURCE_PUSH(ClassName, RESOURCE_TYPE::SS)

/// <summary>
/// SamplerState Resource Struct
/// </summary>
/// 
/// - Shader ������ SamplerState Resource�� 1:1 �����ϴ� Struct
/// - Shader���� ������ �������� ��ȣ�� ���۰� ��ġ�ϴ� �Ѱ��� SamplerState
/// - D3DCompiler���� ���� Shader���� SamplerState�� ������ �ξ ������� ������
///   Register Slot�� �ø��� �ʴ´�..
/// 
/// - ������
///  1) ���� struct�� �̸��� ���� ������ Shader ������ SamplerState�� ��ġ�ؾ��Ѵ� (Struct Name�� ���� SamplerState�� ã�� ����)

///////////////////////////////////////////////////////////////////////////////////////////
// Global SamplerState Resource
///////////////////////////////////////////////////////////////////////////////////////////
SAMPLER_STATE(gSamWrapLinear)
SAMPLER_STATE(gSamWrapLinearPoint)
SAMPLER_STATE(gSamWrapAnisotropic)
SAMPLER_STATE(gSamClampAnisotropic)
SAMPLER_STATE(gSamClampLinear)
SAMPLER_STATE(gSamClampPoint)
SAMPLER_STATE(gSamBorderLinear)
SAMPLER_STATE(gSamBorderLinearPoint)
SAMPLER_STATE(gSamBorderComparisonLinearPoint)
SAMPLER_STATE(gSamMirrorLinear)

/// Resource Hash ���� ���� Define
RESOURCE_DEFINE(DEFINE_SS)