#pragma once

#ifdef YAML_INTERFACE
#define YAML_PARSER_DLL __declspec(dllexport)
#else
#define YAML_PARSER_DLL __declspec(dllimport)

	#ifdef _DEBUG
		#pragma comment(lib,"YAMLParser_x64d")
	#else
		#pragma comment(lib,"YAMLParser_x64r")
	#endif

#endif
