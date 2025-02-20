//SmartWatch.h
#pragma once
#include "ElectronicWatch.h"
class SmartWatch :public ElectronicWatch
{
private:
	float screenSize;
public:
	SmartWatch(float screenSize = 0, const char* corpus = "", const char* brand = "", const char* model = ""):ElectronicWatch(corpus,brand,model)
	{
		this->screenSize = screenSize;
	}
	SmartWatch(const SmartWatch& obj):ElectronicWatch(obj)
	{
		this->screenSize = obj.screenSize;
	}
	~SmartWatch()
	{
		//cout << "SmartWatch destructor" << endl;
	}
	void setScreenSize(float);
	float getScreenSize();
	friend std::ostream& operator << (std::ostream&, const SmartWatch&);
	friend std::istream& operator >> (std::istream&, SmartWatch&);
};