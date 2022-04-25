#pragma once
class RenderResource
{
public:
	std::string m_Name;
};

// Game Engine 측 MaterialBuffer와 대응하는 DirectX 11 전용 Material Data Class
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

// Game Engine 측 MeshBuffer와 대응하는 DirectX 11 전용 Mesh Data Class
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

// Game Engine 측 AnimationBuffer와 대응하는 DirectX 11 전용 Animation Data Class
class AnimationRenderBuffer : public RenderResource
{
public:
	UINT m_BufferIndex;

	UINT m_FrameOffset;
	std::vector<UINT> m_AnimationOffset;

	ID3D11ShaderResourceView* m_AnimationBuf;
};

// Game Engine 측 TerrainBuffer와 대응하는 DirectX 11 전용 Terrain Data Class
class TerrainRenderBuffer : public RenderResource
{
public:
	Matrix* m_Tex;
	std::vector<MaterialRenderBuffer*> m_MaterialList;
};

// Instance Layer를 관리하기 위한 Render Buffer Class
class InstanceRenderBuffer : public RenderResource
{
public:
	UINT m_BufferIndex = 0;

	UINT m_Type = 0;						// Instance Object Type.. 아마 Shader Type으로 바뀔 것 이다..

	MeshRenderBuffer*		m_Mesh;			// Instance의 기준이 되는 Mesh Buffer..
	MaterialRenderBuffer*	m_Material;		// Instance의 기준이 되는 Material Buffer..
	AnimationRenderBuffer*	m_Animation;	// Instance의 기준이 되는 Animation Buffer..
};
