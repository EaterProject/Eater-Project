#define _CRT_SECURE_NO_WARNINGS

#include "ImageParser.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

PARSER_DLL void ImageParser::WriteImagePixel(const char* fileName, ParserData::ImageData& image, EXTENSION_TYPE extension_type)
{
	if (image.type == PIXEL_TYPE::FLOAT_TYPE)
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
	else if (image.type == PIXEL_TYPE::CHAR_TYPE)
	{
		for (int i = 0; i < image.width * image.height; i++)
		{
			unsigned char* p = static_cast<unsigned char*>(image.imgColor) + i;

			for (unsigned int j = 0; j < image.channels; j++)
			{
				p[j] = 256 * p[j];
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
	case HDR_IMAGE:
		stbi_write_hdr(fileName, image.width, image.height, image.channels, static_cast<float*>(image.imgColor));
		break;
	default:
		break;
	}
}

PARSER_DLL void ImageParser::EraseImagePixel(ParserData::ImageData& image)
{
	stbi_image_free(image.imgColor);
}

PARSER_DLL ParserData::ImageData ImageParser::LoadImagePixel(const char* fileName, unsigned int channels, PIXEL_TYPE pixelType)
{
	ParserData::ImageData img;

	switch (pixelType)
	{
	case PIXEL_TYPE::FLOAT_TYPE:
	case PIXEL_TYPE::HDR_TYPE:
		img.imgColor = stbi_loadf(fileName, &img.width, &img.height, &img.comp, channels);
		break;
	case PIXEL_TYPE::CHAR_TYPE:
		img.imgColor = stbi_load(fileName, &img.width, &img.height, &img.comp, channels);
		break; 
	default:
		break;
	}

	img.type = pixelType;
	img.channels = channels;

	return img;
}

PARSER_DLL DirectX::SimpleMath::Vector4 ImageParser::GetPixelColor(ParserData::ImageData& img, float x, float y)
{
	switch (img.type)
	{
	case PIXEL_TYPE::FLOAT_TYPE:
	case PIXEL_TYPE::HDR_TYPE:
	{
		const float* p = static_cast<float*>(img.imgColor) + (4 * ((int)y * img.width + (int)x));

		return DirectX::SimpleMath::Vector4(p[0], p[1], p[2], p[3]);
	}
		break;
	case PIXEL_TYPE::CHAR_TYPE:
	{
		const unsigned char* p = static_cast<unsigned char*>(img.imgColor) + (4 * ((int)y * img.width + (int)x));

		return DirectX::SimpleMath::Vector4(p[0], p[1], p[2], p[3]);
	}
		break;
	default:
		break;
	}
}

PARSER_DLL DirectX::SimpleMath::Vector4 ImageParser::GetPixelColor(ParserData::ImageData& img, int x, int y)
{
	switch (img.type)
	{
	case PIXEL_TYPE::FLOAT_TYPE:
	case PIXEL_TYPE::HDR_TYPE:
	{
		const float* p = static_cast<float*>(img.imgColor) + (4 * (y * img.width + x));

		return DirectX::SimpleMath::Vector4(p[0], p[1], p[2], p[3]);
	}
	break;
	case PIXEL_TYPE::CHAR_TYPE:
	{
		const unsigned char* p = static_cast<unsigned char*>(img.imgColor) + (4 * (y * img.width + x));

		return DirectX::SimpleMath::Vector4(p[0], p[1], p[2], p[3]);
	}
	break;
	default:
		break;
	}
}
