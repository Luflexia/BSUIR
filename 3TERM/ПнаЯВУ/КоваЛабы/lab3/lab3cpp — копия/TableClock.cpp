//TableClock.cpp
#include "TableClock.h"

void TableClock::setCamera(int cam)
{
	this->camera = cam;
}


int TableClock::getCamera()
{
	return this->camera;
}

std::ostream& operator<<(std::ostream& out, const TableClock&obj)
{
	return out << static_cast<const MechanicalWatch&>(obj) << setw(SIZE+1) << obj.camera;
}

std::istream& operator>>(std::istream& in, TableClock&obj)
{
	in >> static_cast<MechanicalWatch&>(obj);
	cout << "Input camera count" << endl;
	in >> obj.camera;
	return in;
}
