#include <vector>
#include "BoneWeights.h"

BoneWeights::BoneWeights()
	:sumWeight(0.0f)
{

}

BoneWeights::~BoneWeights()
{
	m_BoneWeights.clear();
}

void BoneWeights::AddBoneWeight(int boneIndex, float boneWeight)
{
	if (boneWeight <= 0.0f)
		return;

	sumWeight += boneWeight;

	m_BoneWeights.push_back(Weight(boneIndex, boneWeight));
}

void BoneWeights::Normalize()
{
	float fScale = 1.0f / sumWeight;

	std::vector<Weight>::iterator it = m_BoneWeights.begin();
	while (it != m_BoneWeights.end())
	{
		(*it).m_BoneWeight *= fScale;
		++it;
	}
}
