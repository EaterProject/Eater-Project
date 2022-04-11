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

class DebugPass : public RenderPassBase
{
public:
	DebugPass();
	~DebugPass();

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
	VertexShader* m_DebugVS;
	VertexShader* m_DebugIconVS;
	VertexShader* m_DebugMRTVS;
	PixelShader* m_DebugPS;
	PixelShader* m_DebugColorPS;
	PixelShader* m_DebugIconPS;
	PixelShader* m_DebugMRTPS;

	DrawBuffer* m_RayBuffer;
	DrawBuffer* m_FrustumBuffer;
	DrawBuffer* m_QuadBuffer;
	DrawBuffer* m_AxisBuffer;
	DrawBuffer* m_BoxBuffer;
	DrawBuffer* m_CircleBuffer;
	DrawBuffer* m_CircleSphereBuffer;
	DrawBuffer* m_GridBuffer;
	DrawBuffer* m_SphereBuffer;
	DrawBuffer* m_IconBuffer;

	DrawBuffer* m_DebugBuffer;

	ID3D11RasterizerState* m_SolidRS;
	ID3D11RasterizerState* m_WireRS;
	ID3D11RasterizerState* m_NoCullRS;
	ID3D11BlendState* m_AlphaBlendBS;

	ID3D11DepthStencilView* m_DefaltDSV;
	ID3D11RenderTargetView* m_MainRTV;

	ID3D11ShaderResourceView* m_DirectionalLightIcon;
	ID3D11ShaderResourceView* m_PointLightIcon;
	ID3D11ShaderResourceView* m_SpotLightIcon;
	ID3D11ShaderResourceView* m_CamerIcon;

	ID3D11ShaderResourceView* m_AlbedoRT;
	ID3D11ShaderResourceView* m_EmissiveRT;
	ID3D11ShaderResourceView* m_NormalRT;
	ID3D11ShaderResourceView* m_PositionRT;
	ID3D11ShaderResourceView* m_NormalDepthRT;
	ID3D11ShaderResourceView* m_BloomRT;
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

