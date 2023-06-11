#include "Reader.h"

Reader::Reader()
{

}

Reader::Reader(string input)
{
	file.open(input);	
}

string Reader::giveNext()
{
	string line;
	getline(file, line);
	return line;
}

void Reader::close()
{
	file.close();
}
