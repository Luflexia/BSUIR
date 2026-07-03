//ElectronicWatch.cpp
#include "ElectronicWatch.h"

void ElectronicWatch::setscreenSize(const char* screenSize)
{
	copyStr(this->screenSize, screenSize, SIZE);
}

char* ElectronicWatch::getscreenSize()
{
	return this->screenSize;
}

std::ostream& operator<<(std::ostream& out, const ElectronicWatch& obj)
{
	return out << static_cast<const Watch&>(obj) << setw(SIZE + 1) << obj.screenSize;
}

std::istream& operator>>(std::istream& in, ElectronicWatch& obj)
{
	in >> static_cast<Watch&>(obj);
	cout << "Input Screen size" << endl;
	in.getline(obj.screenSize, SIZE + 1);
	cin.clear();
	rewind(stdin);
	return in;
}