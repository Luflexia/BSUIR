//WristSmartWatch.cpp
#include "WristSmartWatch.h"

void WristSmartWatch::setOS(float OS)
{
	this->OS = OS;
}

float WristSmartWatch::getOS()
{
	return this->OS;
}

std::ostream& operator<<(std::ostream& out, const WristSmartWatch& obj)
{
	return out << static_cast<const ElectronicWatch&>(obj) << setw(SIZE + 1) << obj.OS;
}

std::istream& operator>>(std::istream& in, WristSmartWatch& obj)
{
	in >> static_cast<ElectronicWatch&>(obj);
	cout << "Input OS type" << endl;
	in >> obj.OS;
	return in;
}