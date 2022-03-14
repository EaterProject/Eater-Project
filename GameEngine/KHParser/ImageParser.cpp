#define _CRT_SECURE_NO_WARNINGS

#include "ImageParser.h"

#include "FloatImage.h"
#include "CharImage.h"


#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

PARSER_DLL ImageParser* ImageParser::Create(IMAGE_TYPE type)
{
	ImageParser* newParser = nullptr;

	switch (type)
	{
	case IMAGE_TYPE::FLOAT_IMAGE:
		newParser = new FloatImage();
		newParser->Initialize();
		break;
	case IMAGE_TYPE::CHAR_IMAGE:
		newParser = new CharImage();
		newParser->Initialize();
		break;
	default:
		break;
	}

	return newParser;
}

PARSER_DLL void ImageParser::WriteImagePixel(const char* fileName, ParserData::ImageData image, EXTENSION_TYPE extension_type)
{
	if (image.type == IMAGE_TYPE::FLOAT_IMAGE)
	{
		for (int i = 0; i < image.width * image.height; i++)
		{
			float* p = static_cast<float*>(image.imgColor) + i;

			for (unsigned int j = 0; j < image.channels; j++)
			{
				p[j] = 255.99f * p[j];
			}
		}
	}

	switch (extension_type)
	{
	case PNG_IMAGE:
		stbi_write_png(fileName, image.width, image.height, image.channels, image.imgColor, image.width * image.channels);
		break;
	case BMP_IMAGE:
		stbi_write_bmp(fileName, image.width, image.height, image.channels, image.imgColor);
		break;
	default:
		break;
	}
}
