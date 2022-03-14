#include "Player.h"
#include "PhysX_API.h"

void Player::BackUp_PrevData()
{
	// ���� �����͸� ����� ���´�.
	m_Prev_Data = *this;
}

void Player::Create()
{
	// Player�� �����̴� ��ü
	m_PhysData->isDinamic = true;
	PhysX_Create_Actor(m_PhysData);
}

Player::Player()
{

	m_PhysData->SetLockAxis_Position(false, true, false);
	m_PhysData->SetLockAxis_Rotation(true, true, true);
}

Player::~Player()
{
	PhysX_Delete_Actor(m_PhysData);
}

void Player::Update()
{
	// ���� �����͸� ����� ���´�.
	BackUp_PrevData();
	// PhysX Update
	PhysX_Update_Actor(m_PhysData);
	// ���� ��� ó���� ������ ��ȣ ����
	m_Processed_Sequence = m_Sequence;

	// ���� �����͸� ��ü�Ѵ�.
	m_Position = m_PhysData->WorldPosition;
	m_Rotation = m_PhysData->Rotation;
	m_Mov_Vector = DirectX::SimpleMath::Vector3(0, 0, 0);
}
