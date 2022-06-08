#include "TextureManager.h"
#include "EngineData.h"
#include "GraphicEngineManager.h"
#include "GraphicEngineAPI.h"
#include "LoadManager.h"
#include "Profiler/Profiler.h"

#include <filesystem>
#include <iostream>

GraphicEngineManager*	TextureManager::m_Graphic = nullptr;
CRITICAL_SECTION*		TextureManager::m_CriticalSection = nullptr;
TextureManager::TextureManager()
{
	m_Graphic			= nullptr;
	m_CriticalSection	= nullptr;
}
TextureManager::~TextureManager()
{
	m_Graphic			= nullptr;
	m_CriticalSection	= nullptr;
}

void TextureManager::LoadTexture(std::string& Path)
{
	TextureBuffer* buffer = nullptr;

	EnterCriticalSection(m_CriticalSection);
	m_Graphic->CreateTextureBuffer(Path, &buffer);
	LeaveCriticalSection(m_CriticalSection);

	if (buffer == nullptr)
	{
		PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ Engine ][ Create ][ Texture Buffer ] '%s' FAILED!!", Path.c_str());
	}
	else
	{
		std::string Name = CutFilePath(Path);
		buffer->Name = Name;
		LoadManager::TextureList.insert({ Name,buffer });
	}
}

void TextureManager::BakeSkyLightMap(std::string& Path, bool hdri)
{
	SkyLightBuffer* buffer = LoadManager::GetSkyLight(Path);

	// 이미 만들어진 SkyLight가 있다면 처리하지 않는다..
	if (buffer != nullptr)	return;

	TextureBuffer* lightMap = LoadManager::GetTexture(Path);

	// 해당 Texture가 없을 경우..
	if (lightMap == nullptr)
	{
		PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ Engine ][ Bake ][ SkyLight Buffer ] '%s' Texture가 로드되지 않았습니다!!", Path.c_str());
		return;
	}

	EnterCriticalSection(m_CriticalSection);
	m_Graphic->BakeSkyLightMap(lightMap, hdri, &buffer);
	LeaveCriticalSection(m_CriticalSection);

	if (buffer == nullptr)
	{
		PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ Engine ][ Bake ][ SkyLight Buffer ] '%s' FAILED!!", Path.c_str());
	}
	else
	{
		std::string Name = CutFilePath(Path);
		buffer->Name = Name;
		LoadManager::SkyLightList.insert({ Name,buffer });
	}
}

void TextureManager::BakeConvertSkyLightMap(std::string& Path, float angle, float threshold, bool hdri)
{
	TextureBuffer* cubeMap = LoadManager::GetTexture(Path);
	
	// 해당 Texture가 없을 경우..
	if (cubeMap == nullptr)
	{
		PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ Engine ][ Convert ][ Convert SkyLight Buffer ] '%s' Texture가 로드되지 않았습니다!!", Path.c_str());
		return;
	}

	std::string Name = "SkyLight_Convert_" + Path;

	TextureBuffer* buffer = LoadManager::GetTexture(Name);

	// 생성된 Convert Cube Map이 있다면 제거..
	if (buffer)
	{
		LoadManager::DeleteTexture(Name);
		buffer = nullptr;
	}

	EnterCriticalSection(m_CriticalSection);
	m_Graphic->BakeConvertCubeMap(cubeMap, angle, threshold, hdri, &buffer);
	LeaveCriticalSection(m_CriticalSection);

	// 새로 생성한 SkyLight CubeMap 삽입..
	if (buffer == nullptr)
	{
		PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ Engine ][ Convert ][ Convert SkyCube Buffer ] '%s' FAILED!!", Path.c_str());
		return;
	}
	else
	{
		buffer->Name = Name;
		LoadManager::TextureList.insert({ Name, buffer });
	}

	// 해당 SkyLight 검색..
	SkyLightBuffer* skyLight = LoadManager::GetSkyLight(Name);

	// 생성된 Convert SkyLight가 있다면 제거..
	if (skyLight)
	{
		LoadManager::DeleteSkyLight(Name);
		skyLight = nullptr;
	}

	EnterCriticalSection(m_CriticalSection);
	m_Graphic->BakeSkyLightMap(buffer, false, &skyLight);
	LeaveCriticalSection(m_CriticalSection);

	// 새로 생성한 SkyLight Buffer 삽입..
	if (skyLight == nullptr)
	{
		PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ Engine ][ Convert ][ SkyLight Buffer ] '%s' FAILED!!", Path.c_str());
	}
	else
	{
		skyLight->Name = Name;
		LoadManager::SkyLightList.insert({ Name, skyLight });
	}

	// 변경한 SkyLight 적용..
	m_Graphic->SetSkyLight(skyLight, 0);
}

