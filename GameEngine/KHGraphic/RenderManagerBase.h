#pragma once
#include <queue>
#include "EngineData.h"

///
/// 2021/11/12 18:02
/// SeoKyuHwang
///
/// RenderManger Interface Class
///
/// - ��� RenderPass�� �����ϴ� Class
/// - �ܺ� Game Engine���� ��û�� RenderPass�� �����ϴ� Class
/// - ���������� Render��Ŀ� ���� ������ ���ش�.
/// 

interface IRenderManager
{
public:
	virtual void Create(int width, int height) abstract;
	virtual void Start(int width, int height) abstract;
	virtual void Release() abstract;

	// Resize Resource..
	virtual void OnResize(int width, int hegiht) abstract;

public:
	// Render Option Setting..
	virtual void RenderSetting(RenderOption* renderOption) abstract;

	// Global Data Setting..
	virtual void SetGlobalData(GlobalData* globalData) abstract;

	// Environment Map Setting..
	virtual void SetEnvironmentMap(bool enable) abstract;

public:
	// Render Data Add & Delete..
	virtual void AddMeshData(MeshData* meshData) abstract;
	virtual void DeleteMeshData(MeshData* meshData) abstract;
	virtual void ConvertMeshData() abstract;

	// Render Update..
	virtual void Render() abstract;

};