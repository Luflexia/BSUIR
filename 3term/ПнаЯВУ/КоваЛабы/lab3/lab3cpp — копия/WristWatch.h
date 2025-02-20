//WristWatch.h
#pragma once
#include "MechanicalWatch.h"
class WristWatch :public MechanicalWatch
{
private:
	int keyboard;
	char GPU[SIZE+1];
public:
	WristWatch(int keyboard = 0, const char* GPU = "", float screenSize = 0, int battery = 0,
		const char* brand = "", const char* model = ""):MechanicalWatch(screenSize,battery,brand,model)
	{
		this->keyboard = keyboard;
		copyStr(this->GPU, GPU, SIZE);
	}
	WristWatch(const WristWatch& obj):MechanicalWatch(obj)
	{
		this->keyboard = obj.keyboard;
		copyStr(this->GPU, obj.GPU, SIZE);
	}
	~WristWatch()
	{
		//cout << "WristWatch destructor" << endl;
	}
	void setKeyboard(int);
	void setGPU(const char*);
	int getKeyboard();
	char* getGPU();
	friend std::ostream& operator << (std::ostream&, const WristWatch&);
	friend std::istream& operator >> (std::istream&, WristWatch&);
};