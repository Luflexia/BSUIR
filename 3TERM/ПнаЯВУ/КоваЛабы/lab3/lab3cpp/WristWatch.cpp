//WristWatch.cpp
#include "WristWatch.h"

void WristWatch::setclockFaceType(int clockFaceType)
{
	this->clockFaceType = clockFaceType;
}

void WristWatch::setstrap(const char* strap)
{
	copyStr(this->strap, strap, SIZE);
}

int WristWatch::getclockFaceType()
{
	return this->clockFaceType;
}

char* WristWatch::getstrap()
{
	return this->strap;
}

std::ostream& operator<<(std::ostream& out, const WristWatch& obj)
{
	return out << static_cast<const MechanicalWatch&>(obj) << setw(SIZE + 1) << obj.clockFaceType << setw(SIZE + 1) << obj.strap;
}

std::istream& operator>>(std::istream& in, WristWatch& obj)
{
	in >> static_cast<MechanicalWatch&>(obj);
	cout << "Input Strap" << endl;
	in >> obj.clockFaceType;
	cout << "Input Clock face type" << endl;
	in.clear();
	rewind(stdin);
	in.getline(obj.strap, SIZE + 1);
	in.clear();
	rewind(stdin);
	return in;
}