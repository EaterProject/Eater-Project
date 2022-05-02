#pragma once
class DeferredPass : public RenderPassBase
{
public:
	DeferredPass();
	~DeferredPass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;

	void OnResize(int width, int height) override;
	void InstanceResize(size_t& renderMaxCount, size_t& unRenderMaxCount) override;
	
	void Release() override;

	void ApplyOption(UINT renderOption);

public:
	void BeginRender();
	void RenderUpdate(const InstanceRenderBuffer* instance, const std::vector<RenderData*>& meshlist);
	void RenderUpdate(const InstanceRenderBuffer* instance, const RenderData* mesh);

private:
	VertexShader* m_Mesh_VS;
	VertexShader* m_MeshInst_VS;
	VertexShader* m_Skin_VS;
	VertexShader* m_SkinInst_VS;
	VertexShader* m_Terrain_VS;

	PixelShader* m_Deferred_PS;
	PixelShader* m_Terrain_PS;

	InstanceBuffer* m_Mesh_IB;
	InstanceBuffer* m_SkinMesh_IB;

	RenderTexture* m_Albedo_RT;
	RenderTexture* m_Emissive_RT;
	RenderTexture* m_Normal_RT;
	RenderTexture* m_Position_RT;
	RenderTexture* m_NormalDepth_RT;

private:
	std::vector<ID3D11RenderTargetView*> m_RTV_List;
	ID3D11DepthStencilView* m_Defalt_DSV;

	ID3D11DepthStencilState* m_Defalt_DSS;
	ID3D11RasterizerState* m_Solid_RS;
	ID3D11BlendState* m_AlphaBlend_BS;

	D3D11_VIEWPORT* m_Screen_VP;

private:
	RenderData* m_RenderData;

	VertexInput::MeshInstance m_MeshData;
	std::vector<VertexInput::MeshInstance>	m_MeshInstance;

	VertexInput::SkinMeshInstance m_SkinMeshData;
	std::vector<VertexInput::SkinMeshInstance>	m_SkinMeshInstance;

	UINT m_RenderCount = 0;
	UINT m_InstanceCount = 0;
};

