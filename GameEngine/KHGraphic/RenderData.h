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

	Matrix* m_Tex;

	ID3D11ShaderResourceView* m_Albedo;
	ID3D11ShaderResourceView* m_Normal;
	ID3D11ShaderResourceView* m_Emissive;
	ID3D11ShaderResourceView* m_ORM;
};

class MeshRenderData
{
public:
	UINT m_IndexCount;
	UINT m_Stride;
	UINT m_Offset;

	ID3D11Buffer* m_VertexBuf;
	ID3D11Buffer* m_IndexBuf;

	MaterialRenderData* m_Material;
	std::vector<MaterialRenderData*> m_MaterialList;
};

class RenderData
{
public:
	RenderData();
	~RenderData();

public:
	void ConvertData(MeshData* originMesh);

	void Release();

private:
	void ConvertIndexBuffer(IndexBuffer* originBuf);
	void ConvertVertexBuffer(VertexBuffer* originBuf);
	void ConvertMaterial(MaterialData* originMat);
	void ConvertMaterial(MaterialData* originMat, MaterialRenderData* convertMat);
	
public:
	OBJECT_TYPE m_ObjectType;
	bool m_Alpha;

	UINT m_MeshIndex;

	Matrix* m_World;

	std::vector<Matrix>* m_BoneOffsetTM;

	void* m_OriginData;				// Engine Origin Data..
	MeshRenderData* m_MeshData;		// º¯È¯µÈ Mesh Data..

	ParticleData* m_ParticleData;
	ColliderData* m_ColliderData;
};