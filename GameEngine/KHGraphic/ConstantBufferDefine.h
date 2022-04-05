#pragma once
#include "SimpleMath.h"
#include "HashBase.h"
#include "LightHelper.h"
#include "EngineData.h"

#define DIRECTION_LIGHT_COUNT	1
#define POINT_LIGHT_COUNT		5
#define SPOT_LIGHT_COUNT		5
#define GAUSSIAN_RADIUS			7

#define SHADER_CONSTANT_BUFFER(ClassName) CREATE_HASH_CLASS(ClassName, RESOURCE_TYPE::CB) RESOURCE_PUSH(ClassName, RESOURCE_TYPE::CB)

/// <summary>
/// ConstantBuffer Resource Struct
/// </summary>
/// 
/// - Shader 내부의 ConstantBuffer Resource와 1:1 대응하는 Struct
/// - Shader에서 설정한 레지스터 번호와 버퍼가 일치하는 한개의 Constant Buffer
/// - D3DCompiler에서 현재 Shader에서 ConstantBuffer을 선언해 두어도 사용하지 않으면
///   Register Slot에 올리지 않는다..
/// 
/// - 주의점
///  1) 현재 struct의 이름과 변수 순서는 Shader 내부의 ConstantBuffer와 일치해야한다 (Struct Name을 통해 Constant Buffer를 찾기 때문)

///////////////////////////////////////////////////////////////////////////////////////////
// Deferred Constant Buffer
///////////////////////////////////////////////////////////////////////////////////////////

SHADER_CONSTANT_BUFFER(cbStaticMesh)
struct CB_StaticMesh : public cbStaticMesh
{
	DirectX::SimpleMath::Matrix gWorld;
	DirectX::SimpleMath::Matrix gInvWorld;
	DirectX::SimpleMath::Matrix gView;
	DirectX::SimpleMath::Matrix gProj;
	DirectX::SimpleMath::Matrix gTexTransform;
};

SHADER_CONSTANT_BUFFER(cbInstanceStaticMesh)
struct CB_InstanceStaticMesh : public cbInstanceStaticMesh
{
	DirectX::SimpleMath::Matrix gView;
	DirectX::SimpleMath::Matrix gProj;
	DirectX::SimpleMath::Matrix gTexTransform;
};

SHADER_CONSTANT_BUFFER(cbSkinMesh)
struct CB_SkinMesh : public cbSkinMesh
{
	DirectX::SimpleMath::Matrix gWorld;
	DirectX::SimpleMath::Matrix gInvWorld;
	DirectX::SimpleMath::Matrix gView;
	DirectX::SimpleMath::Matrix gProj;
	DirectX::SimpleMath::Matrix gTexTransform;
	DirectX::SimpleMath::Matrix gBoneTransforms[96];
};

SHADER_CONSTANT_BUFFER(cbInstanceSkinMesh)
struct CB_InstanceSkinMesh : public cbInstanceSkinMesh
{
	DirectX::SimpleMath::Matrix gView;
	DirectX::SimpleMath::Matrix gProj;
	DirectX::SimpleMath::Matrix gTexTransform;
	DirectX::SimpleMath::Matrix gBoneTransforms[96];
};

SHADER_CONSTANT_BUFFER(cbDepthStaticMesh)
struct CB_DepthStaticMesh : public cbDepthStaticMesh
{
	DirectX::SimpleMath::Matrix gWorldViewProj;
};

SHADER_CONSTANT_BUFFER(cbInstanceDepthStaticMesh)
struct CB_InstanceDepthStaticMesh : public cbInstanceDepthStaticMesh
{
	DirectX::SimpleMath::Matrix gViewProj;
};

SHADER_CONSTANT_BUFFER(cbDepthSkinMesh)
struct CB_DepthSkinMesh : public cbDepthSkinMesh
{
	DirectX::SimpleMath::Matrix gWorldViewProj;
	DirectX::SimpleMath::Matrix gBoneTransforms[96];
};

