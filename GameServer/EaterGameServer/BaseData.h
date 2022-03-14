#pragma once

#include "SimpleMath.h"

class BaseData
{
public:
	BaseData();
	BaseData& operator=(const BaseData& _rhs)
	{
		m_Speed = _rhs.m_Speed;
		m_Animation = _rhs.m_Animation;
		m_Sequence = _rhs.m_Sequence;
		m_Index = _rhs.m_Index;
		m_Type = _rhs.m_Type;
		m_Position = _rhs.m_Position;
		m_Mov_Vector = _rhs.m_Mov_Vector;
		m_Scale = _rhs.m_Scale;
		m_Rotation = _rhs.m_Rotation;
		return *this;
	}

	virtual ~BaseData();

public:
	// �ӵ�
	float			m_Speed = 0.f;
	// Animation
	unsigned short	m_Animation = 30000;
	// ������
	unsigned short	m_Sequence = 0;
	// ó���� ������
	unsigned short	m_Processed_Sequence = 0;
	// ��ȣ
	unsigned short	m_Index = 0;
	// Ÿ��
	unsigned short	m_Type = 0;
	// ��ġ
	DirectX::SimpleMath::Vector3 m_Position;
	// ���� ����
	DirectX::SimpleMath::Vector3 m_Mov_Vector;
	// ũ��
	DirectX::SimpleMath::Vector3 m_Scale;
	// ȸ��(���ʹϾ�)
	DirectX::SimpleMath::Quaternion m_Rotation;
};

