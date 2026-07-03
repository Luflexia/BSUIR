//MechanicalWatch.cpp
#include "MechanicalWatch.h"

void MechanicalWatch::setScreenSize(float screenSize)
{
	this->screenSize;
}

void MechanicalWatch::setBattery(int battery)
{
	this->battery = battery;
}

float MechanicalWatch::getScreenSize()
{
	return this->screenSize;
}

int MechanicalWatch::getBattery()
{
	return this->battery;
}

std::ostream& operator<<(std::ostream& out, const MechanicalWatch& obj)
{
	return out << static_cast<const Watch&>(obj) << setw(SIZE+1) << obj.battery << setw(SIZE+1) << obj.screenSize;
}

std::istream& operator>>(std::istream& in, MechanicalWatch& obj)
{
	in >> static_cast<Watch&>(obj);
	cout << "Input battery" << endl;
	in >> obj.battery;
	cout << "Input screen size" << endl;
	in >> obj.screenSize;
	
	return in;
}
