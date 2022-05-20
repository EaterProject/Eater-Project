#pragma once
class Shadow_Pass : public RenderPassBase
{
public:
	Shadow_Pass();
	~Shadow_Pass();

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
	void SetShaderList();
	void SetShaderResourceView();

private:
	VertexShader* m_MeshShadow_VS;
	VertexShader* m_MeshInstShadow_VS;
	VertexShader* m_SkinShadow_VS;
	VertexShader* m_SkinInstShadow_VS;

	DepthStencil* m_Shadow_DS;

	InstanceBuffer* m_Mesh_IB;
	InstanceBuffer* m_SkinMesh_IB;

private:
	ID3D11DepthStencilView* m_Shadow_DSV;
	ID3D11RasterizerState* m_Depth_RS;
	D3D11_VIEWPORT* m_Shadow_VP;

private:
	RenderData* m_RenderData;

	VertexInput::MeshDepthInstance m_MeshData;
	std::vector<VertexInput::MeshDepthInstance>	m_MeshInstance;

	VertexInput::SkinMeshDepthInstance m_SkinMeshData;
	std::vector<VertexInput::SkinMeshDepthInstance>	m_SkinMeshInstance;

	UINT m_RenderCount = 0;
	UINT m_InstanceCount = 0;
};

