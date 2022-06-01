#pragma once

#include "FormatDLL.h"
FORMAT_PARSER_DLL class FM_INDEX
{
public:


	unsigned int Index[3] = {0,0,0};
	void SetIndex(unsigned int& _1, unsigned int& _2, unsigned int& _3);
	void SetIndex(unsigned int&& _1, unsigned int&& _2, unsigned int&& _3);
};

