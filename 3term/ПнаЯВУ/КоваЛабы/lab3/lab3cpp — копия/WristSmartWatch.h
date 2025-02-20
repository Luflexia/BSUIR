//WristSmartWatch.h
#pragma once
#include "ElectronicWatch.h"
class WristSmartWatch :public ElectronicWatch
{
private:
	float screenSize;
public:
	WristSmartWatch(float screenSize = 0, const char* corpus = "", const char* brand = "", const char* model = ""):ElectronicWatch(corpus,brand,model)
	{
		this->screenSize = screenSize;
	}
	WristSmartWatch(const WristSmartWatch& obj):ElectronicWatch(obj)
	{
		this->screenSize = obj.screenSize;
	}
	~WristSmartWatch()
	{
		//cout << "WristSmartWatch destructor" << endl;
	}
	void setScreenSize(float);
	float getScreenSize();
	friend std::ostream& operator << (std::ostream&, const WristSmartWatch&);
	friend std::istream& operator >> (std::istream&, WristSmartWatch&);
};