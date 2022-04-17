#pragma once
#include "SimpleMath.h"

namespace VertexInput
{
#pragma region Defalt CVertex Data Struct
	struct MeshVertex
	{
		DirectX::SimpleMath::Vector3 Pos;
		DirectX::SimpleMath::Vector2 Tex;
		DirectX::SimpleMath::Vector3 Normal;
		DirectX::SimpleMath::Vector3 Tangent;
	};

	struct SkinVertex
	{
		UINT BoneIndex1[4];
		UINT BoneIndex2[4];
		float BoneWeight1[4];
		float BoneWeight2[4];

		DirectX::SimpleMath::Vector3 Pos;
		DirectX::SimpleMath::Vector2 Tex;
		DirectX::SimpleMath::Vector3 Normal;
		DirectX::SimpleMath::Vector3 Tangent;
	};

	struct TerrainVertex
	{
		DirectX::SimpleMath::Vector3 Pos;
		DirectX::SimpleMath::Vector2 Tex;
		DirectX::SimpleMath::Vector3 Normal;
		DirectX::SimpleMath::Vector3 Tangent;
		DirectX::SimpleMath::Vector4 Mask;
	};

	struct PosVertex
	{
		DirectX::SimpleMath::Vector3 Pos;
	};

	struct PosColorVertex
	{
		DirectX::SimpleMath::Vector3 Pos;
		DirectX::SimpleMath::Vector4 Color;
	};

	struct PosTexVertex
	{
		DirectX::SimpleMath::Vector3 Pos;
		DirectX::SimpleMath::Vector2 Tex;
	};

	struct PosNormalTexVertex
	{
		DirectX::SimpleMath::Vector3 Pos;
		DirectX::SimpleMath::Vector2 Tex;
		DirectX::SimpleMath::Vector3 Normal;
	};
#pragma endregion Defalt CVertex Data Struct

#pragma region Instance CVertex Data Struct
	struct MeshDepthInstance
	{
		DirectX::SimpleMath::Matrix World;
	};

	struct MeshInstance
	{
		DirectX::SimpleMath::Matrix World;
		DirectX::SimpleMath::Matrix InvWorld;
	};

	struct SkinMeshInstance
	{
		DirectX::SimpleMath::Matrix World;
		DirectX::SimpleMath::Matrix InvWorld;

		UINT PrevAnimationIndex = 0;
		UINT NextAnimationIndex = 0;
		float FrameTime = 0.0f;
		float Pad = 0.0f;
	};

	struct MeshIDInstance
	{
		DirectX::SimpleMath::Matrix World;
		DirectX::SimpleMath::Vector4 HashColor;
	};

	struct ParticleInstance
	{
		DirectX::SimpleMath::Matrix World;
		DirectX::SimpleMath::Matrix TexTransform;
		DirectX::SimpleMath::Vector4 Color;
	};
#pragma endregion Instance CVertex Data Struct
}