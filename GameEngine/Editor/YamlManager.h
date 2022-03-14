#pragma once
#include <string>
class YamlManager
{
public:
	YamlManager();
	~YamlManager();

	void OpenFile(std::string& Path);
	std::string CutKeyString(std::string& key);

	bool isCollider;
	bool isLight;
};

