//Watch.cpp
#include "Watch.h"


void Watch::setBrand(const char* brand)
{
	copyStr(this->brand, brand, SIZE);
}

void Watch::setModel(const char* model)
{
	copyStr(this->model, model, SIZE);
}

char* Watch::getBrand()
{
	return this->brand;
}

char* Watch::getModel()
{
	return this->model;
}

std::ostream& operator<<(std::ostream& out, const Watch& obj)
{
	return out << setw(SIZE + 1) << obj.brand << setw(SIZE + 1) << obj.model;
}

std::istream& operator>>(std::istream& in, Watch& obj)
{
	cout << "Input brand" << endl;
	in.clear();
	rewind(stdin);
	in.getline(obj.brand, SIZE + 1);
	in.clear();
	rewind(stdin);
	cout << "Input model" << endl;
	in.getline(obj.model, SIZE + 1);
	in.clear();
	rewind(stdin);
	return in;
}
