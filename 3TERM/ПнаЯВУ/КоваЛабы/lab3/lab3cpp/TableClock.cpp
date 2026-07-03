//TableClock.cpp
#include "TableClock.h"

void TableClock::setshape(int cam)
{
	this->shape = cam;
}


int TableClock::getshape()
{
	return this->shape;
}

std::ostream& operator<<(std::ostream& out, const TableClock& obj)
{
	return out << static_cast<const MechanicalWatch&>(obj) << setw(SIZE + 1) << obj.shape;
}

std::istream& operator>>(std::istream& in, TableClock& obj)
{
	in >> static_cast<MechanicalWatch&>(obj);
	cout << "Input shape count" << endl;
	in >> obj.shape;
	return in;
}
