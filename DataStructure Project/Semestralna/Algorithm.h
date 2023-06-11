#pragma once
#include <libds/amt/implicit_sequence.h>
#include <functional>
#include <iterator>
#include <WinUser.h>

using namespace std;
class Algorithm {	
public :
	template<typename Iterator>
	static void outputByPredicate(function<bool(Storage*)> predicate, Iterator begin, Iterator end, ds::amt::ImplicitSequence<Storage*>* output) {
		
		for (auto& search = begin; search != end; ++search) {
			auto& actual = *search;

			if (predicate(actual)) {
				output->insertLast().data_ = actual;
			}
		}
	}
};