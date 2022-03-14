#include "ImageParser.h"
#include "FloatImage.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "stb_image.h"

void FloatImage::Initialize()
{

}

void FloatImage::SetTextureRoute(std::string texRoute)
{
	m_IconTextureRoute = texRoute;
}

ParserData::ImageData FloatImage::LoadImagePixel(const char* fileName, unsigned int channels)
{
	std::string filePath = m_IconTextureRoute + fileName;

	ParserData::ImageData imgData;
	imgData.imgColor = stbi_loadf(filePath.c_str(), &imgData.width, &imgData.height, nullptr, channels);
	imgData.type = FLOAT_IMAGE;
	imgData.channels = channels;

	return imgData;
}

DirectX::SimpleMath::Vector4 FloatImage::GetPixelColor(ParserData::ImageData& img, int x, int y)
{
	const float* p = static_cast<float*>(img.imgColor) + (4 * (y * img.width + x));

	return DirectX::SimpleMath::Vector4(p[0], p[1], p[2], p[3]);
}

DirectX::SimpleMath::Vector4 FloatImage::GetPixelColor(ParserData::ImageData& img, float x, float y)
{
	const float* p = static_cast<float*>(img.imgColor) + (4 * ((int)y * img.width + (int)x));

	return DirectX::SimpleMath::Vector4(p[0], p[1], p[2], p[3]);
}
