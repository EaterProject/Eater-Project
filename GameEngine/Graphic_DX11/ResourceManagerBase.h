#pragma once
#include <assert.h>
#include "ResourceBase.h"
#include "HashBase.h"

typedef size_t Hash_Code;

class DepthStencil;
class RenderTexture;
class RenderBuffer;
class DrawBuffer;
class InstanceBuffer;

class RenderTargetView;
class DepthStencilView;
class ShaderResourceView;
class UnorderedAccessView;

class DepthStencilState;
class BlendState;
class RasterizerState;
class ViewPort;

///
/// 2021/11/07 23:02
/// SeoKyuHwang
///
/// # GraphicResourceManager Interface Class
///
/// - 葛电 GraphicResource甫 包府窍绰 Class
/// - GraphicResourceFactory俊辑 积己等 Resource 包府
/// 
interface IGraphicResourceManager
{
public:
	virtual void Create() abstract;
	virtual void Start() abstract;
	virtual void OnResize(int width, int height) abstract;
	virtual void Release() abstract;

public:
	friend class GraphicResourceFactory;

public:
	// BackBuffer RenderTarget Get Function
	virtual RenderTexture* GetMainRenderTarget() abstract;
	// BackBuffer RenderTarget Add Function
	virtual void AddMainRenderTarget(RenderTexture* rtv) abstract;

public:
	// DepthStencil Get Function
	template<typename T, Enable_Check<T> = NULL> DepthStencil* GetDepthStencil();
	// RenderTarget Get Function
	template<typename T, Enable_Check<T> = NULL> RenderTexture* GetRenderTexture();
	// RenderBuffer Get Function
	template<typename T, Enable_Check<T> = NULL> RenderBuffer* GetRenderBuffer();
	// DrawBuffer Get Function
	template<typename T, Enable_Check<T> = NULL> DrawBuffer* GetDrawBuffer();
	// DrawBuffer Get Function
	template<typename T, Enable_Check<T> = NULL> InstanceBuffer* GetInstanceBuffer();

public:
	// DepthStencilView Get Function
	template<typename T, Enable_Check<T> = NULL> DepthStencilView* GetDepthStencilView();
	// RenderTargetView Get Function
	template<typename T, Enable_Check<T> = NULL> RenderTargetView* GetRenderTargetView();
	// ShaderResourceView Get Function
	template<typename T, Enable_Check<T> = NULL> ShaderResourceView* GetShaderResourceView();
	// UnorderedAccessView Get Function
	template<typename T, Enable_Check<T> = NULL> UnorderedAccessView* GetUnorderedAccessView();

public:
	// DepthStencilState Get Function
	template<typename T, Enable_Check<T> = NULL> DepthStencilState* GetDepthStencilState();
	// RasterizerState Get Function
	template<typename T, Enable_Check<T> = NULL> RasterizerState* GetRasterizerState();
	// BlendState Get Function
	template<typename T, Enable_Check<T> = NULL> BlendState* GetBlendState();
	// ViewPort Get Function
	template<typename T, Enable_Check<T> = NULL> ViewPort* GetViewPort();

public:
	// Resource Add Function
	template<typename T, Enable_Check<T> = NULL> void AddResource(ResourceBase* resource);
	// Resource Delete Function
	template<typename T, Enable_Check<T> = NULL> void DeleteResource();

private:
	// Graphic Struct Resource Getter..
	virtual DepthStencil* GetDepthStencil(Hash_Code hash_code) abstract;
	virtual RenderTexture* GetRenderTexture(Hash_Code hash_code) abstract;
	virtual RenderBuffer* GetRenderBuffer(Hash_Code hash_code) abstract;
	virtual DrawBuffer* GetDrawBuffer(Hash_Code hash_code) abstract;
	virtual InstanceBuffer* GetInstanceBuffer(Hash_Code hash_code) abstract;

private:
	// Graphic View Resource Getter..
	virtual RenderTargetView* GetRenderTargetView(Hash_Code hash_code) abstract;
	virtual DepthStencilView* GetDepthStencilView(Hash_Code hash_code) abstract;
	virtual ShaderResourceView* GetShaderResourceView(Hash_Code hash_code) abstract;
	virtual UnorderedAccessView* GetUnorderedAccessView(Hash_Code hash_code) abstract;

private:
	// Graphic State Resource Getter..
	virtual BlendState* GetBlendState(Hash_Code hash_code) abstract;
	virtual RasterizerState* GetRasterizerState(Hash_Code hash_code) abstract;
	virtual DepthStencilState* GetDepthStencilState(Hash_Code hash_code) abstract;
	virtual ViewPort* GetViewPort(Hash_Code hash_code) abstract;

private:
	virtual void AddResource(Hash_Code hash_code, ResourceBase* resource) abstract;
	virtual void DeleteResource(Hash_Code hash_code, RESOURCE_TYPE resource_type) abstract;
};

