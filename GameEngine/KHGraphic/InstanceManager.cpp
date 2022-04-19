#include "DirectDefine.h"
#include "VertexDefine.h"
#include "GraphicResource.h"
#include "DrawBuffer.h"
#include "InstanceManager.h"

VertexInput::MeshInstance		InstanceManager::g_Mesh;
VertexInput::SkinMeshInstance	InstanceManager::g_SkinMesh;
VertexInput::MeshDepthInstance	InstanceManager::g_MeshDepth;
VertexInput::MeshIDInstance		InstanceManager::g_MeshID;
VertexInput::ParticleInstance	InstanceManager::g_Particle;

std::vector<VertexInput::MeshInstance>		InstanceManager::g_Mesh_Instance;
std::vector<VertexInput::SkinMeshInstance>	InstanceManager::g_SkinMesh_Instance;
std::vector<VertexInput::MeshDepthInstance>	InstanceManager::g_MeshDepth_Instance;
std::vector<VertexInput::MeshIDInstance>	InstanceManager::g_MeshID_Instance;
std::vector<VertexInput::ParticleInstance>	InstanceManager::g_Particle_Instance;

UINT		InstanceManager::g_RenderCount = 0;
UINT		InstanceManager::g_InstanceCount = 0;
size_t		InstanceManager::g_InstanceStride = 0;
const UINT	InstanceManager::g_MaxCount = 500;

void InstanceManager::BufferUpdate(InstanceBuffer* instance, void* instanceData, ID3D11DeviceContext* context)
{
	// Mapping SubResource Data..
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	// GPU Access Lock Buffer Data..
	context->Map(instance->InstanceBuf->Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	g_InstanceStride = (size_t)instance->Stride * (size_t)g_InstanceCount;

	// Copy Resource Data..
	memcpy(mappedResource.pData, instanceData, g_InstanceStride);

	// GPU Access UnLock Buffer Data..
	context->Unmap(instance->InstanceBuf->Get(), 0);
}

void InstanceManager::ResetCount()
{
	g_RenderCount = 0;
	g_InstanceCount = 0;
	g_InstanceStride = 0;
}
