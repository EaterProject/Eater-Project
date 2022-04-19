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

	void BakeShadowMap();
	void CopyShadowMap();
	void SetShadowMap(ID3D11ShaderResourceView* shadowMap);
private:
	VertexShader* m_MeshShadowVS;
	VertexShader* m_MeshInstShadowVS;
	VertexShader* m_SkinShadowVS;
	VertexShader* m_SkinInstShadowVS;

	PixelShader* m_ShadowBaking_PS;

	VertexShader* m_Screen_VS;
	PixelShader* m_Copy_PS;

	DrawBuffer* m_Screen_DB;

	DepthStencil* m_ShadowDS;

	InstanceBuffer* m_Mesh_IB;
	
	VertexInput::MeshDepthInstance m_MeshData;
	std::vector<VertexInput::MeshDepthInstance>	m_MeshInstance;

	UINT m_InstanceCount = 0;
	size_t m_InstanceStride = 0;

	ID3D11ShaderResourceView* m_ShadowMap;

	ID3D11DepthStencilView* m_ShadowDSV;
	ID3D11RasterizerState* m_DepthRS;
	D3D11_VIEWPORT* m_ShadowVP;
};

