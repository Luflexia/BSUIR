#pragma once
#include "Watch.h"


class MechanicalWatch :public Watch
{
private:
	float mechanism;
	int material;
public:
	MechanicalWatch(float mechanism = 0, int material = 0, const char* brand = "", const char* model = "") :Watch(brand, model)
	{
		this->mechanism = mechanism;
		this->material = material;
	}
	MechanicalWatch(const MechanicalWatch& obj) :Watch(obj)
	{
		this->mechanism = obj.mechanism;
		this->material = obj.material;
	}
	~MechanicalWatch()
	{
		//cout << "MechanicalWatch destructor" << endl;
	}
	void setmechanism(float);
	void setmaterial(int);
	float getmechanism();
	int getmaterial();
	friend std::ostream& operator << (std::ostream&, const MechanicalWatch&);
	friend std::istream& operator >> (std::istream&, MechanicalWatch&);
};