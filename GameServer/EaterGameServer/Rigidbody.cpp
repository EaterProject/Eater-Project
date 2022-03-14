#include "Rigidbody.h"
#include "PhysEngineDLL.h"
#include "PhysData.h"

Rigidbody::Rigidbody()
{
	m_PhysData = new PhysData();
	// ±âº»Àº Static Mesh
	m_PhysData->isDinamic = false;
}

Rigidbody::~Rigidbody()
{
	delete m_PhysData;
}
