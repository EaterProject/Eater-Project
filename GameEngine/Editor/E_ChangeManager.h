#pragma once
#include "SimpleMath.h"
#include <string>

namespace DirectX
{
	namespace SimpleMath
	{
		struct Matrix;
	}
}

class GameObject;
class MeshFilter;
class Transform;
class E_ChangeManager
{
public:
	E_ChangeManager();
	~E_ChangeManager();

	void SetModelName(std::string ModelName);
public:
	void Change_Static(int index, GameObject* Object);
	void Change_Skin(int index, GameObject* Object);
	void Change_Bone(int index, GameObject* Object);
	void Change_Material(int index, GameObject* Object);
	void Change_Animation(int index, GameObject* Object);
public:
	void Change_LocalTM(int Nodeindex, Transform* TR);
	void Change_WorldTM(int NodeIndex, Transform* TR);
	void Change_Name(int Nodeindex,MeshFilter* MF);
	std::string GetOriginalName(std::string Original);
	std::string ModelName;
};

