#pragma once
class ShadowPass : public RenderPassBase
{
public:
	ShadowPass();
	~ShadowPass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;

	void OnResize(int width, int height) override;
	void InstanceResize(size_t& renderMaxCount, size_t& unRenderMaxCount) override;

	void Release() override;

	void SetOption(RenderOption* renderOption) override;

public:
	void BeginRender();
	void RenderUpdate(const InstanceRenderBuffer* instance, const std::vector<RenderData*>& meshlist);
	void RenderUpdate(const InstanceRenderBuffer* instance, const RenderData* meshData);

private:
	VertexShader* m_MeshShadowVS;
	VertexShader* m_MeshInstShadowVS;
	VertexShader* m_SkinShadowVS;
	VertexShader* m_SkinInstShadowVS;

	DepthStencil* m_ShadowDS;

	InstanceBuffer* m_Mesh_IB;
	
	VertexInput::MeshDepthInstance m_MeshData;
	std::vector<VertexInput::MeshDepthInstance>	m_MeshInstance;

	UINT m_InstanceCount = 0;
	size_t m_InstanceStride = 0;


	ID3D11DepthStencilView* m_ShadowDSV;
	ID3D11RasterizerState* m_DepthRS;
	D3D11_VIEWPORT* m_ShadowVP;
};

