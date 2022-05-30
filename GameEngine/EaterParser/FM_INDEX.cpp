#include "FM_INDEX.h"

void FM_INDEX::SetIndex(unsigned int& _1, unsigned int& _2, unsigned int& _3)
{
	Index[0] = _1;
	Index[1] = _2;
	Index[2] = _3;
}

void FM_INDEX::SetIndex(unsigned int&& _1, unsigned int&& _2, unsigned int&& _3)
{
	Index[0] = _1;
	Index[1] = _2;
	Index[2] = _3;
}