template<typename T, Enable_Check<T>>
inline DepthStencil* IGraphicResourceManager::GetDepthStencil()
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::DS);

	return GetDepthStencil(T::GetHashCode());
}

template<typename T, Enable_Check<T>>
inline RenderTexture* IGraphicResourceManager::GetRenderTexture()
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::RT);
	
	return GetRenderTexture(T::GetHashCode());
}

template<typename T, Enable_Check<T>>
inline RenderBuffer* IGraphicResourceManager::GetRenderBuffer()
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::RB);

	return GetRenderBuffer(T::GetHashCode());
}

template<typename T, Enable_Check<T>>
inline DrawBuffer* IGraphicResourceManager::GetDrawBuffer()
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::DB);

	return GetDrawBuffer(T::GetHashCode());
}

template<typename T, Enable_Check<T>>
inline InstanceBuffer* IGraphicResourceManager::GetInstanceBuffer()
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::IB);

	return GetInstanceBuffer(T::GetHashCode());
}

template<typename T, Enable_Check<T>>
inline DepthStencilView* IGraphicResourceManager::GetDepthStencilView()
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::DSV || T::GetType() == RESOURCE_TYPE::DS);

	return GetDepthStencilView(T::GetHashCode());
}

template<typename T, Enable_Check<T>>
inline RenderTargetView* IGraphicResourceManager::GetRenderTargetView()
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::RTV || T::GetType() == RESOURCE_TYPE::RT);
	
	return GetRenderTargetView(T::GetHashCode());
}

template<typename T, Enable_Check<T>>
inline ShaderResourceView* IGraphicResourceManager::GetShaderResourceView()
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::SRV || T::GetType() == RESOURCE_TYPE::RT || T::GetType() == RESOURCE_TYPE::DS);

	return GetShaderResourceView(T::GetHashCode());
}

template<typename T, Enable_Check<T>>
inline UnorderedAccessView* IGraphicResourceManager::GetUnorderedAccessView()
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::UAV || T::GetType() == RESOURCE_TYPE::RT);

	return GetUnorderedAccessView(T::GetHashCode());
}

template<typename T, Enable_Check<T>>
inline BlendState* IGraphicResourceManager::GetBlendState()
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::BS);
	
	return GetBlendState(T::GetHashCode());
}

template<typename T, Enable_Check<T>>
inline RasterizerState* IGraphicResourceManager::GetRasterizerState()
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::RS);
	
	return GetRasterizerState(T::GetHashCode());
}

template<typename T, Enable_Check<T>>
inline DepthStencilState* IGraphicResourceManager::GetDepthStencilState()
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::DSS);
	
	return GetDepthStencilState(T::GetHashCode());
}

template<typename T, Enable_Check<T>>
inline ViewPort* IGraphicResourceManager::GetViewPort()
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::VP);
	
	return GetViewPort(T::GetHashCode());
}

template<typename T, Enable_Check<T>>
inline void IGraphicResourceManager::AddResource(ResourceBase* resource)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == resource->GetType());
	
	AddResource(T::GetHashCode(), resource);
}

template<typename T, Enable_Check<T>>
inline void IGraphicResourceManager::DeleteResource()
{
	DeleteResource(T::GetHashCode(), T::GetType());
}
