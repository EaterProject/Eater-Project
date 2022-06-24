#pragma once
class Alpha_Pass : public RenderPassBase
{
public:
	Alpha_Pass();
	~Alpha_Pass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;

	void OnResize(int width, int height) override;
	void InstanceResize(size_t& renderMaxCount, size_t& unRenderMaxCount) override;
	
	void Release() override;

	void ApplyOption() override;

public:
	void BeginRender();
	void RenderUpdate(const InstanceRenderBuffer* instance, const std::vector<RenderData*>& meshlist);
	void RenderUpdate(const InstanceRenderBuffer* instance, const RenderData* meshData);

private:
	VertexShader* m_ParticleInst_VS;
	VertexShader* m_Mesh_VS;
	VertexShader* m_MeshInst_VS;
	PixelShader* m_Particle_PS;
	PixelShader* m_Mesh_PS;

	InstanceBuffer* m_Particle_IB;
	InstanceBuffer* m_Mesh_IB;

private:
	RenderData* m_RenderData;

	VertexInput::MeshInstance m_MeshData;
	std::vector<VertexInput::MeshInstance>	m_MeshInstance;

	VertexInput::ParticleInstance m_ParticleData;
	std::vector<VertexInput::ParticleInstance>	m_ParticleInstance;

	UINT m_RenderCount = 0;
	UINT m_InstanceCount = 0;
};

