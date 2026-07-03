//WristSmartWatch.h
#pragma once
#include "ElectronicWatch.h"
class WristSmartWatch :public ElectronicWatch
{
private:
	float OS;
public:
	WristSmartWatch(float OS = 0, const char* corpus = "", const char* brand = "", const char* model = "") :ElectronicWatch(corpus, brand, model)
	{
		this->OS = OS;
	}
	WristSmartWatch(const WristSmartWatch& obj) :ElectronicWatch(obj)
	{
		this->OS = obj.OS;
	}
	~WristSmartWatch()
	{
		//cout << "WristSmartWatch destructor" << endl;
	}
	void setOS(float);
	float getOS();
	friend std::ostream& operator << (std::ostream&, const WristSmartWatch&);
	friend std::istream& operator >> (std::istream&, WristSmartWatch&);
};