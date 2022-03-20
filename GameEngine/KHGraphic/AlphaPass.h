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
	VertexShader* m_ParticleVS;
	VertexShader* m_ParticleInstVS;
	VertexShader* m_MeshInstVS;
	PixelShader* m_ParticlePS;
	PixelShader* m_MeshPS;

	InstanceBuffer* m_Particle_IB;
	InstanceBuffer* m_Mesh_IB;

	VertexInput::ParticleInstance m_ParticleData;
	std::vector<VertexInput::ParticleInstance>	m_ParticleInstance;

	UINT m_InstanceCount = 0;
	size_t m_InstanceStride = 0;

	ID3D11Buffer* m_IndexBuffer;
	ID3D11Buffer* m_VertexBuffer;
	UINT m_IndexCount;
	UINT m_Stride;
	UINT m_Offset;
};

