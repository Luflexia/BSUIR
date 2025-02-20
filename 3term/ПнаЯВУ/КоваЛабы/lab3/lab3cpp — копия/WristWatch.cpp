//WristWatch.cpp
#include "WristWatch.h"

void WristWatch::setKeyboard(int keyboard)
{
	this->keyboard = keyboard;
}

void WristWatch::setGPU(const char* GPU)
{
	copyStr(this->GPU, GPU, SIZE);
}

int WristWatch::getKeyboard()
{
	return this->keyboard;
}

char* WristWatch::getGPU()
{
	return this->GPU;
}

std::ostream& operator<<(std::ostream& out, const WristWatch& obj)
{
	return out << static_cast<const MechanicalWatch&>(obj) << setw(SIZE+1) << obj.keyboard << setw(SIZE+1) << obj.GPU;
}

std::istream& operator>>(std::istream& in, WristWatch& obj)
{
	in >> static_cast<MechanicalWatch&>(obj);
	cout << "Input keys count" << endl;
	in >> obj.keyboard;
	cout << "Input GPU" << endl;
	in.clear();
	rewind(stdin);
	in.getline(obj.GPU, SIZE + 1);
	in.clear();
	rewind(stdin);
	return in;
}
