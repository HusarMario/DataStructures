#include "Data.h"

Data::Data()
{
	//priprava sekvencii a hierarchie
	kraje = new ds::amt::ImplicitSequence<Storage*>;
	okresy = new ds::amt::ImplicitSequence<Storage*>;
	obce = new ds::amt::ImplicitSequence<Storage*>;
	slovensko = new ds::amt::MultiWayExplicitHierarchy<Storage*>;
	tabulkaKrajov = new ds::adt::SortedSequenceTable<string, ds::amt::ImplicitSequence<Storage*>*>;
	tabulkaOkresov = new ds::adt::SortedSequenceTable<string, ds::amt::ImplicitSequence<Storage*>*>;
	tabulkaObci = new ds::adt::SortedSequenceTable<string, ds::amt::ImplicitSequence<Storage*>*>;

	//priprava readerov jednotlivych csv suborov
	Reader* readerKraje = new Reader("kraje.csv");
	Reader* readerOkresy = new Reader("okresy.csv");
	Reader* readerObce = new Reader("obce.csv");

	//preskocenie prvych riadkov suborov
	readerKraje->giveNext();
	readerOkresy->giveNext();
	readerObce->giveNext();

	//vytvorenie root udaja pre Slovensku republiku
	auto root = &slovensko->emplaceRoot();
	root->data_ = new Storage();
	root->data_->putData("0");
	root->data_->putData("0");
	root->data_->putData("Slovenská republika");
	root->data_->putData(" ");
	root->data_->putData("Slovnesko");
	root->data_->putData("SK");

	//pripravenie premennych pre dynamicke nacitavanie zo vsetkych suborov naraz
	int krajeIndex = 0;
	int okresyIndex = 0;
	int obceIndex = 0;

	auto lastKraj = root;
	auto lastOkres = root;
	auto lastObec = root;

	//nacitanie obce
	while (true)
	{
		string next = readerObce->giveNext();
		if (next.empty()) {
			break;
		}

		this->obce->insertLast().data_ = new Storage();
		stringstream stream(next);
		string singleData;
		while (getline(stream, singleData, ';')) {
			this->obce->accessLast()->data_->putData(singleData);
		}
		// pridanie do tabulkyObci
		if (!this->tabulkaObci->contains(this->obce->accessLast()->data_->getData(2))) {
			tabulkaObci->insert(this->obce->accessLast()->data_->getData(2), new ds::amt::ImplicitSequence<Storage*>);
		}
		tabulkaObci->find(this->obce->accessLast()->data_->getData(2))->insertLast().data_ = this->obce->accessLast()->data_;

		//ukoncenie nacitavania pri najdeni zahranicia
		if (this->obce->accessLast()->data_->getData(1).find("SKZZZZ") == 0) {
			break;
		}

		//nacitanie okresy
		if (okresy->isEmpty() || obce->accessLast()->data_->getData(1).find(okresy->accessLast()->data_->getData(1)) != 0) {

			string next = readerOkresy->giveNext();
			if (next.empty()) {
				break;
			}

			this->okresy->insertLast().data_ = new Storage();
			stringstream stream(next);
			string singleData;
			while (getline(stream, singleData, ';')) {
				this->okresy->accessLast()->data_->putData(singleData);
			}

			// pridanie do tabulkyOkresov
			if (!this->tabulkaOkresov->contains(this->okresy->accessLast()->data_->getData(2))) {
				tabulkaOkresov->insert(this->okresy->accessLast()->data_->getData(2), new ds::amt::ImplicitSequence<Storage*>);
			}
			tabulkaOkresov->find(this->okresy->accessLast()->data_->getData(2))->insertLast().data_ = this->okresy->accessLast()->data_;

			// nacitanie kraje
			if (kraje->isEmpty() || okresy->accessLast()->data_->getData(5).find(kraje->accessLast()->data_->getData(1)) != 0) {

				string next = readerKraje->giveNext();
				if (next.empty()) {
					break;
				}

				this->kraje->insertLast().data_ = new Storage();
				stringstream stream(next);
				string singleData;
				while (getline(stream, singleData, ';')) {
					this->kraje->accessLast()->data_->putData(singleData);
				}

				// pridanie do tabulkyKrajov
				if (!this->tabulkaKrajov->contains(this->kraje->accessLast()->data_->getData(2))) {
					tabulkaKrajov->insert(this->kraje->accessLast()->data_->getData(2), new ds::amt::ImplicitSequence<Storage*>);
				}
				tabulkaKrajov->find(this->kraje->accessLast()->data_->getData(2))->insertLast().data_ = this->kraje->accessLast()->data_;

				lastKraj = &slovensko->emplaceSon(*root, krajeIndex++);
				lastKraj->data_ = kraje->accessLast()->data_;
				okresyIndex = 0;
			}

			lastOkres = &slovensko->emplaceSon(*lastKraj, okresyIndex++);
			lastOkres->data_ = okresy->accessLast()->data_;
			obceIndex = 0;
		}

		lastObec = &slovensko->emplaceSon(*lastOkres, obceIndex++);
		lastObec->data_ = obce->accessLast()->data_;
	}

	//zahranicny kraj
	string zahranicie = readerKraje->giveNext();
	this->kraje->insertLast().data_ = new Storage();
	stringstream stream(zahranicie);
	string singleData;
	while (getline(stream, singleData, ';')) {
		this->kraje->accessLast()->data_->putData(singleData);
	}
	lastKraj = &slovensko->emplaceSon(*root, krajeIndex++);
	lastKraj->data_ = kraje->accessLast()->data_;
	okresyIndex = 0;
	// pridanie do tabulkyKrajov
	if (!this->tabulkaKrajov->contains(this->kraje->accessLast()->data_->getData(2))) {
		tabulkaKrajov->insert(this->kraje->accessLast()->data_->getData(2), new ds::amt::ImplicitSequence<Storage*>);
	}
	tabulkaKrajov->find(this->kraje->accessLast()->data_->getData(2))->insertLast().data_ = this->kraje->accessLast()->data_;

	//zahranicny okres1
	zahranicie = readerOkresy->giveNext();
	this->okresy->insertLast().data_ = new Storage();
	stringstream stream2(zahranicie);
	while (getline(stream2, singleData, ';')) {
		this->okresy->accessLast()->data_->putData(singleData);
	}
	lastOkres = &slovensko->emplaceSon(*lastKraj, okresyIndex++);
	lastOkres->data_ = okresy->accessLast()->data_;
	obceIndex = 0;

	// pridanie do tabulkyOkresov
	if (!this->tabulkaOkresov->contains(this->okresy->accessLast()->data_->getData(2))) {
		tabulkaOkresov->insert(this->okresy->accessLast()->data_->getData(2), new ds::amt::ImplicitSequence<Storage*>);
	}
	tabulkaOkresov->find(this->okresy->accessLast()->data_->getData(2))->insertLast().data_ = this->okresy->accessLast()->data_;

	//zahranicna obec1
	lastObec = &slovensko->emplaceSon(*lastOkres, obceIndex++);
	lastObec->data_ = obce->accessLast()->data_;


	//zahranicny okres2
	zahranicie = readerOkresy->giveNext();
	this->okresy->insertLast().data_ = new Storage();
	stringstream stream3(zahranicie);
	while (getline(stream3, singleData, ';')) {
		this->okresy->accessLast()->data_->putData(singleData);
	}
	lastOkres = &slovensko->emplaceSon(*lastKraj, okresyIndex++);
	lastOkres->data_ = okresy->accessLast()->data_;
	obceIndex = 0;

	// pridanie do tabulkyOkresov
	if (!this->tabulkaOkresov->contains(this->okresy->accessLast()->data_->getData(2))) {
		tabulkaOkresov->insert(this->okresy->accessLast()->data_->getData(2), new ds::amt::ImplicitSequence<Storage*>);
	}
	tabulkaOkresov->find(this->okresy->accessLast()->data_->getData(2))->insertLast().data_ = this->okresy->accessLast()->data_;

	//zahranicna obec2
	zahranicie = readerObce->giveNext();
	this->obce->insertLast().data_ = new Storage();
	stringstream stream4(zahranicie);
	while (getline(stream4, singleData, ';')) {
		this->obce->accessLast()->data_->putData(singleData);
	}
	lastObec = &slovensko->emplaceSon(*lastOkres, obceIndex++);
	lastObec->data_ = obce->accessLast()->data_;

	// pridanie do tabulkyObci
	if (!this->tabulkaObci->contains(this->obce->accessLast()->data_->getData(2))) {
		tabulkaObci->insert(this->obce->accessLast()->data_->getData(2), new ds::amt::ImplicitSequence<Storage*>);
	}
	tabulkaObci->find(this->obce->accessLast()->data_->getData(2))->insertLast().data_ = this->obce->accessLast()->data_;

	//nastavenie iterator na zaciatok hierarchie
	iterator = slovensko->accessRoot();

	//uzatvorenie readerov
	readerKraje->close();
	readerOkresy->close();
	readerObce->close();

	//zavolanie destruktorov pre readery
	delete readerKraje;
	delete readerOkresy;
	delete readerObce;


	//bonusove data
	Reader* readerBonus = new Reader("bonus.csv");
	while (true)
	{
		string next = readerBonus->giveNext();
		if (next.empty()) {
			break;
		}

		int split = next.find(";");			//useknutie nazvu
		int data = split;					//useknutie cisla

		// rovnake obce maju v csv subore za ciarkou napisany okres, ten ale mozem odrezat pretoze v tabulke ich podla okresov uz mam zoradene
		if (next.find(",") != string::npos) {
			split = next.find(",");
		}

		if (tabulkaKrajov->contains(next.substr(0, split))) {
			auto sekvencia = tabulkaKrajov->find(next.substr(0, split));
			for (int i = 0; i < sekvencia->size(); i++)
			{
				if (sekvencia->access(i)->data_->sizeOfStorage() == 6) {
					sekvencia->access(i)->data_->putData(next.substr(data + 1, next.size()));
				}
			}
		}

		if (tabulkaOkresov->contains(next.substr(0, split))) {
			auto sekvencia = tabulkaOkresov->find(next.substr(0, split));
			for (int i = 0; i < sekvencia->size(); i++)
			{
				if (sekvencia->access(i)->data_->sizeOfStorage() == 6) {
					sekvencia->access(i)->data_->putData(next.substr(data + 1, next.size()));
				}
			}
		}

		if (tabulkaObci->contains(next.substr(0, split))) {
			auto sekvencia = tabulkaObci->find(next.substr(0, split));
			for (int i = 0; i < sekvencia->size(); i++)
			{
				if (sekvencia->access(i)->data_->sizeOfStorage() == 6) {
					sekvencia->access(i)->data_->putData(next.substr(data + 1, next.size()));
				}
			}
		}
	}
	readerBonus->close();
	delete readerBonus;
}

