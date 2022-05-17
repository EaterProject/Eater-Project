#include "TextureManager.h"
#include "EngineData.h"
#include "GraphicEngineManager.h"
#include "GraphicsEngine.h"
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

	// �̹� ������� SkyLight�� �ִٸ� ó������ �ʴ´�..
	if (buffer != nullptr)	return;

	TextureBuffer* lightMap = LoadManager::GetTexture(Path);

	// �ش� Texture�� ���� ���..
	if (lightMap == nullptr)
	{
		PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ Engine ][ Bake ][ SkyLight Buffer ] '%s' Texture�� �ε���� �ʾҽ��ϴ�!!", Path.c_str());
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

void TextureManager::BakeConvertCubeMap(std::string& Path, float angle, bool save_file, bool hdri, bool apply_skylight, bool apply_environment)
{
	TextureBuffer* cubeMap = LoadManager::GetTexture(Path);

	// �ش� Texture�� ���� ���..
	if (cubeMap == nullptr)
	{
		PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ Engine ][ Bake ][ Convert CubeMap Buffer ] '%s' Texture�� �ε���� �ʾҽ��ϴ�!!", Path.c_str());
		return;
	}

	std::string Name = Path + "_Convert";

	TextureBuffer* buffer = LoadManager::GetTexture(Name);

	// ������ Convert Cube Map�� �ִٸ� ����..
	if (buffer && apply_environment)
	{
		LoadManager::DeleteTexture(Name);
		buffer = nullptr;
	}

	EnterCriticalSection(m_CriticalSection);
	m_Graphic->BakeConvertCubeMap(cubeMap, angle, save_file, hdri, &buffer);
	LeaveCriticalSection(m_CriticalSection);

	if (buffer == nullptr)
	{
		PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ Engine ][ Bake ][ Convert CubeMap Buffer ] '%s' FAILED!!", Path.c_str());
		return;
	}
	else
	{
		buffer->Name = Name;
		LoadManager::TextureList.insert({ Name, buffer });
	}

	if (apply_skylight)
	{
		SkyLightBuffer* skyLight = LoadManager::GetSkyLight(Name);

		// ������ Convert Sky Light�� �ִٸ� ����..
		if (skyLight)
		{
			LoadManager::DeleteSkyLight(Name);
			skyLight = nullptr;
		}

		EnterCriticalSection(m_CriticalSection);
		m_Graphic->BakeSkyLightMap(buffer, false, &skyLight);
		LeaveCriticalSection(m_CriticalSection);

		if (skyLight == nullptr)
		{
			PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ Engine ][ Bake ][ SkyLight Buffer ] '%s' FAILED!!", Path.c_str());
		}
		else
		{
			skyLight->Name = Name;
			LoadManager::SkyLightList.insert({ Name, skyLight });
		}

		m_Graphic->SetSkyLight(skyLight);
	}

	if (apply_environment)
	{
		m_Graphic->SetEnvironment(buffer);
	}
}

void TextureManager::Initialize(GraphicEngineManager* Graphic, CRITICAL_SECTION* _cs)
{
	m_Graphic			= Graphic;
	m_CriticalSection	= _cs;
}
bool TextureManager::CheckFolder(std::string& Path)
{
	//��ο��� .�� ã�����ߴٸ� ����
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
			//�����ȿ� ������ �����ʴ´�
			itr++;
			continue;
		}
		std::string FileName = entry.path().filename().string();
		std::string FilePath = Path + '/';
		FilePath += FileName;

		//���� ������ �̸��� �˾ƿ´�
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



