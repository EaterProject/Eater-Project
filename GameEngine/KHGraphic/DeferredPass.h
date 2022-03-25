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
	void Release() override;

	void SetOption(UINT renderOption);

	void BeginRender();
	void RenderUpdate(const InstanceRenderBuffer* instance, const RenderData* mesh);
	void RenderUpdate(const InstanceRenderBuffer* instance, const std::vector<RenderData*>& meshlist);

private:
	VertexShader* m_MeshVS;
	VertexShader* m_MeshInstVS;
	VertexShader* m_SkinVS;
	VertexShader* m_SkinInstVS;
	VertexShader* m_TerrainVS;

	PixelShader* m_DeferredPS;
	PixelShader* m_TerrainPS;

	InstanceBuffer* m_Mesh_IB;

	ID3D11DepthStencilView* m_DefaltDSV;
	
	ID3D11DepthStencilState* m_DefaltDSS;
	ID3D11RasterizerState* m_SolidRS;
	ID3D11BlendState* m_AlphaBlendBS;

	RenderTexture* m_AlbedoRT;
	RenderTexture* m_EmissiveRT;
	RenderTexture* m_NormalRT;
	RenderTexture* m_PositionRT;
	RenderTexture* m_NormalDepthRT;

	std::vector<ID3D11RenderTargetView*> m_RTVList;

	VertexInput::MeshInstance m_MeshData;
	std::vector<VertexInput::MeshInstance>	m_MeshInstance;

	UINT m_InstanceCount = 0;
	size_t m_InstanceStride = 0;

	D3D11_VIEWPORT* m_ScreenVP;
};

