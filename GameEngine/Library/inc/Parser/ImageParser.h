#pragma once
#pragma warning(disable : 4251)

#include <windows.h>
#include "ParserDLL.h"
#include "ParserData.h"

/// Image Parser InterFace Class
/// 
/// - �ܺο��� ���������� ����� �Լ����� ��Ƴ� InterFace Class
/// - Parser Type�� ���� DLL ���ο��� �Ҵ� �� ������ ��ȯ ����
/// - �ش� Parser Class�� �ƴ� InterFace Class�� ���� �Լ� ȣ�� ���


class ImageParser
{
public:
	ImageParser() = default;
	virtual ~ImageParser() = default;

public:
	// Create �Լ��� ���� ImageParser ����..
	static PARSER_DLL ImageParser* Create(IMAGE_TYPE type);

public:
	// Pixel Data�� ���� Image File ����..
	static PARSER_DLL void WriteImagePixel(const char* fileName, ParserData::ImageData image, EXTENSION_TYPE extension_type);
	static PARSER_DLL void EraseImagePixel(ParserData::ImageData image);
public:
	// �⺻���� �ʱ�ȭ �Լ�..
	virtual PARSER_DLL void Initialize() abstract;

	// �⺻ Texture ��� ���� �Լ�..
	virtual PARSER_DLL void SetTextureRoute(std::string texRoute) abstract;

	// �ش� Image Pixel Color ���� �Լ�..
	virtual PARSER_DLL ParserData::ImageData LoadImagePixel(const char* fileName, unsigned int channels) abstract;

	// �ش� ��ġ Pixel Color ���� �Լ�..
	virtual PARSER_DLL DirectX::SimpleMath::Vector4 GetPixelColor(ParserData::ImageData& img, int x, int y) abstract;
	virtual PARSER_DLL DirectX::SimpleMath::Vector4 GetPixelColor(ParserData::ImageData& img, float x, float y) abstract;
};