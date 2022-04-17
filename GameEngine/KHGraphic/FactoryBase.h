#pragma once
#include <windows.h>

typedef size_t Hash_Code;

interface ID3D11Graphic;
interface IShaderManager;
interface IGraphicResourceManager;

class FactoryBase
{
public:
	FactoryBase() = default;
	virtual ~FactoryBase() = default;

public:
	virtual void Create(int width, int height) abstract;
	virtual void Start() abstract;
	virtual void Release() abstract;

public:
	static void Initialize(ID3D11Graphic* graphic, IGraphicResourceManager* resource, IShaderManager* shader);
	static void Reset();

protected:
	static ID3D11Graphic* g_Graphic;
	static IShaderManager* g_ShaderManager;
	static IGraphicResourceManager* g_ResourceManager;
};

