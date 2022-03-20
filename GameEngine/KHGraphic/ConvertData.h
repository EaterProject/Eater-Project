#pragma once
// Game Engine �� MaterialBuffer�� �����ϴ� DirectX 11 ���� Material Data Class
class MaterialRenderBuffer
{
public:
	MaterialSubData* m_MaterialSubData;

	ID3D11ShaderResourceView* m_Albedo;
	ID3D11ShaderResourceView* m_Normal;
	ID3D11ShaderResourceView* m_Emissive;
	ID3D11ShaderResourceView* m_ORM;
};

// Game Engine �� MeshBuffer�� �����ϴ� DirectX 11 ���� Mesh Data Class
class MeshRenderBuffer
{
public:
	UINT m_IndexCount;
	UINT m_Stride;
	UINT m_Offset;

	ID3D11Buffer* m_VertexBuf;
	ID3D11Buffer* m_IndexBuf;
};

// Game Engine �� TerrainBuffer�� �����ϴ� DirectX 11 ���� Terrain Data Class
class TerrainRenderBuffer
{
public:
	Matrix* m_Tex;
	std::vector<MaterialRenderBuffer*> m_MaterialList;
};

// Instance Layer�� �����ϱ� ���� Render Buffer Class
class InstanceRenderBuffer
{
public:
	UINT m_BufferIndex = 0;

	UINT m_Type = 0;					// Instance Object Type.. �Ƹ� Shader Type���� �ٲ� �� �̴�..

	MeshRenderBuffer* m_Mesh;			// Instance�� ������ �Ǵ� Mesh Buffer..
	MaterialRenderBuffer* m_Material;	// Instance�� ������ �Ǵ� Material Buffer..
};
