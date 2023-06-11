#pragma once
#include <fstream>
#include <sstream>

using namespace std;
class Reader
{
private:
	ifstream file;
	Reader();
public:
	Reader(string input);
	string giveNext();
	void close();
};

