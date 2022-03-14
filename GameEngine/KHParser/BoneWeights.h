#pragma once
struct Weight
{
	Weight(int num, float weight) : m_BoneNumber(num), m_BoneWeight(weight) {}
	
	int			m_BoneNumber;		// Bone IndexNumber
	float		m_BoneWeight;		// Bone Weight
};

class BoneWeights
{
public:
	BoneWeights();
	~BoneWeights();

public:
	void AddBoneWeight(int boneIndex, float boneWeight);
	void Normalize();

public:
	float sumWeight;
	std::vector<Weight> m_BoneWeights;
};

