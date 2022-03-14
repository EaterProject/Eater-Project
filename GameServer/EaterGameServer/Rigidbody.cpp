#include "Rigidbody.h"
#include "PhysEngineDLL.h"
#include "PhysData.h"

Rigidbody::Rigidbody()
{
	m_PhysData = new PhysData();
	// �⺻�� Static Mesh
	m_PhysData->isDinamic = false;
}

Rigidbody::~Rigidbody()
{
	delete m_PhysData;
}
