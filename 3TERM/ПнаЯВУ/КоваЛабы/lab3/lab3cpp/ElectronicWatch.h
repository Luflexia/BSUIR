// ElectronicWatch.h
#pragma once
#include "Watch.h"

class ElectronicWatch :public Watch
{
private:
	char screenSize[SIZE + 1];
public:
	ElectronicWatch(const char* screenSize = "", const char* brand = "", const char* model = "") :Watch(brand, model)
	{
		copyStr(this->screenSize, screenSize, SIZE);
	}
	ElectronicWatch(const ElectronicWatch& obj) :Watch(obj)
	{
		copyStr(this->screenSize, obj.screenSize, SIZE);
	}
	~ElectronicWatch()
	{
		//cout << "ElectronicWatch destructor" << endl;
	}
	void setscreenSize(const char*);
	char* getscreenSize();
	friend std::ostream& operator << (std::ostream&, const ElectronicWatch&);
	friend std::istream& operator >> (std::istream&, ElectronicWatch&);
};