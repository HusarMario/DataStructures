#include "Storage.h"

Storage::Storage()
{
	data = new ds::amt::ImplicitSequence<string>;
}

Storage::~Storage()
{
	delete data;
}

void Storage::putData(string data)
{
	this->data->insertLast().data_ = data;
}

string Storage::getData(int index)
{
	return this->data->access(index)->data_;
}

void Storage::printData()
{
	for (size_t i = 0; i < this->data->size(); i++)
	{
		cout << this->data->access(i)->data_;
		cout << " ";
	}
	cout << endl;
}

int Storage::sizeOfStorage()
{
	return data->size();
}
