#pragma once
class MeshIndexData
{
public:
	UINT m_MeshIndex;
	UINT m_ListIndex;
};

class MaterialRenderData
{
public:
	UINT m_MaterialIndex;

	MaterialSubData* m_MaterialSubData;

	ID3D11ShaderResourceView* m_Albedo;
	ID3D11ShaderResourceView* m_Normal;
	ID3D11ShaderResourceView* m_Emissive;
	ID3D11ShaderResourceView* m_ORM;
};

class MeshRenderData
{
public:
	UINT m_BufferIndex;

	UINT m_IndexCount;
	UINT m_Stride;
	UINT m_Offset;

	ID3D11Buffer* m_VertexBuf;
	ID3D11Buffer* m_IndexBuf;

	MaterialRenderData* m_Material;
};

class TerrainRenderData
{
public:
	std::vector<MaterialRenderData*> m_MaterialList;
};