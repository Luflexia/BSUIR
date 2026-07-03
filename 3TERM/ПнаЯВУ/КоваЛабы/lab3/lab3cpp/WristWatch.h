//WristWatch.h
#pragma once
#include "MechanicalWatch.h"
class WristWatch :public MechanicalWatch
{
private:
	int clockFaceType;
	char strap[SIZE + 1];
public:
	WristWatch(int clockFaceType = 0, const char* strap = "", float screenSize = 0, int battery = 0,
		const char* brand = "", const char* model = "") :MechanicalWatch(screenSize, battery, brand, model)
	{
		this->clockFaceType = clockFaceType;
		copyStr(this->strap, strap, SIZE);
	}
	WristWatch(const WristWatch& obj) :MechanicalWatch(obj)
	{
		this->clockFaceType = obj.clockFaceType;
		copyStr(this->strap, obj.strap, SIZE);
	}
	~WristWatch()
	{
		//cout << "WristWatch destructor" << endl;
	}
	void setclockFaceType(int);
	void setstrap(const char*);
	int getclockFaceType();
	char* getstrap();
	friend std::ostream& operator << (std::ostream&, const WristWatch&);
	friend std::istream& operator >> (std::istream&, WristWatch&);
};