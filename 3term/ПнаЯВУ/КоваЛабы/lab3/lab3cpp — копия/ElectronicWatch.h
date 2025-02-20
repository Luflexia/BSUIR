//ElectronicWatch.h
#pragma once
#include "Watch.h"

class ElectronicWatch :public Watch
{
private:
	char corpus[SIZE+1];
public:
	ElectronicWatch(const char* corpus = "", const char* brand = "", const char* model = "") :Watch(brand, model)
	{
		copyStr(this->corpus, corpus, SIZE);
	}
	ElectronicWatch(const ElectronicWatch& obj) :Watch(obj)
	{
		copyStr(this->corpus, obj.corpus, SIZE);
	}
	~ElectronicWatch()
	{
		//cout << "ElectronicWatch destructor" << endl;
	}
	void setCorpus(const char*);
	char* getCorpus();
	friend std::ostream& operator << (std::ostream&, const ElectronicWatch&);
	friend std::istream& operator >> (std::istream&, ElectronicWatch&);
};