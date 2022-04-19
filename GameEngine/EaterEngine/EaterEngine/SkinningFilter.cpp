#include "SkinningFilter.h"
#include "GameObject.h"
#include "LoadManager.h"
#include "Transform.h"
#include "EngineData.h"
#include "ObjectManager.h"

SkinningFilter::SkinningFilter()
{
	BoneSize = 0;
}

SkinningFilter::~SkinningFilter()
{
}

void SkinningFilter::Update()
{
	MeshData* data = gameobject->OneMeshData;
	//������ �־��ֱ�
	for (int i = 0; i < BoneSize; i++)
	{
		if (BoneList[i] == nullptr) { continue; }
	
		//�׷��� ������������ �Ѱ��ټ��ֵ��� ���� �־���
		data->Object_Data->BoneOffsetTM[i] = BoneOffsetTM[i] * (*BoneList[i]->GetWorld());
	}
}

void SkinningFilter::PushBoneList(std::vector<GameObject*>* mBoneList)
{
	// BoneList �� BoneOffsetTM �� 1:1 ��Ī�� �Ǿ���Ѵ�
	// ���� �� ������Ʈ ����Ʈ�� Tranform�� �̾Ƽ� �ٽ�����
	BoneSize = (int)mBoneList->size();
	BoneList.resize(BoneSize);

	for (int i = 0; i < BoneSize; i++)
	{
		if ((*mBoneList)[i] == nullptr) { continue; }
		BoneList[i] = (*mBoneList)[i]->transform;
	}
}

void SkinningFilter::PushBone_OffsetList(std::vector<DirectX::SimpleMath::Matrix>& mBoneOffsetTM)
{
	BoneOffsetTM = mBoneOffsetTM;

	gameobject->OneMeshData->Object_Data->BoneOffsetTM.resize(BoneSize);
}
