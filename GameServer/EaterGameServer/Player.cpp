#include "Player.h"
#include "PhysX_API.h"

void Player::BackUp_PrevData()
{
	// 이전 데이터를 백업해 놓는다.
	m_Prev_Data = *this;
}

void Player::Create()
{
	// Player는 움직이는 객체
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
	// 이전 데이터를 백업해 놓는다.
	BackUp_PrevData();
	// PhysX Update
	PhysX_Update_Actor(m_PhysData);
	// 물리 운동이 처리된 시퀀스 번호 저장
	m_Processed_Sequence = m_Sequence;

	// 현재 데이터를 교체한다.
	m_Position = m_PhysData->WorldPosition;
	m_Rotation = m_PhysData->Rotation;
	m_Mov_Vector = DirectX::SimpleMath::Vector3(0, 0, 0);
}
