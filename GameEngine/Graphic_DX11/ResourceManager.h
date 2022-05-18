#pragma once
#include <unordered_map>
#include "ResourceManagerBase.h"

interface ID3D11Graphic;
class SamplerState;

class GraphicResourceManager : public IGraphicResourceManager
{
public:
	GraphicResourceManager(ID3D11Graphic* graphic, IShaderManager* shaderManager);
	~GraphicResourceManager();

public:
	void Create() override;
	void Start() override;
	void OnResize(int width, int height) override;
	void Release() override;

public:
	RenderTexture* GetMainRenderTarget() override;
	void AddMainRenderTarget(RenderTexture* rtv) override;

public:
	DepthStencil* GetDepthStencil(Hash_Code hash_code) override;
	RenderTexture* GetRenderTexture(Hash_Code hash_code) override;
	RenderBuffer* GetRenderBuffer(Hash_Code hash_code) override;
	DrawBuffer* GetDrawBuffer(Hash_Code hash_code) override;
	InstanceBuffer* GetInstanceBuffer(Hash_Code hash_code) override;

public:
	RenderTargetView* GetRenderTargetView(Hash_Code hash_code) override;
	DepthStencilView* GetDepthStencilView(Hash_Code hash_code) override;
	ShaderResourceView* GetShaderResourceView(Hash_Code hash_code) override;
	UnorderedAccessView* GetUnorderedAccessView(Hash_Code hash_code) override;

public:
	BlendState* GetBlendState(Hash_Code hash_code) override;
	RasterizerState* GetRasterizerState(Hash_Code hash_code) override;
	DepthStencilState* GetDepthStencilState(Hash_Code hash_code) override;
	ViewPort* GetViewPort(Hash_Code hash_code) override;

public:
	void AddResource(Hash_Code hash_code, ResourceBase* resource) override;
	void DeleteResource(Hash_Code hash_code, RESOURCE_TYPE resource_type) override;
	
private:
	void SetShaderSampler();

private:
	ID3D11Graphic* m_Graphic;
	IShaderManager* m_ShaderManager;

	/////////////////////////////////////////////////////////////////////////////////////////
	// Graphic Struct Resource List
	/////////////////////////////////////////////////////////////////////////////////////////

	RenderTexture* m_BackBuffer;
	std::unordered_map<Hash_Code, DepthStencil*> m_DepthStencilList;
	std::unordered_map<Hash_Code, RenderTexture*> m_RenderTargetList;
	std::unordered_map<Hash_Code, RenderBuffer*> m_RenderBufferList;
	std::unordered_map<Hash_Code, DrawBuffer*> m_DrawBufferList;
	std::unordered_map<Hash_Code, InstanceBuffer*> m_InstanceBufferList;

	/////////////////////////////////////////////////////////////////////////////////////////
	// Graphic View Resource List
	/////////////////////////////////////////////////////////////////////////////////////////

	std::unordered_map<Hash_Code, RenderTargetView*> m_RenderTargetViewList;
	std::unordered_map<Hash_Code, DepthStencilView*> m_DepthStencilViewList;
	std::unordered_map<Hash_Code, ShaderResourceView*> m_ShaderResourceViewList;
	std::unordered_map<Hash_Code, UnorderedAccessView*> m_UnorderedAccessViewList;


	/////////////////////////////////////////////////////////////////////////////////////////
	// Graphic State Resource List
	/////////////////////////////////////////////////////////////////////////////////////////

	std::unordered_map<Hash_Code, DepthStencilState*> m_DepthStencilStateList;
	std::unordered_map<Hash_Code, RasterizerState*> m_RasterizerStateList;
	std::unordered_map<Hash_Code, BlendState*> m_BlendStateList;
	std::unordered_map<Hash_Code, SamplerState*> m_SamplerStateList;
	std::unordered_map<Hash_Code, ViewPort*> m_ViewPortList;
};