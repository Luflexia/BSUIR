//ElectronicWatch.h
#pragma once
#include "Watch.h"


class MechanicalWatch :public Watch
{
private:
	float screenSize;
	int battery;
public:
	MechanicalWatch(float screenSize = 0, int battery = 0, const char* brand = "", const char* model = "") :Watch(brand, model)
	{
		this->screenSize = screenSize;
		this->battery = battery;
	}
	MechanicalWatch(const MechanicalWatch& obj) :Watch(obj)
	{
		this->screenSize = obj.screenSize;
		this->battery = obj.battery;
	}
	~MechanicalWatch()
	{
		//cout << "MechanicalWatch destructor" << endl;
	}
	void setScreenSize(float);
	void setBattery(int);
	float getScreenSize();
	int getBattery();
	friend std::ostream& operator << (std::ostream&, const MechanicalWatch&);
	friend std::istream& operator >> (std::istream&, MechanicalWatch&);
};