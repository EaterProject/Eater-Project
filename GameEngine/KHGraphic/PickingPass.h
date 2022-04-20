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
	void InstanceResize(size_t& renderMaxCount, size_t& unRenderMaxCount) override;

	void Release() override;

	void BeginRender();
	void RenderUpdate(const InstanceRenderBuffer* instance, const RenderData* mesh);
	void RenderUpdate(const InstanceRenderBuffer* instance, const std::vector<RenderData*>& meshlist);
	void NoneMeshRenderUpdate(const std::vector<RenderData*>& meshlist);
	UINT FindPick(int x, int y);

private:
	VertexShader* m_Mesh_VS;
	VertexShader* m_Mesh_Inst_VS;
	VertexShader* m_Skin_VS;
	VertexShader* m_Skin_Inst_VS;
	PixelShader* m_Mesh_ID_PS;

	InstanceBuffer* m_MeshID_IB;
	InstanceBuffer* m_SkinMeshID_IB;

	DrawBuffer* m_Box_DB;

	RenderTexture* m_ID_RT;
	RenderTexture* m_ID_Copy_RT;

private:
	ID3D11Texture2D* m_ID_Tex2D;
	ID3D11Texture2D* m_ID_CopyTex2D;
	ID3D11RenderTargetView* m_ID_RTV;

	ID3D11DepthStencilState* m_Defalt_DSS;
	ID3D11DepthStencilView* m_Defalt_DSV;

	D3D11_VIEWPORT* m_Screen_VP;
	D3D11_BOX m_PickPointBox;

private:
	RenderData* m_RenderData;
	
	VertexInput::MeshIDInstance m_MeshData;
	std::vector<VertexInput::MeshIDInstance> m_MeshInstance;

	VertexInput::SkinMeshIDInstance m_SkinMeshData;
	std::vector<VertexInput::SkinMeshIDInstance> m_SkinMeshInstance;

	UINT m_RenderCount = 0;
	UINT m_InstanceCount = 0;
};