SHADER_CONSTANT_BUFFER(cbInstanceDepthSkinMesh)
struct CB_InstanceDepthSkinMesh : public cbInstanceDepthSkinMesh
{
	DirectX::SimpleMath::Matrix gViewProj;
	DirectX::SimpleMath::Matrix gBoneTransforms[96];
};

SHADER_CONSTANT_BUFFER(cbLightSub)
struct CB_LightSub : public cbLightSub
{
	DirectX::SimpleMath::Matrix gViewProjTex;
	DirectX::SimpleMath::Vector3 gEyePosW;
}; 

SHADER_CONSTANT_BUFFER(cbLight)
struct CB_Light : public cbLight
{
	DirectionalLightData gDirLights[DIRECTION_LIGHT_COUNT];
	PointLightData gPointLights[POINT_LIGHT_COUNT];
	SpotLightData gSpotLights[SPOT_LIGHT_COUNT];

	UINT gDirLightCount;
	UINT gPointLightCount;
	UINT gSpotLightCount;
};

SHADER_CONSTANT_BUFFER(cbMaterial)
struct CB_Material : public cbMaterial
{
	DirectX::SimpleMath::Vector4 gAddColor = DirectX::SimpleMath::Vector4(0.5f, 0.5f, 0.5f, 1.0f);
	float gEmissiveFactor = 1.0f;
	float gRoughnessFactor = 1.0f;
	float gMetallicFactor = 1.0f;

	UINT gTexID = 0;
};

SHADER_CONSTANT_BUFFER(cbFog)
struct CB_Fog : public cbFog
{
	DirectX::SimpleMath::Vector3 gFogColor;
	float gFogStartPos;
	DirectX::SimpleMath::Vector3 gEyePosW;
	float gTime;
};

/////////////////////////////////////////////////////////////////////////////////////////////
//// Blur Constant Buffer
/////////////////////////////////////////////////////////////////////////////////////////////

SHADER_CONSTANT_BUFFER(cbBlurTexel)
struct CB_BlurTexel : public cbBlurTexel
{
	DirectX::SimpleMath::Vector2 gTexelSize;
};

SHADER_CONSTANT_BUFFER(cbBlurOrder)
struct CB_BlurOrder : public cbBlurOrder
{
	DirectX::SimpleMath::Vector2 gBlurOrder;
};

/////////////////////////////////////////////////////////////////////////////////////////////
//// SSAO Constant Buffer
/////////////////////////////////////////////////////////////////////////////////////////////

SHADER_CONSTANT_BUFFER(cbSsaoObject)
struct CB_SsaoObject : public cbSsaoObject
{
	DirectX::SimpleMath::Matrix  gViewToTexSpace;
	DirectX::SimpleMath::Matrix  gProj;
};

SHADER_CONSTANT_BUFFER(cbSsaoFrustum)
struct CB_SsaoFrustum : public cbSsaoFrustum
{
	DirectX::SimpleMath::Vector4 gFrustumCorners[4];
};

SHADER_CONSTANT_BUFFER(cbSsaoOption)
struct CB_SsaoOption : public cbSsaoOption
{
	DirectX::SimpleMath::Vector4 gOffsetVectors[14];

	float    gOcclusionRadius = 0.1f;
	float    gOcclusionFadeStart = 0.2f;
	float    gOcclusionFadeEnd = 4.0f;
	float    gSurfaceEpsilon = 0.05f;
};

/////////////////////////////////////////////////////////////////////////////////////////////
//// Particle Constant Buffer
/////////////////////////////////////////////////////////////////////////////////////////////

SHADER_CONSTANT_BUFFER(cbInstanceParticleMesh)
struct CB_InstanceParticleMesh : public cbInstanceParticleMesh
{
	DirectX::SimpleMath::Matrix  gViewProj;
};

/////////////////////////////////////////////////////////////////////////////////////////////
//// OIT Constant Buffer
/////////////////////////////////////////////////////////////////////////////////////////////

