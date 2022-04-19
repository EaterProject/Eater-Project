#pragma once

/// 2022/04/19 0:55
/// SeoKyuHwang
///
/// # Instance Manager Class
///
/// - Instance 관련 Data를 전역으로 관리하는 Class
/// - Instance 관련 Rendering에서 전역으로 사용

class InstanceManager
{
public:
	static VertexInput::MeshInstance		g_Mesh;
	static VertexInput::SkinMeshInstance	g_SkinMesh;
	static VertexInput::MeshDepthInstance	g_MeshDepth;
	static VertexInput::MeshIDInstance		g_MeshID;
	static VertexInput::ParticleInstance	g_Particle;

	static std::vector<VertexInput::MeshInstance>		g_Mesh_Instance;
	static std::vector<VertexInput::SkinMeshInstance>	g_SkinMesh_Instance;
	static std::vector<VertexInput::MeshDepthInstance>	g_MeshDepth_Instance;
	static std::vector<VertexInput::MeshIDInstance>		g_MeshID_Instance;
	static std::vector<VertexInput::ParticleInstance>	g_Particle_Instance;

	static UINT g_RenderCount;
	static UINT g_InstanceCount;
	static size_t g_InstanceStride;
	static const UINT g_MaxCount;

public:
	static void BufferUpdate(InstanceBuffer* instance, void* instanceData, ID3D11DeviceContext* context);
	static void ResetCount();
};

