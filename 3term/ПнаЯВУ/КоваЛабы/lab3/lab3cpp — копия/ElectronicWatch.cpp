//ElectronicWatch.cpp
#include "ElectronicWatch.h"

void ElectronicWatch::setCorpus(const char* corpus)
{
	copyStr(this->corpus, corpus, SIZE);
}

char* ElectronicWatch::getCorpus()
{
	return this->corpus;
}

std::ostream& operator<<(std::ostream& out, const ElectronicWatch& obj)
{
	return out << static_cast<const Watch&>(obj) << setw(SIZE+1) << obj.corpus;
}

std::istream& operator>>(std::istream& in, ElectronicWatch& obj)
{
	in >> static_cast<Watch&>(obj);
	cout << "Input corpus name" << endl;
	in.getline(obj.corpus, SIZE+1);
	cin.clear();
	rewind(stdin);
	return in;
}
