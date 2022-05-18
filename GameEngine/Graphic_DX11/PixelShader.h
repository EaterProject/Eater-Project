#pragma once

/// <summary>
/// PixelShader Class
/// </summary>
/// 
/// - �Ѱ��� PixelShader�� �����ϴ� Ŭ����

class PixelShader : public ShaderBase
{
public:
	PixelShader(const char* shaderName, const char* fileName, const char* entry_point, const char* shader_model, const D3D_SHADER_MACRO* pDefines);
	~PixelShader();

public:
	void LoadShader(std::string fileName, const char* entry_point, const char* shader_model, const D3D_SHADER_MACRO* pDefines) override;
	void Update() override;
	void Update(ID3D11DeviceContext* context) override;
	void Release() override;

public:
	static void UnBindConstantBuffer(UINT startSlot, UINT numViews);
	static void UnBindShaderResourceView(UINT startSlot, UINT numViews);

private:
	// Pixel Shader
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS;
};