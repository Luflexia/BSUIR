//WristSmartWatch.cpp
#include "WristSmartWatch.h"

void WristSmartWatch::setScreenSize(float screenSize)
{
	this->screenSize = screenSize;
}

float WristSmartWatch::getScreenSize()
{
	return this->screenSize;
}

std::ostream& operator<<(std::ostream& out, const WristSmartWatch& obj)
{
	return out << static_cast<const ElectronicWatch&>(obj) << setw(SIZE+1) << obj.screenSize;
}

std::istream& operator>>(std::istream& in, WristSmartWatch& obj)
{
	in >> static_cast<ElectronicWatch&>(obj);
	cout << "Input screen size" << endl;
	in >> obj.screenSize;
	return in;
}