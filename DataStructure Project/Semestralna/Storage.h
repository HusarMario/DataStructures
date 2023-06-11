#pragma once
#include <libds/amt/implicit_sequence.h>
#include <string>

using namespace std;
class Storage
{
private:
	ds::amt::ImplicitSequence<string>* data;
public:
	Storage();
	~Storage();
	void putData(string data);
	string getData(int index);
	void printData();
	int sizeOfStorage();
};

