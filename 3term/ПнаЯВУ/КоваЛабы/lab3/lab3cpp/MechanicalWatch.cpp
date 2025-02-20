//MechanicalWatch.cpp
#include "MechanicalWatch.h"

void MechanicalWatch::setmechanism(float mechanism)
{
	this->mechanism;
}

void MechanicalWatch::setmaterial(int material)
{
	this->material = material;
}

float MechanicalWatch::getmechanism()
{
	return this->mechanism;
}

int MechanicalWatch::getmaterial()
{
	return this->material;
}

std::ostream& operator<<(std::ostream& out, const MechanicalWatch& obj)
{
	return out << static_cast<const Watch&>(obj) << setw(SIZE + 1) << obj.material << setw(SIZE + 1) << obj.mechanism;
}

std::istream& operator>>(std::istream& in, MechanicalWatch& obj)
{
	in >> static_cast<Watch&>(obj);
	cout << "Input material" << endl;
	in >> obj.material;
	cout << "Input mechanism" << endl;
	in >> obj.mechanism;

	return in;
}