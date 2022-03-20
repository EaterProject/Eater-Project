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
	//데이터 넣어주기
	for (int i = 0; i < BoneSize; i++)
	{
		if (BoneList[i] == nullptr) { continue; }
		//본의 월드
		DirectX::XMMATRIX BoneWorld = *BoneList[i]->GetWorld();
	
		//본의 오프셋
		DirectX::XMFLOAT4X4 temp = (*BoneOffsetTM)[i];
		DirectX::XMMATRIX Offset = DirectX::XMLoadFloat4x4(&temp);
	
		//그래픽 랜더링쪽으로 넘겨줄수있도록 값을 넣어줌
		(data->Object_Data->BoneOffsetTM)[i] = ((Offset * BoneWorld));
	}
}

void SkinningFilter::PushBoneList(std::vector<GameObject*>* mBoneList)
{
	// BoneList 와 BoneOffsetTM 은 1:1 매칭이 되어야한다
	// 받은 본 오브젝트 리스트를 Tranform만 뽑아서 다시저장
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