Data::~Data()
{
	for (auto x = kraje->begin(); x != kraje->end(); x++) {
		delete* x;
	}
	delete kraje;
	for (auto x = okresy->begin(); x != okresy->end(); x++) {
		delete* x;
	}
	delete okresy;
	for (auto x = obce->begin(); x != obce->end(); x++) {
		delete* x;
	}
	delete obce;

	delete slovensko->accessRoot()->data_;
	delete slovensko;

	//vymazava vytvorene sekvencie nie kopie dat (obsah sekvencii je prazdny pretoze jednotlive data boli mazane vyssie)
	for (auto x = tabulkaKrajov->begin(); x != tabulkaKrajov->end(); x++)
	{
		delete (*x).data_;
	}
	delete tabulkaKrajov;

	for (auto x = tabulkaOkresov->begin(); x != tabulkaOkresov->end(); x++)
	{
		delete (*x).data_;
	}
	delete tabulkaOkresov;

	for (auto x = tabulkaObci->begin(); x != tabulkaObci->end(); x++)
	{
		delete (*x).data_;
	}
	delete tabulkaObci;
}

ds::amt::ImplicitSequence<Storage*>::ImplicitSequenceIterator Data::getBeginKraje()
{
	return kraje->begin();
}

ds::amt::ImplicitSequence<Storage*>::ImplicitSequenceIterator Data::getEndKraje()
{
	return kraje->end();
}

