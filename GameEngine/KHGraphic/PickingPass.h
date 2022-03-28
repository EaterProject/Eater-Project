#pragma once
class PickingPass : public RenderPassBase
{
public:
	PickingPass();
	~PickingPass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;

	void OnResize(int width, int height) override;
	void Release() override;

	void BeginRender();
	void RenderUpdate(const InstanceRenderBuffer* instance, const RenderData* mesh);
	void RenderUpdate(const InstanceRenderBuffer* instance, const std::vector<RenderData*>& meshlist);
	void NoneMeshRenderUpdate(const std::vector<RenderData*>& meshlist);
	int FindPick(int x, int y);

private:
	int ColorToHash(Vector4 color);

private:
	VertexShader* m_Mesh_VS;
	VertexShader* m_Mesh_Inst_VS;
	VertexShader* m_Skin_VS;
	VertexShader* m_Skin_Inst_VS;
	PixelShader* m_Mesh_ID_PS;

	InstanceBuffer* m_MeshID_IB;

	DrawBuffer* m_Box_DB;

	RenderTexture* m_ID_RT;
	RenderTexture* m_ID_Copy_RT;

	ID3D11Texture2D* m_ID_Tex2D;
	ID3D11Texture2D* m_ID_CopyTex2D;
	ID3D11RenderTargetView* m_ID_RTV;

	ID3D11DepthStencilState* m_Defalt_DSS;
	ID3D11DepthStencilView* m_Defalt_DSV;

	D3D11_VIEWPORT* m_Screen_VP;

	VertexInput::MeshIDInstance m_MeshData;
	std::vector<VertexInput::MeshIDInstance> m_MeshInstance;

	UINT m_InstanceCount = 0;
	size_t m_InstanceStride = 0;
};