void TextureManager::BakeConvertSkyCubeMap(std::string& Path, float angle, float threshold, bool hdri)
{
	TextureBuffer* cubeMap = LoadManager::GetTexture(Path);

	// 해당 Texture가 없을 경우..
	if (cubeMap == nullptr)
	{
		PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ Engine ][ Convert ][ Convert SkyCube Buffer ] '%s' Texture가 로드되지 않았습니다!!", Path.c_str());
		return;
	}

	std::string Name = "SkyCube_Convert_" + Path;

	TextureBuffer* buffer = LoadManager::GetTexture(Name);

	// 생성된 Convert Cube Map이 있다면 제거..
	if (buffer)
	{
		LoadManager::DeleteTexture(Name);
		buffer = nullptr;
	}

	EnterCriticalSection(m_CriticalSection);
	m_Graphic->BakeConvertCubeMap(cubeMap, angle, threshold, hdri, &buffer);
	LeaveCriticalSection(m_CriticalSection);

	if (buffer == nullptr)
	{
		PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ Engine ][ Convert ][ Convert SkyCube Buffer ] '%s' FAILED!!", Path.c_str());
		return;
	}
	else
	{
		buffer->Name = Name;
		LoadManager::TextureList.insert({ Name, buffer });
	}

	// 변경한 SkyCube 적용..
	m_Graphic->SetSkyCube(buffer);
}

void TextureManager::SaveConvertSkyLightMap(std::string& Path, std::string& SaveName)
{
	std::string Name = "SkyLight_Convert_" + Path;

	TextureBuffer* buffer = LoadManager::GetTexture(Name);

	if (buffer == nullptr)
	{
		PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ Engine ][ Save ][ Save Convert SkyLight CubeMap ] '%s' FAILED!!", Path.c_str());
		return;
	}

	m_Graphic->SaveConvertCubeMap(buffer, SaveName);
}

void TextureManager::SaveConvertSkyCubeMap(std::string& Path, std::string& SaveName)
{
	std::string Name = "SkyCube_Convert_" + Path;

	TextureBuffer* buffer = LoadManager::GetTexture(Name);

	if (buffer == nullptr)
	{
		PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ Engine ][ Save ][ Save Convert SkyCube CubeMap ] '%s' FAILED!!", Path.c_str());
		return;
	}

	m_Graphic->SaveConvertCubeMap(buffer, SaveName);
}

void TextureManager::Initialize(GraphicEngineManager* Graphic, CRITICAL_SECTION* _cs)
{
	m_Graphic			= Graphic;
	m_CriticalSection	= _cs;
}
bool TextureManager::CheckFolder(std::string& Path)
{
	//경로에서 .을 찾지못했다면 폴더
	std::size_t Chick = Path.rfind('.');

	if (Chick > 2)
	{
		isFolder = true;
	}
	else
	{
		isFolder = false;
	}
	return isFolder;
}

void TextureManager::LoadFolder(std::string& Path)
{
	std::filesystem::path p(Path);
	if (std::filesystem::exists(p) == false)
	{
		return;
	}

	if (std::filesystem::is_directory(p) == false)
	{
		return;
	}

	std::filesystem::directory_iterator itr(p);
	while (itr != std::filesystem::end(itr)) {
		const std::filesystem::directory_entry& entry = *itr;

		if (std::filesystem::is_directory(entry.path()) == true)
		{
			//폴더안에 폴더는 읽지않는다
			itr++;
			continue;
		}
		std::string FileName = entry.path().filename().string();
		std::string FilePath = Path + '/';
		FilePath += FileName;

		//읽을 파일의 이름을 알아온다
		LoadFile(FilePath);
		itr++;
	}
}

void TextureManager::LoadFile(std::string& Path)
{
	TextureBuffer* buffer = nullptr;

	EnterCriticalSection(m_CriticalSection);
	m_Graphic->CreateTextureBuffer(Path, &buffer);
	LeaveCriticalSection(m_CriticalSection);
	
	TextureList.insert({CutFilePath(Path),buffer});
}

std::string TextureManager::CutFilePath(std::string& Path)
{
	if (isFolder == true)
	{
		std::size_t Start = Path.rfind('\\') + 1;
		std::size_t End = Path.rfind('.') - Start;
		return Path.substr(Start, End);
	}
	else
	{
		std::size_t Start = Path.rfind('/') + 1;
		std::size_t End = Path.rfind('.') - Start;
		return Path.substr(Start, End);;
	}
}



