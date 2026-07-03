//SmartWatch.h
#pragma once
#include "ElectronicWatch.h"
class SmartWatch :public ElectronicWatch
{
private:
	float battery;
public:
	SmartWatch(float battery = 0, const char* corpus = "", const char* brand = "", const char* model = "") :ElectronicWatch(corpus, brand, model)
	{
		this->battery = battery;
	}
	SmartWatch(const SmartWatch& obj) :ElectronicWatch(obj)
	{
		this->battery = obj.battery;
	}
	~SmartWatch()
	{
		//cout << "SmartWatch destructor" << endl;
	}
	void setbattery(float);
	float getbattery();
	friend std::ostream& operator << (std::ostream&, const SmartWatch&);
	friend std::istream& operator >> (std::istream&, SmartWatch&);
};