#pragma once
#include "HashBase.h"

#define INSTANCE_BUFFER(ClassName) CREATE_HASH_CLASS(ClassName, RESOURCE_TYPE::IB)

///////////////////////////////////////////////////////////////////////////////////////////
// Global Instance Buffer
///////////////////////////////////////////////////////////////////////////////////////////

INSTANCE_BUFFER(IB_Mesh)
INSTANCE_BUFFER(IB_SkinMesh)
INSTANCE_BUFFER(IB_MeshDepth)
INSTANCE_BUFFER(IB_SkinMeshDepth)
INSTANCE_BUFFER(IB_MeshID)
INSTANCE_BUFFER(IB_Particle)