ds::amt::ImplicitSequence<Storage*>::ImplicitSequenceIterator Data::getBeginOkresy()
{
	return okresy->begin();
}

ds::amt::ImplicitSequence<Storage*>::ImplicitSequenceIterator Data::getEndOkresy()
{
	return okresy->end();
}

ds::amt::ImplicitSequence<Storage*>::ImplicitSequenceIterator Data::getBeginObce()
{
	return obce->begin();
}

ds::amt::ImplicitSequence<Storage*>::ImplicitSequenceIterator Data::getEndObce()
{
	return obce->end();
}

ds::amt::MultiWayExplicitHierarchy<Storage*>::PreOrderHierarchyIterator Data::getBeginSlovensko()
{
	return slovensko->beginPre();
}

ds::amt::MultiWayExplicitHierarchy<Storage*>::PreOrderHierarchyIterator Data::getEndSlovensko()
{
	return slovensko->endPre();
}

ds::amt::MultiWayExplicitHierarchy<Storage*>::PreOrderHierarchyIterator Data::getBeginHierarchy()
{
	return ds::amt::MultiWayExplicitHierarchy<Storage*>::PreOrderHierarchyIterator(slovensko, iterator);
}

ds::amt::MultiWayExplicitHierarchy<Storage*>::PreOrderHierarchyIterator Data::getEndHierarchy()
{
	return ds::amt::MultiWayExplicitHierarchy<Storage*>::PreOrderHierarchyIterator(slovensko, nullptr);
}

