#include "YamlManager.h"
#include "YAMLHeader.h"

YamlManager::YamlManager()
{

}

YamlManager::~YamlManager()
{

}

void YamlManager::OpenFile(std::string& Path)
{
	std::vector<std::string> KeyData;
	YAML_OPEN_FILE(Path, &KeyData);

	int size = (int)KeyData.size();
	for (int i = 0; i < size; i++)
	{
		std::string& Name = CutKeyString(KeyData[i]);

		//게임오브젝트가 아니면 넘긴다
		if (Name != "GameObject") { continue; }
		
		std::vector<std::string> key;
		YAML_FIND_STRING_DATA(&key, 2, KeyData[i], "m_Component");
		int Count = (int)key.size();

		for (int j = 1; j <= Count; j++)
		{
			int index = i + j;
			std::string ComponentKey = CutKeyString(KeyData[index]);

			if (ComponentKey == "BoxCollider")
			{

			}
			else if (ComponentKey == "Light")
			{

			}
			else if (ComponentKey == "Transform")
			{

			}
		}

		i += Count;
	}
}

std::string YamlManager::CutKeyString(std::string& key)
{
	size_t start = 0;
	size_t end = key.rfind('_');
	std::string& Name = key.substr(start, end);
	return Name;
}
