#include "ModelParser.h"

#include "ASEFile.h"
#include "ASEParser.h"

#include "BoneWeights.h"
#define FBXSDK_SHARED
#pragma comment(lib, "libfbxsdk")
#include <fbxsdk.h>
#include "FBXParser.h"

std::string ModelParser::g_TextureRoute;

PARSER_DLL ModelParser* ModelParser::Create(MODEL_TYPE type)
{
	ModelParser* newParser = nullptr;

	switch (type)
	{
	case MODEL_TYPE::FBX_MODEL:
		newParser = new FBXParser;
		newParser->Initialize();
		break;
	case MODEL_TYPE::ASE_MODEL:
		newParser = new ASEParser;
		newParser->Initialize();
		break;
	default:
		break;
	}

	return newParser;
}

PARSER_DLL void ModelParser::SetTextureRoute(std::string texRoute)
{
	// 기본 Texture 파일 루트 설정
	g_TextureRoute = texRoute;
}
