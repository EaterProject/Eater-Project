#include "SkinningFilter.h"
#include "GameObject.h"
#include "LoadManager.h"
#include "Transform.h"
#include "EngineData.h"
#include "BaseManager.h"
#include "ObjectManager.h"

SkinningFilter::SkinningFilter()
{
	BoneOffsetTM = nullptr;
	BoneSize = 0;
}

SkinningFilter::~SkinningFilter()
{
	BoneOffsetTM = nullptr;
}

void SkinningFilter::Update()
{
	MeshData* data = gameobject->OneMeshData;
	//������ �־��ֱ�
	for (int i = 0; i < BoneSize; i++)
	{
		if (BoneList[i] == nullptr) { continue; }
		//���� ����
		DirectX::XMMATRIX BoneWorld = *BoneList[i]->GetWorld();
	
		//���� ������
		DirectX::XMFLOAT4X4 temp = (*BoneOffsetTM)[i];
		DirectX::XMMATRIX Offset = DirectX::XMLoadFloat4x4(&temp);
	
		//�׷��� ������������ �Ѱ��ټ��ֵ��� ���� �־���
		(data->Object_Data->BoneOffsetTM)[i] = ((Offset * BoneWorld));
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

void SkinningFilter::PushBone_OffsetList(std::vector<DirectX::SimpleMath::Matrix>* mBoneOffsetTM)
{
	BoneOffsetTM = mBoneOffsetTM;

	BoneOffsetTM->resize(BoneSize);
	gameobject->OneMeshData->Object_Data->BoneOffsetTM.resize(BoneSize);
}
