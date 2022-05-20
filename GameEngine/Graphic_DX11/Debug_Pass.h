#pragma once
typedef enum DEBUG_TYPE
{
	DEBUG_AXIS,
	DEBUG_BOX,
	DEBUG_CIRCLE,
	DEBUG_CIRCLESPHERE,
	DEBUG_GRID,
	DEBUG_QUAD,
	DEBUG_RAY,
	DEBUG_FRUSTUM,
	DEBUG_SPHERE,
	DEBUG_TEXTURE
}DEBUG_TYPE;

class Debug_Pass : public RenderPassBase
{
public:
	Debug_Pass();
	~Debug_Pass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;
	void OnResize(int width, int height) override;
	void Release() override;

	void BeginRender();
	void RenderUpdate(const RenderData* meshData);
	void GlobalRender();
	void MRTRender();

private:
	void BufferUpdate(DEBUG_TYPE type);
	void SetRay(const Vector3& start, const Vector3& end);
	void SetFrustum(const Vector3* corner);

	Matrix LookAt_Matrix(Vector3 pos, Vector3 look);

private:
	VertexShader* m_Debug_VS;
	VertexShader* m_DebugIcon_VS;
	VertexShader* m_DebugMRT_VS;
	PixelShader* m_Debug_PS;
	PixelShader* m_DebugColor_PS;
	PixelShader* m_DebugIcon_PS;
	PixelShader* m_DebugMRT_PS;

	DrawBuffer* m_Ray_DB;
	DrawBuffer* m_Frustum_DB;
	DrawBuffer* m_Quad_DB;
	DrawBuffer* m_Axis_DB;
	DrawBuffer* m_Box_DB;
	DrawBuffer* m_Circle_DB;
	DrawBuffer* m_CircleSphere_DB;
	DrawBuffer* m_Grid_DB;
	DrawBuffer* m_Sphere_DB;
	DrawBuffer* m_Icon_DB;

	DrawBuffer* m_Debug_DB;

private:
	ID3D11RasterizerState* m_Solid_RS;
	ID3D11RasterizerState* m_Wire_RS;
	ID3D11RasterizerState* m_NoCull_RS;
	ID3D11BlendState* m_AlphaBlend_BS;

	ID3D11DepthStencilView* m_Defalt_DSV;
	ID3D11RenderTargetView* m_Main_RTV;

	ID3D11ShaderResourceView* m_DirectionalLightIcon;
	ID3D11ShaderResourceView* m_PointLightIcon;
	ID3D11ShaderResourceView* m_SpotLightIcon;
	ID3D11ShaderResourceView* m_ParticleIcon;
	ID3D11ShaderResourceView* m_CamerIcon;

	ID3D11ShaderResourceView* m_Albedo_RT;
	ID3D11ShaderResourceView* m_Emissive_RT;
	ID3D11ShaderResourceView* m_Normal_RT;
	ID3D11ShaderResourceView* m_Position_RT;
	ID3D11ShaderResourceView* m_NormalDepth_RT;
	ID3D11ShaderResourceView* m_Bloom_RT;
	ID3D11ShaderResourceView* m_ShadowMap;
	ID3D11ShaderResourceView* m_SSAOMap;

	D3D11_VIEWPORT* m_MRT1;
	D3D11_VIEWPORT* m_MRT2;
	D3D11_VIEWPORT* m_MRT3;
	D3D11_VIEWPORT* m_MRT4;
	D3D11_VIEWPORT* m_MRT5;
	D3D11_VIEWPORT* m_MRT6;
	D3D11_VIEWPORT* m_MRT7;
	D3D11_VIEWPORT* m_MRT8;
};

