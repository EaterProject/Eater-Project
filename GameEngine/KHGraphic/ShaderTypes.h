#pragma once

/// <summary>
/// �������� Ÿ���� �Ѱ��� �Լ��� ��ȯ�ϱ� ���� ���� Class
/// �ش� Class�� Type�� �ݿ��Ͽ� ���� Class Operator�� ���� �������� Ÿ������ ��ȯ
/// </summary>

class ShaderManager;

class ShaderBase;
class VertexShader;
class PixelShader;
class ComputeShader;

class OriginalShader
{
public:
	ShaderManager* pThis;
	std::string shaderName;

public:
	operator ShaderBase*();
	operator VertexShader*();
	operator PixelShader*();
	operator ComputeShader*();
};
