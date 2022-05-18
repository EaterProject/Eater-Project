#pragma once
namespace Cull
{
	class Plane
	{
	public:
		Plane() = default;
	public:
		DirectX::SimpleMath::Vector4 point;

	public:
		void Normalize()
		{
			float mag = sqrt(point.x * point.x + point.y * point.y + point.z * point.z);

			point.x = point.x / mag;
			point.y = point.y / mag;
			point.z = point.z / mag;
			point.w = point.w / mag;
		}
	};

	class Frustum
	{
	public:
		union
		{
			struct
			{
				Plane Left;
				Plane Right;
				Plane Top;
				Plane Bottom;
				Plane Near;
				Plane Far;
			};
			Plane Planes[6];
		};

	public:
		Frustum()
		{
			Planes[0].point = DirectX::SimpleMath::Vector4();
			Planes[1].point = DirectX::SimpleMath::Vector4();
			Planes[2].point = DirectX::SimpleMath::Vector4();
			Planes[3].point = DirectX::SimpleMath::Vector4();
			Planes[4].point = DirectX::SimpleMath::Vector4();
			Planes[5].point = DirectX::SimpleMath::Vector4();
		}

	public:
		void FrustumTransform(const DirectX::SimpleMath::Matrix viewproj)
		{
			Left.point.x = viewproj._14 + viewproj._11;
			Left.point.y = viewproj._24 + viewproj._21;
			Left.point.z = viewproj._34 + viewproj._31;
			Left.point.w = viewproj._44 + viewproj._41;

			Right.point.x = viewproj._14 - viewproj._11;
			Right.point.y = viewproj._24 - viewproj._21;
			Right.point.z = viewproj._34 - viewproj._31;
			Right.point.w = viewproj._44 - viewproj._41;

			Top.point.x = viewproj._14 - viewproj._12;
			Top.point.y = viewproj._24 - viewproj._22;
			Top.point.z = viewproj._34 - viewproj._32;
			Top.point.w = viewproj._44 - viewproj._42;

			Bottom.point.x = viewproj._14 + viewproj._12;
			Bottom.point.y = viewproj._24 + viewproj._22;
			Bottom.point.z = viewproj._34 + viewproj._32;
			Bottom.point.w = viewproj._44 + viewproj._42;

			Near.point.x = viewproj._13;
			Near.point.y = viewproj._23;
			Near.point.z = viewproj._33;
			Near.point.w = viewproj._43;

			Far.point.x = viewproj._14 - viewproj._13;
			Far.point.y = viewproj._24 - viewproj._23;
			Far.point.z = viewproj._34 - viewproj._33;
			Far.point.w = viewproj._44 - viewproj._43;

			// Normalize Plane
			Left.Normalize();
			Right.Normalize();
			Top.Normalize();
			Bottom.Normalize();
			Near.Normalize();
			Far.Normalize();
		}
	};
}
