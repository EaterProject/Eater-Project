#include "TextureManager.h"
#include "DebugManager.h"
#include "EngineData.h"
#include "GraphicEngineManager.h"
#include "GraphicsEngine.h"
#include "LoadManager.h"

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
		DebugManager::Print(DebugManager::MSG_TYPE::MSG_LOAD, "TEXTURE", CutFilePath(Path),true);
	}
	else
	{
		std::string Name = CutFilePath(Path);
		LoadManager::TextureList.insert({ Name,buffer });
	}
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



