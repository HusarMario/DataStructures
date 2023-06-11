#pragma once

#include <Windows.h>
#include <WinUser.h>
#include <string>
#include "Storage.h"


using namespace std;
class Predicates
{
public:
    static bool hasGymnazium(Storage* comparing) {
        if (comparing->sizeOfStorage() == 7 && comparing->getData(6) != "0") {
            return true;
        }
        else {
            return false;
        }
    }

    static bool containsStr(string input, string comparing) {
        CharLowerBuffA(&input[0], (DWORD)input.size());
        CharLowerBuffA(&comparing[0], (DWORD)comparing.size());
        if (comparing.find(input) != string::npos) return true; return false;
    }

    static bool startsWithStr(string input, string comparing) {
        CharLowerBuffA(&input[0], (DWORD)input.size());
        CharLowerBuffA(&comparing[0], (DWORD)comparing.size());
        if (comparing.find(input) == 0) return true; return false;
    }

    static bool hasType(int input, Storage* comparing) {

        //Explicitne zabezpecene typy zahranicia a korena
        if (comparing->getData(1) == "9") {
            if (input == 1) return true; return false;
        }

        if (comparing->getData(1) == "SKZZZ") {
            if (input == 2) return true; return false;
        }

        if (comparing->getData(1) == "SKZZZZZZZZZZ") {
            if (input == 3) return true; return false;
        }

        if (comparing->getData(1) == "SKZZZZ") {
            if (input == 2 && comparing->getData(0) == "81") return true;
            if (input == 3 && comparing->getData(0) == "2929") return true;
            return false;
        }

        if (comparing->getData(0) == "0") {
            return false;
        }


        switch (input)
        {
        case 1: {
            if (comparing->getData(1).length() == 1) return true; return false;
            break;
        }
        case 2: {
            if (comparing->getData(1).length() == 6) return true; return false;
            break;
        }
        case 3: {
            if (comparing->getData(1).length() == 12) return true; return false;
            break;
        }
        default:
            break;
        }
        return false;
    }
};

