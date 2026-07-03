// Watch.h
#pragma once
#include <iostream>
#include <iomanip>
#include "copyStr.h"
#define SIZE 15
using namespace std;
class Watch
{
private:
	char brand[SIZE+1];
	char model[SIZE+1];
public:
	Watch(const char* brand = "",const char* model = "")
	{
		copyStr(this->brand, brand, SIZE);
		copyStr(this->model, model, SIZE);	
	}
	Watch(const Watch& obj)
	{
		copyStr(this->brand, obj.brand, SIZE);
		copyStr(this->model, obj.model, SIZE);
	}
	~Watch()
	{
		//cout << "Watchdestructor" << endl;
	}
	void setBrand(const char*);
	void setModel(const char*);
	char* getBrand();
	char* getModel();
	friend std::ostream& operator << (std::ostream&, const Watch&); 
	friend std::istream& operator >> (std::istream&, Watch&);
};