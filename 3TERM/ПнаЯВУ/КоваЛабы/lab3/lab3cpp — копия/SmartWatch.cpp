//SmartWatch.cpp
#include "SmartWatch.h"

void SmartWatch::setScreenSize(float screenSize)
{
	this->screenSize = screenSize;
}

float SmartWatch::getScreenSize()
{
	return this->screenSize;
}

std::ostream& operator<<(std::ostream& out, const SmartWatch& obj)
{
	return out << static_cast<const ElectronicWatch&>(obj) << setw(SIZE+1) << obj.screenSize;
}

std::istream& operator>>(std::istream& in, SmartWatch& obj)
{
	in >> static_cast<ElectronicWatch&>(obj);
	cout << "Input screen size" << endl;
	in >> obj.screenSize;
	return in;
}