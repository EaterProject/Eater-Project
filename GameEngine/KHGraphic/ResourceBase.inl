#pragma once

#pragma region GraphicResource
template<typename T>
GraphicResource<T>::GraphicResource(RESOURCE_TYPE type, Hash_Code hashCode, T* resource)
	:ResourceBase(type, hashCode), m_Resource(resource)
{

}

template<typename T>
GraphicResource<T>::~GraphicResource()
{
	Release();
}

template<typename T>
void GraphicResource<T>::Reset()
{
	m_Resource.Reset();
}

template<typename T>
void GraphicResource<T>::Release()
{
	RELEASE_COM(m_Resource);
}

template<typename T>
T* GraphicResource<T>::Get()
{
	return m_Resource.Get();
}

template<typename T>
T** GraphicResource<T>::GetAddress()
{
	return m_Resource.GetAddressOf();
}

template<typename T>
T** GraphicResource<T>::ReleaseGetAddress()
{
	return m_Resource.ReleaseAndGetAddressOf();
}
#pragma endregion GraphicResource

#pragma region GraphicView
template<typename T>
GraphicView<T>::GraphicView(RESOURCE_TYPE type, Hash_Code hashCode, T* resource)
	:ResourceBase(type, hashCode), m_Resource(resource)
{
	// Resource 추출..
	ID3D11Resource* baseResource = nullptr;
	resource->GetResource(&baseResource);

	// Resource Dimension 체크..
	D3D11_RESOURCE_DIMENSION dimension;
	baseResource->GetType(&dimension);

	// Resource Type..
	switch (dimension)
	{
	case D3D11_RESOURCE_DIMENSION_UNKNOWN:
		break;
	case D3D11_RESOURCE_DIMENSION_BUFFER:
	{

	}
	break;
	case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
	{
		ID3D11Texture1D* tex1D = (ID3D11Texture1D*)baseResource;
		D3D11_TEXTURE1D_DESC texDesc;
		tex1D->GetDesc(&texDesc);

		SetViewSize(texDesc.Width);
	}
	break;
	case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
	{
		ID3D11Texture2D* tex2D = (ID3D11Texture2D*)baseResource;
		D3D11_TEXTURE2D_DESC texDesc;
		tex2D->GetDesc(&texDesc);

		SetViewSize(texDesc.Width, texDesc.Height);
	}
	break;
	case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
	{
		ID3D11Texture3D* tex3D = (ID3D11Texture3D*)baseResource;
		D3D11_TEXTURE3D_DESC texDesc;
		tex3D->GetDesc(&texDesc);

		SetViewSize(texDesc.Width, texDesc.Height, texDesc.Depth);
	}
	break;
	default:
		break;
	}

	baseResource->Release();
}

template<typename T>
GraphicView<T>::~GraphicView()
{
	Release();
}

template<typename T>
void GraphicView<T>::Reset()
{
	m_Resource.Reset();
}

template<typename T>
void GraphicView<T>::Release()
{
	RELEASE_COM(m_Resource);
}

template<typename T>
void GraphicView<T>::SetViewSize(UINT width)
{
	m_ViewWidth = width;
}

template<typename T>
void GraphicView<T>::SetViewSize(UINT width, UINT height)
{
	m_ViewWidth = width;
	m_ViewHeight = height;
}

template<typename T>
void GraphicView<T>::SetViewSize(UINT width, UINT height, UINT depth)
{
	m_ViewWidth = width;
	m_ViewHeight = height;
	m_ViewDepth = depth;
}

template<typename T>
void GraphicView<T>::SetNumElements(UINT numElements)
{
	m_NumElements = numElements;
}

template<typename T>
void GraphicView<T>::GetViewPort(D3D11_VIEWPORT* viewPort)
{
	viewPort->Width = m_ViewWidth;
	viewPort->Height = m_ViewHeight;
}

template<typename T>
T* GraphicView<T>::Get()
{
	return m_Resource.Get();
}

template<typename T>
T** GraphicView<T>::GetAddress()
{
	return m_Resource.GetAddressOf();
}

template<typename T>
T** GraphicView<T>::ReleaseGetAddress()
{
	return m_Resource.ReleaseAndGetAddressOf();
}
#pragma endregion GraphicView

#pragma region GraphicState
template<typename T>
GraphicState<T>::GraphicState(RESOURCE_TYPE type, Hash_Code hashCode, T* resource)
	:ResourceBase(type, hashCode), m_Resource(resource)
{

}

template<typename T>
GraphicState<T>::~GraphicState()
{
	Release();
}

template<typename T>
void GraphicState<T>::Reset()
{
	m_Resource.Reset();
}

template<typename T>
void GraphicState<T>::Release()
{
	RELEASE_COM(m_Resource);
}

template<typename T>
T* GraphicState<T>::Get()
{
	return m_Resource.Get();
}

template<typename T>
T** GraphicState<T>::GetAddress()
{
	return m_Resource.GetAddressOf();
}

template<typename T>
T** GraphicState<T>::ReleaseGetAddress()
{
	return m_Resource.ReleaseAndGetAddressOf();
}
#pragma endregion GraphicState
