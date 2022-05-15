#pragma once

/// <summary>
/// ComputeShader Class
/// </summary>
/// 
/// - 한개의 ComputeShader를 관리하는 클래스

class ComputeShader : public ShaderBase
{
public:
	ComputeShader(const char* shaderName, const char* fileName, const char* entry_point, const char* shader_model, const D3D_SHADER_MACRO* pDefines);
	~ComputeShader();

public:
	void LoadShader(std::string fileName, const char* entry_point, const char* shader_model, const D3D_SHADER_MACRO* pDefines) override;
	void Update() override;
	void Update(ID3D11DeviceContext* context) override;
	void Release() override;

	// ComputeShader UnorderedAccessView 설정..
	template<typename T>
	void SetUnorderedAccessView(ID3D11UnorderedAccessView* uav);

public:
	static void UnBindComputeShader();
	static void UnBindConstantBuffer(UINT startSlot, UINT numViews);
	static void UnBindShaderResourceView(UINT startSlot, UINT numViews);
	static void UnBindUnorderedAccessView(UINT startSlot, UINT numViews);

private:
	// Compute Shader
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_CS;

	// 연속된 메모리 공간에 있는 UnorderedAccessView List..
	std::vector<Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>> m_UnorderedAccessViews;

	// ComputeShader UnorderedAccessView List..
	std::unordered_map<Hash_Code, UnorderedAccessBuffer*> m_UAV_List;
};

template<typename T>
void ComputeShader::SetUnorderedAccessView(ID3D11UnorderedAccessView* uav)
{
	// 해당 Value 찾기..
	std::unordered_map<Hash_Code, UnorderedAccessBuffer*>::iterator it = m_UAV_List.find(typeid(T).hash_code());

	// 해당 Key에 대한 Value가 없다면..
	if (it == m_UAV_List.end()) return;

	// 해당 Register Slot에 삽입..
	m_UnorderedAccessViews[it->second->register_number] = uav;
}