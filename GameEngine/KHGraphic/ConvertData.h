#pragma once
// Render List Layer 관리를 위한 Class
class MeshIndexData
{
public:
	UINT m_MeshIndex;
	UINT m_ListIndex;
};

// Game Engine 측 Data와 대응하는 DirectX 11 전용 Material Data Class
class MaterialRenderBuffer
{
public:
	UINT m_MaterialIndex;

	MaterialSubData* m_MaterialSubData;

	ID3D11ShaderResourceView* m_Albedo;
	ID3D11ShaderResourceView* m_Normal;
	ID3D11ShaderResourceView* m_Emissive;
	ID3D11ShaderResourceView* m_ORM;
};

// Game Engine 측 Data와 대응하는 DirectX 11 전용 Mesh Data Class
class MeshRenderBuffer
{
public:
	UINT m_BufferIndex;

	UINT m_IndexCount;
	UINT m_Stride;
	UINT m_Offset;

	ID3D11Buffer* m_VertexBuf;
	ID3D11Buffer* m_IndexBuf;
};

class TerrainRenderData
{
public:
	std::vector<MaterialRenderBuffer*> m_MaterialList;
};