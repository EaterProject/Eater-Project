#pragma once
#include "EnumDefine.h"
#include <string>

typedef size_t Hash_Code;

/// 2021/12/08 2:06
/// SeoKyuHwang
///
/// # ResourceBase Class
///
/// - Graphic Resource Base Class
/// 
#pragma region ResourceBase
class ResourceBase
{
public:
	ResourceBase(RESOURCE_TYPE type, Hash_Code hashCode) : m_ResourceType(type), m_HashCode(hashCode) {}

public:
	virtual void Reset() abstract;
	virtual void Release() abstract;

public:
	void SetName(const char* name) { m_ResourceName = name; }

	RESOURCE_TYPE GetType() { return m_ResourceType; }
	Hash_Code GetHashCode() { return m_HashCode; }

private:
	std::string m_ResourceName;

	RESOURCE_TYPE m_ResourceType;
	Hash_Code m_HashCode;
};
#pragma endregion ResourceBase

/// 2022/02/10 12:20
/// SeoKyuHwang
///
/// # GraphicResource Class
///
/// - Rendering에 필요한 Graphic Resource Base Class
/// 
#pragma region GraphicResource
template<typename T>
class GraphicResource : public ResourceBase
{
public:
	GraphicResource(RESOURCE_TYPE type, Hash_Code hashCode, T* resource);
	virtual ~GraphicResource();

public:
	void Reset();
	void Release();

public:
	T* Get();
	T** GetAddress();
	T** ReleaseGetAddress();

protected:
	Microsoft::WRL::ComPtr<T> m_Resource;
};
#pragma endregion GraphicResource


/// 2022/02/13 18:40
/// SeoKyuHwang
///
/// # GraphicView Class
///
/// - Rendering에 필요한 Graphic View Base Class
/// 
#pragma region GraphicView
template<typename T>
class GraphicView : public ResourceBase
{
public:
	GraphicView(RESOURCE_TYPE type, Hash_Code hashCode, T* resource);
	virtual ~GraphicView();

public:
	void Reset();
	void Release();

public:
	void SetViewSize(UINT width);
	void SetViewSize(UINT width, UINT height);
	void SetViewSize(UINT width, UINT height, UINT depth);
	void SetNumElements(UINT numElements);

public:
	void GetViewPort(D3D11_VIEWPORT* viewPort);

public:
	T* Get();
	T** GetAddress();
	T** ReleaseGetAddress();

protected:
	Microsoft::WRL::ComPtr<T> m_Resource;
	UINT m_ViewWidth;
	UINT m_ViewHeight;
	UINT m_ViewDepth;
	UINT m_NumElements;
};
#pragma endregion GraphicView


/// 2022/02/13 18:40
/// SeoKyuHwang
///
/// # GraphicState Class
///
/// - Rendering에 필요한 Graphic State Base Class
/// 
#pragma region GraphicState
template<typename T>
class GraphicState : public ResourceBase
{
public:
	GraphicState(RESOURCE_TYPE type, Hash_Code hashCode, T* resource);
	virtual ~GraphicState();

public:
	void Reset();
	void Release();

public:
	T* Get();
	T** GetAddress();
	T** ReleaseGetAddress();

protected:
	Microsoft::WRL::ComPtr<T> m_Resource;
};
#pragma endregion GraphicState

#include "ResourceBase.inl"
