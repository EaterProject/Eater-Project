#pragma once
class AlphaPass : public RenderPassBase
{
public:
	AlphaPass();
	~AlphaPass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;

	void OnResize(int width, int height) override;
	void InstanceResize(size_t& renderMaxCount, size_t& unRenderMaxCount) override;
	
	void Release() override;

	void SetOption(RenderOption* renderOption) override;
	void SetIBLEnvironmentMapResource(EnvironmentBuffer* resource);

	void BeginRender();
	void RenderUpdate(const InstanceRenderBuffer* instance, const std::vector<RenderData*>& meshlist);
	void RenderUpdate(const InstanceRenderBuffer* instance, const RenderData* meshData);

private:
	void ParticleUpdate(ParticleData* particleSystem);

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
	UINT m_Width = 0;
};

