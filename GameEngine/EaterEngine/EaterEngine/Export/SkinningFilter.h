#pragma once

#include "EaterEngineDLL.h"
#include "Component.h"
#include <vector>

class Transform;
class GameObject;

namespace ParserData 
{
	class CMesh;
}

namespace DirectX
{
	namespace SimpleMath
	{
		struct Matrix;
	}
}

class SkinningFilter :public Component
{
public:

	EATER_ENGINEDLL SkinningFilter();
	virtual EATER_ENGINEDLL ~SkinningFilter();
	virtual void Update();

	void PushBoneList(std::vector<GameObject*>* mBoneList);
	void PushBone_OffsetList(std::vector<DirectX::SimpleMath::Matrix>& mBoneOffsetTM);
private:
	//���� ������ TM
	std::vector<DirectX::SimpleMath::Matrix> BoneOffsetTM;
	//�� ����Ʈ
	std::vector<Transform*> BoneList;

	int BoneSize;
};