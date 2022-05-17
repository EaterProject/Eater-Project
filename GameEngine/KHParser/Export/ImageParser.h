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
	ImageParser() = delete;
	~ImageParser() = delete;

public:
	// Pixel Data�� ���� Image File ����..
	static PARSER_DLL void WriteImagePixel(const char* fileName, ParserData::ImageData& image, EXTENSION_TYPE extension_type);
	static PARSER_DLL void EraseImagePixel(ParserData::ImageData& image);

	static PARSER_DLL ParserData::ImageData LoadImagePixel(const char* fileName, unsigned int channels, PIXEL_TYPE pixelType);
	static PARSER_DLL DirectX::SimpleMath::Vector4 GetPixelColor(ParserData::ImageData& img, float x, float y);
};