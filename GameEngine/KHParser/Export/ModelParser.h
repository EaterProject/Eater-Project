#pragma once
#pragma warning(disable : 4251)

#include <windows.h>
#include "ParserDLL.h"
#include "ParserData.h"

/// Model Parser InterFace Class
/// 
/// - �ܺο��� ���������� ����� �Լ����� ��Ƴ� InterFace Class
/// - Parser Type�� ���� DLL ���ο��� �Ҵ� �� ������ ��ȯ ����
/// - �ش� Parser Class�� �ƴ� InterFace Class�� ���� �Լ� ȣ�� ���

class ModelParser
{
public:
	ModelParser() = default;
	virtual ~ModelParser() = default;

public:
	// Create �Լ��� ���� ModelParser ����..
	static PARSER_DLL ModelParser* Create(MODEL_TYPE type);

	// �⺻ Texture ��� ���� �Լ�..
	static PARSER_DLL void SetTextureRoute(std::string texRoute);

protected:
	static std::string g_TextureRoute;

public:
	// �⺻���� �ʱ�ȭ �Լ�..
	virtual PARSER_DLL void Initialize() abstract;

	// Model Data ���� �Լ�..
	virtual PARSER_DLL void Release() abstract;

	// Model Load �Լ�..
	virtual PARSER_DLL ParserData::Model* LoadModel(std::string fileName) { return nullptr; }
	virtual PARSER_DLL ParserData::Model* LoadModel(std::string fileName, UINT state) { return nullptr; }
};

