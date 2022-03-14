#pragma once

/// <summary>
/// �������� Ÿ���� �Ѱ��� �Լ��� ��ȯ�ϱ� ���� ���� Class
/// �ش� Class�� Type�� �ݿ��Ͽ� ���� Class Operator�� ���� �������� Ÿ������ ��ȯ
/// </summary>

class ShaderManager;
class VertexShader;
class PixelShader;
class ComputeShader;

class OriginalShader
{
public:
	ShaderManager* pThis;
	std::string shaderName;

public:
	operator VertexShader*();
	operator PixelShader*();
	operator ComputeShader*();
};
