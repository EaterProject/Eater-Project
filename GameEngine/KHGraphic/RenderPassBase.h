#pragma once

interface IShaderManager;
interface IFactoryManager;
interface IGraphicResourceManager;

class GlobalData;
class RenderOption;

///
/// 2021/11/08 2:22
/// SeoKyuHwang
///
/// # RenderBase Class
///
/// - ���� Rendering Class���� �ش� Rendering�� �´� GraphicResource ������ ����
///   ���� Manager & DeviceContext�� �������� �ΰ� ���
/// 
class RenderPassBase
{
public:
	RenderPassBase() = default;
	virtual ~RenderPassBase() = default;

public:
	virtual void Create(int width, int height) abstract;
	virtual void Start(int width, int height) abstract;

	virtual void OnResize(int width, int height) abstract;
	virtual void Release() abstract;

	virtual void SetResize(int width, int height) {}
	virtual void SetOption(RenderOption* renderOption) {}

public:
	static void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, IFactoryManager* factory, IGraphicResourceManager* resource, IShaderManager* shader);
	static void GraphicReset();
	static void Reset();

public:
	friend class RenderManager;

protected:
	static Microsoft::WRL::ComPtr<ID3D11Device> g_Device;
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext> g_Context;

	static IFactoryManager* g_Factory;
	static IGraphicResourceManager* g_Resource;
	static IShaderManager* g_Shader;

	static GlobalData* g_GlobalData;
};