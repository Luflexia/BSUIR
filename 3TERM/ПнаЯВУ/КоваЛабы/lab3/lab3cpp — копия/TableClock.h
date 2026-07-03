//TableClock.h
#pragma once
#include "MechanicalWatch.h"

class TableClock : public MechanicalWatch
{
private:
	int camera;
public:
	TableClock(int camera = 0, float screenSize = 0, int battery = 0, const char* brand = "",
		const char* model = ""):MechanicalWatch(screenSize,battery,brand,model)
	{
		this->camera = camera;
	}
	TableClock(const TableClock& obj):MechanicalWatch(obj)
	{
		this->camera = obj.camera;
	}
	~TableClock()
	{
		//cout << "TableClock destructor" << endl;
	}
	void setCamera(int);
	int getCamera();
	friend std::ostream& operator << (std::ostream&, const TableClock&);
	friend std::istream& operator >> (std::istream&, TableClock&);
};