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
	void Release() override;

	void SetOption(RenderOption* renderOption) override;

	void BeginRender();
	void RenderUpdate(const InstanceRenderBuffer* instance, const std::vector<RenderData*>& meshlist);
	void RenderUpdate(const InstanceRenderBuffer* instance, const RenderData* meshData);

private:
	void ParticleUpdate(ParticleData* particleSystem);

private:
	VertexShader* m_ParticleInstVS;
	VertexShader* m_MeshInst_VS;
	PixelShader* m_ParticlePS;
	PixelShader* m_MeshPS;

	InstanceBuffer* m_Particle_IB;
	InstanceBuffer* m_Mesh_IB;

private:
	VertexInput::ParticleInstance m_ParticleData;
	std::vector<VertexInput::ParticleInstance>	m_ParticleInstance;

	UINT m_InstanceCount = 0;
	size_t m_InstanceStride = 0;
};

