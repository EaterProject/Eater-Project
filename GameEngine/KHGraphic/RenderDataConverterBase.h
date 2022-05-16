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
	virtual void ConvertMeshData(MeshData* originData, RenderData* renderData) abstract;	// Mesh Data -> Render Data 변환..
	virtual void ConvertRenderData(MeshData* originData, RenderData* renderData) abstract;	// Mesh Data -> Render Data 변환..

	virtual void ResourceUpdate() abstract;		// 현재 프레임에 생성 & 삽입 Resource Update..

	virtual void Release() abstract;			// Release 함수..

public:
	virtual void PushMesh(MeshBuffer* mesh) abstract;						// 현재 프레임에 생성된 Mesh 임시 Queue에 삽입..
	virtual void PushMaterial(MaterialBuffer* material) abstract;			// 현재 프레임에 생성된 Material 임시 Queue에 삽입..
	virtual void PushAnimation(AnimationBuffer* animation) abstract;		// 현재 프레임에 생성된 Animation 임시 Queue에 삽입..

public:
	virtual void PushChangeMesh(MeshBuffer* mesh) abstract;					// 현재 프레임에 변경된 Mesh 임시 Queue에 삽입..
	virtual void PushChangeMaterial(MaterialBuffer* material) abstract;		// 현재 프레임에 변경된 Material 임시 Queue에 삽입..
	virtual void PushChangeAnimation(AnimationBuffer* animation) abstract;	// 현재 프레임에 생성된 Animation 임시 Queue에 삽입..

public:
	virtual void DeleteRenderData(UINT index) abstract;						// 해당 Render Data 즉시 제거..
	virtual void DeleteInstance(UINT index) abstract;						// 해당 Instance Resource 즉시 제거..
	virtual void DeleteMesh(UINT index) abstract;							// 해당 Mesh Resource 즉시 제거..
	virtual void DeleteMaterial(UINT index) abstract;						// 해당 Material Resource 즉시 제거..
	virtual void DeleteAnimation(UINT index) abstract;						// 해당 Animation Resource 즉시 제거..

public:
	virtual size_t FindMaxInstanceCount() abstract;							// 모든 Layer 내부의 Instance 개수 중 제일 큰 개수 반환..

public:
	virtual RenderData* GetRenderData(int index) abstract;					// Render Data 고유 Index로 검색..
	virtual MeshRenderBuffer* GetMesh(int index) abstract;					// Mesh Resource 고유 Index로 검색..
	virtual MaterialRenderBuffer* GetMaterial(int index) abstract;			// Material Resource 고유 Index로 검색..
	virtual AnimationRenderBuffer* GetAnimation(int index) abstract;		// Animation Resource 고유 Index로 검색..
	virtual InstanceRenderBuffer* GetInstance(int index) abstract;			// Instance Resource 고유 Index로 검색..
	virtual InstanceLayer* GetLayer(int index) abstract;					// Layer Resource 고유 Index로 검색..
};