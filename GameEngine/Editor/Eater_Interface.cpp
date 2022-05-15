#include "Eater_Interface.h"

Eater_Interface::Eater_Interface()
{

}

Eater_Interface::~Eater_Interface()
{

}

std::string Eater_Interface::CutStr(std::string& Name)
{
	std::size_t Start = Name.rfind('/') + 1;
	std::size_t End = Name.rfind('.') - Start;
	return Name.substr(Start, End);
}
