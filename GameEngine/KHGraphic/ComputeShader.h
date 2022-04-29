#pragma once

/// <summary>
/// ComputeShader Class
/// </summary>
/// 
/// - �Ѱ��� ComputeShader�� �����ϴ� Ŭ����

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

	// ComputeShader UnorderedAccessView ����..
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

	// ���ӵ� �޸� ������ �ִ� UnorderedAccessView List..
	std::vector<Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>> m_UnorderedAccessViews;

	// ComputeShader UnorderedAccessView List..
	std::unordered_map<Hash_Code, UnorderedAccessBuffer*> m_UAV_List;
};

template<typename T>
void ComputeShader::SetUnorderedAccessView(ID3D11UnorderedAccessView* uav)
{
	// �ش� Value ã��..
	std::unordered_map<Hash_Code, UnorderedAccessBuffer*>::iterator it = m_UAV_List.find(typeid(T).hash_code());

	// �ش� Key�� ���� Value�� ���ٸ�..
	if (it == m_UAV_List.end()) return;

	// �ش� Register Slot�� ����..
	m_UnorderedAccessViews[it->second->register_number] = uav;
}