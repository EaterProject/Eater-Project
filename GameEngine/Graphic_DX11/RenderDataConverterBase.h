#pragma once
#include <windows.h>

class MeshData;
class RenderData;

class MeshBuffer;
class MaterialBuffer;
class AnimationBuffer;

class MeshRenderBuffer;
class MaterialRenderBuffer;
class AnimationRenderBuffer;
class TerrainRenderBuffer;
class InstanceRenderBuffer;
class InstanceLayer;

interface IRenderDataConverter
{
public:
	virtual void ConvertMeshData(MeshData* originData, RenderData* renderData) abstract;	// Mesh Data -> Render Data ��ȯ..
	virtual void ConvertRenderData(MeshData* originData, RenderData* renderData) abstract;	// Mesh Data -> Render Data ��ȯ..

	virtual void ResourceUpdate() abstract;		// ���� �����ӿ� ���� & ���� Resource Update..

	virtual void Release() abstract;			// Release �Լ�..

public:
	virtual void PushMesh(MeshBuffer* mesh) abstract;						// ���� �����ӿ� ������ Mesh �ӽ� Queue�� ����..
	virtual void PushMaterial(MaterialBuffer* material) abstract;			// ���� �����ӿ� ������ Material �ӽ� Queue�� ����..
	virtual void PushAnimation(AnimationBuffer* animation) abstract;		// ���� �����ӿ� ������ Animation �ӽ� Queue�� ����..

public:
	virtual void PushChangeMesh(MeshBuffer* mesh) abstract;					// ���� �����ӿ� ����� Mesh �ӽ� Queue�� ����..
	virtual void PushChangeMaterial(MaterialBuffer* material) abstract;		// ���� �����ӿ� ����� Material �ӽ� Queue�� ����..
	virtual void PushChangeAnimation(AnimationBuffer* animation) abstract;	// ���� �����ӿ� ������ Animation �ӽ� Queue�� ����..

public:
	virtual void DeleteRenderData(UINT index) abstract;						// �ش� Render Data ��� ����..
	virtual void DeleteInstance(UINT index) abstract;						// �ش� Instance Resource ��� ����..
	virtual void DeleteMesh(UINT index) abstract;							// �ش� Mesh Resource ��� ����..
	virtual void DeleteMaterial(UINT index) abstract;						// �ش� Material Resource ��� ����..
	virtual void DeleteAnimation(UINT index) abstract;						// �ش� Animation Resource ��� ����..

public:
	virtual size_t FindMaxInstanceCount() abstract;							// ��� Layer ������ Instance ���� �� ���� ū ���� ��ȯ..

public:
	virtual RenderData* GetRenderData(int index) abstract;					// Render Data ���� Index�� �˻�..
	virtual MeshRenderBuffer* GetMesh(int index) abstract;					// Mesh Resource ���� Index�� �˻�..
	virtual MaterialRenderBuffer* GetMaterial(int index) abstract;			// Material Resource ���� Index�� �˻�..
	virtual AnimationRenderBuffer* GetAnimation(int index) abstract;		// Animation Resource ���� Index�� �˻�..
	virtual InstanceRenderBuffer* GetInstance(int index) abstract;			// Instance Resource ���� Index�� �˻�..
	virtual InstanceLayer* GetLayer(int index) abstract;					// Layer Resource ���� Index�� �˻�..
};