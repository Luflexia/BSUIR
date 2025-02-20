//TableClock.h
#pragma once
#include "MechanicalWatch.h"

class TableClock : public MechanicalWatch
{
private:
	int shape;
public:
	TableClock(int shape = 0, float screenSize = 0, int battery = 0, const char* brand = "",
		const char* model = "") :MechanicalWatch(screenSize, battery, brand, model)
	{
		this->shape = shape;
	}
	TableClock(const TableClock& obj) :MechanicalWatch(obj)
	{
		this->shape = obj.shape;
	}
	~TableClock()
	{
		//cout << "TableClock destructor" << endl;
	}
	void setshape(int);
	int getshape();
	friend std::ostream& operator << (std::ostream&, const TableClock&);
	friend std::istream& operator >> (std::istream&, TableClock&);
};