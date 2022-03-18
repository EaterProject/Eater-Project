#pragma once
// Render List Layer ������ ���� Class
class MeshIndexData
{
public:
	UINT m_MeshIndex;
	UINT m_ListIndex;
};

// Game Engine �� Data�� �����ϴ� DirectX 11 ���� Material Data Class
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

// Game Engine �� Data�� �����ϴ� DirectX 11 ���� Mesh Data Class
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

// Instance Layer�� �����ϱ� ���� Render Buffer Class
class InstanceRenderBuffer
{
public:
	UINT m_InstanceIndex = 0;

	MeshRenderBuffer* m_Mesh;			// Instance�� ������ �Ǵ� Mesh Buffer..
	MaterialRenderBuffer* m_Material;	// Instance�� ������ �Ǵ� Material Buffer..
};

class TerrainRenderData
{
public:
	std::vector<MaterialRenderBuffer*> m_MaterialList;
};