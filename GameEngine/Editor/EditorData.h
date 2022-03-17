#pragma once
#include <string>
#include <atlstr.h>
#include "EngineData.h"

static std::string GetDataString(CString Num)
{
	CT2CA convertedString(Num);
	return convertedString;
}

static CString GetDataCString(std::string Num)
{
	CString Number;
	Number = Num.c_str();
	return Number;
}

struct MeshOption
{
	std::string Name;
	int SaveType;

	const int BASE		= 0;
	const int SKIN		= 1;
	const int TERRAIN	= 2;
	const int ANIMATION = 3;

	//매트릭스 정보
	DirectX::SimpleMath::Vector3 Position;
	DirectX::SimpleMath::Vector3 Rotation;
	DirectX::SimpleMath::Vector3 Scale;

	DirectX::SimpleMath::Vector3 Position_Bone;
	DirectX::SimpleMath::Vector3 Rotation_Bone;
	DirectX::SimpleMath::Vector3 Scale_Bone;

	//움직이는 오브젝트 여부
	bool DinamicObject = false;

	DirectX::SimpleMath::Matrix GetMatrix()
	{
		DirectX::SimpleMath::Matrix Position_4x4;
		Position_4x4._11 = 1;				Position_4x4._12 = 0;			Position_4x4._13 = 0;			Position_4x4._14 = 0;
		Position_4x4._21 = 0;				Position_4x4._22 = 1;			Position_4x4._23 = 0;			Position_4x4._24 = 0;
		Position_4x4._31 = 0;				Position_4x4._32 = 0;			Position_4x4._33 = 1;			Position_4x4._34 = 0;
		Position_4x4._41 = Position.x;		Position_4x4._42 = Position.y;	Position_4x4._43 = Position.z;	Position_4x4._44 = 1;

		float radX = Rotation.x * 3.141592f / 180;
		float radY = Rotation.y * 3.141592f / 180;
		float radZ = Rotation.z * 3.141592f / 180;
		DirectX::XMMATRIX _P = DirectX::XMMatrixRotationX(radX);
		DirectX::XMMATRIX _Y = DirectX::XMMatrixRotationY(radY);
		DirectX::XMMATRIX _R = DirectX::XMMatrixRotationZ(radZ);
		DirectX::XMMATRIX Rotation_4x4 = _R* _Y* _P;

		DirectX::SimpleMath::Matrix Scale_4x4;
		Scale_4x4._11 = Scale.x;	Scale_4x4._12 = 0;			Scale_4x4._13 = 0;			Scale_4x4._14 = 0;
		Scale_4x4._21 = 0;			Scale_4x4._22 = Scale.y;	Scale_4x4._23 = 0;			Scale_4x4._24 = 0;
		Scale_4x4._31 = 0;			Scale_4x4._32 = 0;			Scale_4x4._33 = Scale.z;	Scale_4x4._34 = 0;
		Scale_4x4._41 = 0;			Scale_4x4._42 = 0;			Scale_4x4._43 = 0;			Scale_4x4._44 = 1;

		return Scale_4x4 * Rotation_4x4 * Position_4x4;
	}

	DirectX::SimpleMath::Matrix GetBoneMatrix()
	{
		DirectX::SimpleMath::Matrix Position_4x4;
		Position_4x4._11 = 1;				Position_4x4._12 = 0;			Position_4x4._13 = 0;			Position_4x4._14 = 0;
		Position_4x4._21 = 0;				Position_4x4._22 = 1;			Position_4x4._23 = 0;			Position_4x4._24 = 0;
		Position_4x4._31 = 0;				Position_4x4._32 = 0;			Position_4x4._33 = 1;			Position_4x4._34 = 0;
		Position_4x4._41 = Position_Bone.x;		Position_4x4._42 = Position_Bone.y;	Position_4x4._43 = Position_Bone.z;	Position_4x4._44 = 1;

		float radX = Rotation_Bone.x * 3.141592f / 180;
		float radY = Rotation_Bone.y * 3.141592f / 180;
		float radZ = Rotation_Bone.z * 3.141592f / 180;
		DirectX::XMMATRIX _P = DirectX::XMMatrixRotationX(radX);
		DirectX::XMMATRIX _Y = DirectX::XMMatrixRotationY(radY);
		DirectX::XMMATRIX _R = DirectX::XMMatrixRotationZ(radZ);
		DirectX::XMMATRIX Rotation_4x4 = _R * _Y * _P;

		DirectX::SimpleMath::Matrix Scale_4x4;
		Scale_4x4._11 = Scale_Bone.x;	Scale_4x4._12 = 0;			Scale_4x4._13 = 0;			Scale_4x4._14 = 0;
		Scale_4x4._21 = 0;			Scale_4x4._22 = Scale_Bone.y;	Scale_4x4._23 = 0;			Scale_4x4._24 = 0;
		Scale_4x4._31 = 0;			Scale_4x4._32 = 0;			Scale_4x4._33 = Scale_Bone.z;	Scale_4x4._34 = 0;
		Scale_4x4._41 = 0;			Scale_4x4._42 = 0;			Scale_4x4._43 = 0;				Scale_4x4._44 = 1;

		return Scale_4x4 * Rotation_4x4 * Position_4x4;
	}

	//texture
	std::string Diffuse		= "";
	std::string Normal		= "";
	std::string Emissive	= "";
	std::string ORM			= "";
};

struct FBXMeshData
{
	std::string Name;
	
	std::string Diffuse;
	std::string Normal;
	std::string Emissive;
	std::string ORM;
};

struct TransformData
{
	std::string Pos[3];
	std::string Rot[3];
	std::string Scl[3];
};

struct AnimationData
{
	bool Use = false;
	int Count = 0;
	float Speed = 5.0f;
	std::string AnimationList[5];
	std::string NowAnimation;
};

struct RenderMeshData
{
	enum class TYPE
	{
		BASE = 0,
		SKIN = 1,
		TERRAIN =2,
		ANIMATION =3,
	};

	std::string MeshName = "";
	bool Error = false;

	bool isTransform;
	bool isAnimation;

	TransformData mTransform;
	AnimationData mAnimation;
};
