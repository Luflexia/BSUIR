//SmartWatch.cpp
#include "SmartWatch.h"

void SmartWatch::setbattery(float battery)
{
	this->battery = battery;
}

float SmartWatch::getbattery()
{
	return this->battery;
}

std::ostream& operator<<(std::ostream& out, const SmartWatch& obj)
{
	return out << static_cast<const ElectronicWatch&>(obj) << setw(SIZE + 1) << obj.battery;
}

std::istream& operator>>(std::istream& in, SmartWatch& obj)
{
	in >> static_cast<ElectronicWatch&>(obj);
	cout << "Input Battery capacity" << endl;
	in >> obj.battery;
	return in;
}