void Data::hierarchyPrintInfo()
{
	switch (slovensko->level(*iterator))
	{
	case 0: cout << "Nachádzaš sa v koreni : "; break;
	case 1: cout << "Nachádzaš sa v kraji : "; break;
	case 2: cout << "Nachádzaš sa v okrese : "; break;
	case 3: cout << "Nachádzaš sa v obci : "; break;
	default:
		break;
	}
	cout << iterator->data_->getData(2) << endl;
}

void Data::hierarchyprintOptionsIterator()
{
	int index = 1;
	for (auto son : *iterator->sons_)
	{
		cout << index << " : " << son->data_->getData(2) << endl;
		index++;
	}
}

void Data::hierarchygetDeeper(int indexOfSon)
{
	if (hierarchycanGetDeeper()) {
		if (indexOfSon >= iterator->sons_->size()) {
			cout << "Index synov bol presiahnuty." << endl;
			return;
		}


		iterator = slovensko->accessSon(*iterator, indexOfSon);
		cout << endl;
	}
}

void Data::hierarchyreturnUp()
{
	if (hierarchycanReturnUp()) {
		iterator = slovensko->accessParent(*iterator);
		cout << endl;
	}
}

void Data::tabulkaPrintAll(string pattern, int type)
{
	switch (type)
	{
	case 1: {
		if (tabulkaKrajov->contains(pattern)) {
			for (auto x = tabulkaKrajov->find(pattern)->begin(); x != tabulkaKrajov->find(pattern)->end(); x++)
			{
				(*x)->printData();
			}
		}
		else {
			cout << "Takýto kraj sa v zozname nenachádza." << endl;
		}
		break;
	}
	case 2: {
		if (tabulkaOkresov->contains(pattern)) {
			for (auto x = tabulkaOkresov->find(pattern)->begin(); x != tabulkaOkresov->find(pattern)->end(); x++)
			{
				(*x)->printData();
			}
		}
		else {
			cout << "Takýto okres sa v zozname nenachádza." << endl;
		}
		break;
	}
	case 3: {
		if (tabulkaObci->contains(pattern)) {
			for (auto x = tabulkaObci->find(pattern)->begin(); x != tabulkaObci->find(pattern)->end(); x++)
			{
				(*x)->printData();
			}
		}
		else {
			cout << "Takáto obec sa v zozname nenachádza." << endl;
		}
		break;
	}
	default:
		break;
	}
}

bool Data::hierarchycanGetDeeper()
{
	if (slovensko->level(*iterator) != 3) 
		return true; 
	cout << "Nemôžeš ís hlbšie." << endl;
	return false;
}

bool Data::hierarchycanReturnUp()
{
	if (slovensko->level(*iterator) != 0) 
		return true; 
	cout << "Nemôžeš ís vyššie." << endl;
	return false;
}
