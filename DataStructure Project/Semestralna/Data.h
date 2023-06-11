#pragma once

#include <libds/amt/explicit_hierarchy.h>
#include <libds/amt/implicit_sequence.h>
#include <libds/adt/table.h>
#include "Storage.h"
#include "Reader.h"
#include <iostream>
#include <sstream>

class Data
{
private:
	ds::amt::ImplicitSequence<Storage*>* kraje;
	ds::amt::ImplicitSequence<Storage*>* okresy;
	ds::amt::ImplicitSequence<Storage*>* obce;
	ds::amt::MultiWayExplicitHierarchy<Storage*>* slovensko;
	ds::amt::MultiWayExplicitHierarchyBlock<Storage*>* iterator;
	ds::adt::SortedSequenceTable<string, ds::amt::ImplicitSequence<Storage*>*>* tabulkaKrajov;
	ds::adt::SortedSequenceTable<string, ds::amt::ImplicitSequence<Storage*>*>* tabulkaOkresov;
	ds::adt::SortedSequenceTable<string, ds::amt::ImplicitSequence<Storage*>*>* tabulkaObci;
	bool hierarchycanGetDeeper();
	bool hierarchycanReturnUp();
public:
	Data();
	~Data();
	ds::amt::ImplicitSequence<Storage*>::ImplicitSequenceIterator getBeginKraje();
	ds::amt::ImplicitSequence<Storage*>::ImplicitSequenceIterator getEndKraje();
	ds::amt::ImplicitSequence<Storage*>::ImplicitSequenceIterator getBeginOkresy();
	ds::amt::ImplicitSequence<Storage*>::ImplicitSequenceIterator getEndOkresy();
	ds::amt::ImplicitSequence<Storage*>::ImplicitSequenceIterator getBeginObce();
	ds::amt::ImplicitSequence<Storage*>::ImplicitSequenceIterator getEndObce();
	ds::amt::MultiWayExplicitHierarchy<Storage*>::PreOrderHierarchyIterator getBeginSlovensko();
	ds::amt::MultiWayExplicitHierarchy<Storage*>::PreOrderHierarchyIterator getEndSlovensko();
	ds::amt::MultiWayExplicitHierarchy<Storage*>::PreOrderHierarchyIterator getBeginHierarchy();
	ds::amt::MultiWayExplicitHierarchy<Storage*>::PreOrderHierarchyIterator getEndHierarchy();
	void hierarchyPrintInfo();
	void hierarchyprintOptionsIterator();
	void hierarchygetDeeper(int indexOfSon);
	void hierarchyreturnUp();
	void tabulkaPrintAll(string pattern, int type);
	
};

