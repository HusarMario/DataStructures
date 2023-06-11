#pragma once
#include <libds/amt/implicit_sequence.h>
#include <libds/adt/sorts.h>
#include <functional>
#include <iterator>
#include <WinUser.h>
#include "Storage.h"

using namespace std;
class Sorter {
public:
	static void sortByComparator(function<bool(Storage*,Storage*)> compare, ds::amt::ImplicitSequence<Storage*>* output) {
		ds::adt::SelectSort<Storage*> sorter;
		sorter.sort(*output, compare);
	}
};