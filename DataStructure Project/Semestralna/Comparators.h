#pragma once
#include <Windows.h>
#include <WinUser.h>
#include <iostream>
#include <clocale>
#include <locale>
#include <string>
#include "Storage.h"
#include <libds/adt/table.h>

using namespace std;
class Comparators
{
public:
	static bool compareAlphabetical(Storage* object1, Storage* object2) {
		ds::adt::SortedSequenceTable<string, int>* alphabet = new ds::adt::SortedSequenceTable<string, int>;
		string characters[] = {
			"a", "·", "‰", "b", "c", "Ë", "d", "Ô", "dz", "dû", "e", "È", "f", "g", "h", "ch", "i", "Ì", "j", "k", "l", "Â", "æ", "m", "n", "Ú",
			"o", "Û", "Ù", "p", "q", "r", "‡", "s", "ö", "t", "ù", "u", "˙", "v", "w", "x", "y", "˝", "z", "û", "-", "(", ")", " "};
		
		

		for (int i = 0; i < 50; i++)
		{
			alphabet->insert(characters[i], i);
		}
		
		string object1title = object1->getData(2);
		string object2title = object2->getData(2);

		CharLowerBuffA(&object1title[0], (DWORD)object1title.size());
		CharLowerBuffA(&object2title[0], (DWORD)object2title.size());

		int comparisionSize = min(object1title.length(), object2title.length());
		int index1 = 0;
		int index2 = 0;

		while (comparisionSize > index1 && comparisionSize > index2) {
			string character1 = object1title.substr(index1, 1);
			if (index1 + 1 != comparisionSize && character1 == "c" && object1title.substr(index1, 2) == "ch") {
				character1 = object1title.substr(index1, 2);
				index1++;
			}
			else if (index1 + 1 != comparisionSize && character1 == "d" && object1title.substr(index1, 2) == "dz") {
				character1 = object1title.substr(index1, 2);
				index1++;
			}
			else if (index1 + 1 != comparisionSize && character1 == "d" && object1title.substr(index1, 2) == "dû") {
				character1 = object1title.substr(index1, 2);
				index1++;
			}

			string character2 = object2title.substr(index2, 1);
			if (index2 + 1 != comparisionSize && character2 == "c" && object2title.substr(index2, 2) == "ch") {
				character2 = object2title.substr(index2, 2);
				index2++;
			}
			else if (index2 + 1 != comparisionSize && character2 == "d" && object2title.substr(index2, 2) == "dz") {
				character2 = object2title.substr(index2, 2);
				index2++;
			}
			else if (index2 + 1 != comparisionSize && character2 == "d" && object2title.substr(index2, 2) == "dû") {
				character2 = object1title.substr(index2, 2);
				index2++;
			}

			int value1 = alphabet->find(character1);
			int value2 = alphabet->find(character2);

			if (value1 < value2) {
				delete alphabet;
				return true;
			}
			if (value1 > value2) {
				delete alphabet;
				return false;
			}

			index1++;
			index2++;
		}

		if ((object1title.length() == index1) && (object2title.length() == index2)) {
			delete alphabet;
			return true;
		}
		else if (object1title.length() == index1) {
			delete alphabet;
			return true;
		}
		else {
			delete alphabet;
			return false;
		}	
	}

	/*static bool compareAlphabetical(Storage* object1, Storage* object2) {
		string object1title = object1->getData(2);
		string object2title = object2->getData(2);
		
		CharLowerBuffA(&object1title[0], (DWORD)object1title.size());
		CharLowerBuffA(&object2title[0], (DWORD)object2title.size());

		int comparision = object1title.compare(object2title);
		return comparision < 0;
	}*/

	static bool compareVowelsCount(Storage* object1, Storage* object2) {
		int object1VowelsCount = 0;
		int object2VowelsCount = 0;

		for (int i = 0; i < object1->getData(2).length(); i++)
		{
			if (isVowel(object1->getData(2)[i])) {
				object1VowelsCount++;
			}
		}

		for (int i = 0; i < object2->getData(2).length(); i++)
		{
			if (isVowel(object2->getData(2)[i])) {
				object2VowelsCount++;
			}
		}

		return object1VowelsCount <= object2VowelsCount;
	}

private:
	static bool isVowel(char c) {	//podæa internetu mÙûu samohl·sky byù aj r ‡ l Â m n (Ëo mne prÌde ako hÂuposù takûe som to neimplementoval) z·roveÚ okrem Ù s˙ dvojhl·sky branÈ ako dve samohl·sky
		if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'y' || c == '·' || c == 'È' || c == 'Ì' || c == 'Û' || c == '˙' || c == '˝' || c == 'Ù' || c == '‰'
			|| c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U' || c == 'Y' || c == '¡' || c == '…' || c == 'Õ' || c == '”' || c == '⁄' || c == '›' || c == '‘' ||  c == 'ƒ') {
			return true;
		}
		else {
			return false;
		}
	}
};