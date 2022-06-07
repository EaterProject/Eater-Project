#pragma once
class RenderResource
{
public:
	std::string m_Name;
};

// Game Engine �� Material Buffer�� �����ϴ� DirectX 11 ���� Material Data Class
class MaterialRenderBuffer : public RenderResource
{
public:
	UINT m_BufferIndex;
	
	MaterialProperty* m_MaterialProperty;

	ID3D11ShaderResourceView* m_Albedo;
	ID3D11ShaderResourceView* m_Normal;
	ID3D11ShaderResourceView* m_Emissive;
	ID3D11ShaderResourceView* m_ORM;
};

// Game Engine �� Mesh Buffer�� �����ϴ� DirectX 11 ���� Mesh Data Class
class MeshRenderBuffer : public RenderResource
{
public:
	UINT m_BufferIndex;

	UINT m_IndexCount;
	UINT m_Stride;
	UINT m_Offset;

	ID3D11Buffer* m_VertexBuf;
	ID3D11Buffer* m_IndexBuf;

	MeshProperty* m_MeshProperty;
};

// Game Engine �� Animation Buffer�� �����ϴ� DirectX 11 ���� Animation Data Class
class AnimationRenderBuffer : public RenderResource
{
public:
	UINT m_BufferIndex;

	UINT m_FrameOffset;
	std::vector<UINT> m_AnimationOffset;

	ID3D11ShaderResourceView* m_AnimationBuf;
};

// Game Engine �� Terrain Buffer�� �����ϴ� DirectX 11 ���� Terrain Data Class
class TerrainRenderBuffer : public RenderResource
{
public:
	Matrix* m_Tex;
	std::vector<MaterialRenderBuffer*> m_MaterialList;
};

// Game Engine �� UI Buffer�� �����ϴ� DirectX 11 ���� UI Data Class
class UIRenderBuffer : public RenderResource
{
public:
	UINT m_BufferIndex;
	UINT m_BufferLayer;

	UIProperty* m_UIProperty;

	ID3D11ShaderResourceView* m_Albedo;
};

// Instance Layer�� �����ϱ� ���� Render Buffer Class
class InstanceRenderBuffer : public RenderResource
{
public:
	UINT m_BufferIndex = 0;

	UINT m_Type = 0;						// Instance Object Type.. �Ƹ� Shader Type���� �ٲ� �� �̴�..

	MeshRenderBuffer*		m_Mesh;			// Instance�� ������ �Ǵ� Mesh Buffer..
	MaterialRenderBuffer*	m_Material;		// Instance�� ������ �Ǵ� Material Buffer..
	AnimationRenderBuffer*	m_Animation;	// Instance�� ������ �Ǵ� Animation Buffer..
};