SHADER_CONSTANT_BUFFER(cbOitFrame)
struct CB_OitFrame : public cbOitFrame
{
	UINT gFrameWidth;
};

/////////////////////////////////////////////////////////////////////////////////////////////
//// FXAA Constant Buffer
/////////////////////////////////////////////////////////////////////////////////////////////

SHADER_CONSTANT_BUFFER(cbFxaaFrame)
struct CB_FxaaFrame : public cbFxaaFrame
{
	DirectX::SimpleMath::Vector4 gFrame;
};

/////////////////////////////////////////////////////////////////////////////////////////////
//// SkyBox Constant Buffer
/////////////////////////////////////////////////////////////////////////////////////////////

/// 필요 없을듯
SHADER_CONSTANT_BUFFER(cbCubeObject)
struct CB_CubeObject : public cbCubeObject
{
	DirectX::SimpleMath::Matrix gViewProj;
};

/////////////////////////////////////////////////////////////////////////////////////////////
//// IBL Constant Buffer cbExternalData
/////////////////////////////////////////////////////////////////////////////////////////////

SHADER_CONSTANT_BUFFER(cbExternalData)
struct CB_ExternalData : public cbExternalData
{
	float gRoughness;
};

/////////////////////////////////////////////////////////////////////////////////////////////
//// Bloom Constant Buffer
/////////////////////////////////////////////////////////////////////////////////////////////

SHADER_CONSTANT_BUFFER(cbBloomBright)
struct CB_BloomBright : public cbBloomBright
{
	float gThreshold;
};

SHADER_CONSTANT_BUFFER(cbBloomBlurOrder)
struct CB_BloomBlurOrder : public cbBloomBlurOrder
{
	DirectX::SimpleMath::Vector2 gBlurTexelSize;
	DirectX::SimpleMath::Vector2 gBlurOrder;
};

SHADER_CONSTANT_BUFFER(cbBloomFinal)
struct CB_BloomFinal : public cbBloomFinal
{
	float gCoefficient;
};

/////////////////////////////////////////////////////////////////////////////////////////////
//// Picking Constant Buffer
/////////////////////////////////////////////////////////////////////////////////////////////
SHADER_CONSTANT_BUFFER(cbStaticMeshID)
struct CB_StaticMesh_ID : public cbStaticMeshID
{
	DirectX::SimpleMath::Vector4 gHashColor;
	DirectX::SimpleMath::Matrix gWorldViewProj;
};

SHADER_CONSTANT_BUFFER(cbInstanceStaticMeshID)
struct CB_Instance_StaticMesh_ID : public cbInstanceStaticMeshID
{
	DirectX::SimpleMath::Matrix gViewProj;
};

SHADER_CONSTANT_BUFFER(cbSkinMeshID)
struct CB_SkinMesh_ID : public cbSkinMeshID
{
	DirectX::SimpleMath::Vector4 gHashColor;
	DirectX::SimpleMath::Matrix gWorldViewProj;
	DirectX::SimpleMath::Matrix gBoneTransforms[96];
};

SHADER_CONSTANT_BUFFER(cbInstanceSkinMeshID)
struct CB_InstanceSkinMesh_ID : public cbInstanceSkinMeshID
{
	DirectX::SimpleMath::Matrix gViewProj;
	DirectX::SimpleMath::Matrix gBoneTransforms[96];
};

/////////////////////////////////////////////////////////////////////////////////////////////
//// Debug Constant Buffer
/////////////////////////////////////////////////////////////////////////////////////////////

SHADER_CONSTANT_BUFFER(cbDebugObject)
struct CB_DebugObject : public cbDebugObject
{
	DirectX::SimpleMath::Matrix  gWorldViewProj;
};

SHADER_CONSTANT_BUFFER(cbDebugOption)
struct CB_DebugOption : public cbDebugOption
{
	DirectX::SimpleMath::Vector3 gColor = DirectX::SimpleMath::Vector3(1.0f);
};


/// Resource Hash 재등록 방지 Define
RESOURCE_DEFINE(DEFINE_CB)