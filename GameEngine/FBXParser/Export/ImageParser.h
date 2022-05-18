#pragma once
#pragma warning(disable : 4251)

#include <windows.h>
#include "ParserDLL.h"
#include "ParserData.h"

/// Image Parser InterFace Class
/// 
/// - 외부에서 실질적으로 사용할 함수들을 모아논 InterFace Class
/// - Parser Type을 통해 DLL 내부에서 할당 후 포인터 반환 형식
/// - 해당 Parser Class가 아닌 InterFace Class를 통해 함수 호출 방식


class ImageParser
{
public:
	ImageParser() = delete;
	~ImageParser() = delete;

public:
	// Pixel Data를 통해 Image File 생성..
	static PARSER_DLL void WriteImagePixel(const char* fileName, ParserData::ImageData& image, EXTENSION_TYPE extension_type);
	static PARSER_DLL void EraseImagePixel(ParserData::ImageData& image);

	static PARSER_DLL ParserData::ImageData LoadImagePixel(const char* fileName, unsigned int channels, PIXEL_TYPE pixelType);
	static PARSER_DLL DirectX::SimpleMath::Vector4 GetPixelColor(ParserData::ImageData& img, float x, float y);
};