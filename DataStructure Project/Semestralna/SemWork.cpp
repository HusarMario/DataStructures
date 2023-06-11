#include <libds/heap_monitor.h>
#include <iostream>
#include "Predicates.h"
#include "Comparators.h"
#include "Algorithm.h"
#include "Sorter.h"
#include <strsafe.h>
#include "Data.h"
#include <complexities/complexity_analyzer.h>
 
bool mainControl(Data* data);
bool sequenceControl(Data* data);
bool sequencePickControl(ds::amt::ImplicitSequence<Storage*>::ImplicitSequenceIterator begin, ds::amt::ImplicitSequence<Storage*>::ImplicitSequenceIterator end);
bool hierarchyControl(Data* data);
bool hierarchyPickControl(ds::amt::MultiWayExplicitHierarchy<Storage*>::PreOrderHierarchyIterator begin, ds::amt::MultiWayExplicitHierarchy<Storage*>::PreOrderHierarchyIterator end);
bool tableControl(Data* data);
bool outputSequence(ds::amt::ImplicitSequence<Storage*>* output);

int main()
{
    initHeapMonitor();
    SetConsoleOutputCP(1250);
    SetConsoleCP(1250);

    Data* data = new Data();

    while (mainControl(data)) {
        //Vykonavanie kodu v mainControl
    }

    delete data;

    
}

bool mainControl(Data* data) {
    cout << "Vyber si z možonstí ovládania semestrálnej práce: " << endl;
    cout << "0 = Ukonči program. " << endl;
    cout << "1 = Práca so sekvenciou (1.časť semestrálnej práce): " << endl;
    cout << "2 = Práca s hierarchiou (2.časť semestrálnej práce): " << endl;
    cout << "3 = Hľadať skrz tabuľku (3.časť semestrálnej práce): " << endl;
    cout << endl;

    int index = 0;
    cin >> index;

    switch (index)
    {
    case 0: {
        return false;
        break;
    }
    case 1: {
        while (sequenceControl(data))
        {
            //vykonavanie sequenceControl
        }
        break;
    }
    case 2: {
        while (hierarchyControl(data)) 
        {
            //vykonavanie hierarchyControl
        }
        break;
    }
    case 3: {
        while (tableControl(data)) 
        {
            //vykonavanie tableControl
        }
        break;
    }
    default:
        break;
    }
    return true;
}

bool sequenceControl(Data* data) {
    cout << "Vyber si z možností ovládania sekvencie: " << endl;
    cout << "0 = Vráť sa naspäť. " << endl;
    cout << "1 = Ovládanie krajov: " << endl;
    cout << "2 = Ovládanie okresov: " << endl;
    cout << "3 = Ovládanie obiec: " << endl;
    cout << endl;

    int index = 0;
    cin >> index;

    switch (index)
    {
    case 0: {
        return false;
        break;
    }
    case 1: {
        while (sequencePickControl(data->getBeginKraje(), data->getEndKraje()))
        {
            //vykonvanie vypisu krajov zo sekvencie
        }
        break;
    }
    case 2: {
        while (sequencePickControl(data->getBeginOkresy(), data->getEndOkresy()))
        {
            //vykonvanie vypisu okresov zo sekvencie
        }
        break;
    }
    case 3: {
        while (sequencePickControl(data->getBeginObce(), data->getEndObce()))
        {
            //vykonvanie vypisu obiec zo sekvencie
        }
        break;
    }
    default:
        break;
    }
    
    return true;
}

bool sequencePickControl(ds::amt::ImplicitSequence<Storage*>::ImplicitSequenceIterator begin, ds::amt::ImplicitSequence<Storage*>::ImplicitSequenceIterator end) {
    cout << "Vyber si z možností ovládania vybranej sekvencie: " << endl;
    cout << "0 = Vráť sa naspäť. " << endl;
    cout << "1 = Vypíš všetky prvky: " << endl;
    cout << "2 = Vypíš prvky začínajúce na: " << endl;
    cout << "3 = Vypíš prvky obsahujúce: " << endl;
    cout << "4 = Vypíš prvky s gymnáziom (bonusový predikát) : " << endl;
    cout << endl;

    int index = 0;
    cin >> index;

    switch (index)
    {
    case 0: {
        return false;
        break;
    }
    case 1: {   

        ds::amt::ImplicitSequence<Storage*>* output = new ds::amt::ImplicitSequence<Storage*>;
        Algorithm::outputByPredicate([](Storage* check)
            {
                return true;
            }, begin, end, output);
        outputSequence(output);
        delete output;
        break;


        
    }
    case 2: {
        string pattern;
        cin >> pattern;

        ds::amt::ImplicitSequence<Storage*>* output = new ds::amt::ImplicitSequence<Storage*>;
        Algorithm::outputByPredicate([pattern](Storage* check)
            {
                if (Predicates::startsWithStr(pattern, check->getData(2))) return true; return false;
            }, begin, end, output);

        outputSequence(output);
        delete output;
        break;
    }
    case 3: {
        string pattern;
        cin >> pattern;

        ds::amt::ImplicitSequence<Storage*>* output = new ds::amt::ImplicitSequence<Storage*>;
        Algorithm::outputByPredicate([pattern](Storage* check)
            {
                if (Predicates::containsStr(pattern, check->getData(2))) return true; return false;
            }, begin, end, output);

        outputSequence(output);
        delete output;
        break;
    }
    case 4: {
        ds::amt::ImplicitSequence<Storage*>* output = new ds::amt::ImplicitSequence<Storage*>;
        Algorithm::outputByPredicate([](Storage* check)
            {
                if (Predicates::hasGymnazium(check)) return true; return false;
            }, begin, end, output);
        outputSequence(output);
        delete output;
        break;
    }
    default:
        break;
    }

    return true;
}

bool outputSequence(ds::amt::ImplicitSequence<Storage*>* output) {
    
    cout << "0: Nezoradiť prvky" << endl;
    cout << "1: Zoradiť abecedne" << endl;
    cout << "2: Zoradiť podľa samohlások" << endl;
    cout << endl;

    int index = 0;
    cin >> index;
    switch (index)
    {
    case 0: {
        for (auto search = output->begin(); search != output->end(); ++search) {
            auto storage = *search;
            storage->printData();
        }
        cout << endl;
        break;
    }
    case 1: {
        Sorter::sortByComparator([](Storage* object1, Storage* object2)
            {
                if (Comparators::compareAlphabetical(object1, object2)) return true; return false;
            }, output);

        for (auto search = output->begin(); search != output->end(); ++search) {
            auto storage = *search;
            storage->printData();
        }
        cout << endl;
        break;
    }
    case 2: {
        Sorter::sortByComparator([](Storage* object1, Storage* object2)
            {
                if (Comparators::compareVowelsCount(object1, object2)) return true; return false;
            }, output);

        for (auto search = output->begin(); search != output->end(); ++search) {
            auto storage = *search;
            storage->printData();
        }
        cout << endl;
        break;
    }
    default:
        break;
    }



    return false;
}


bool hierarchyControl(Data* data) {
    data->hierarchyPrintInfo();
    cout << endl;
    cout << "Vyber si z možností ovládania hierarchie: " << endl;
    cout << "0 = Vráť sa naspäť." << endl;
    cout << "1 = Vypíš údaje podľa predikátov: " << endl;
    cout << "2 = Pokračuj hierarchiou skrz syna: " << endl;
    cout << "3 = Vráť sa naspäť na rodiča: " << endl;


    int index = 0;
    cin >> index;
    switch (index)
    {
    case 0: {
        return false;
        break;
    }
    case 1: {
        hierarchyPickControl(data->getBeginHierarchy(), data->getEndHierarchy());
        break;
    }
    case 2: {
        data->hierarchyprintOptionsIterator();
        int sonIndex;
        cin >> sonIndex;
        data->hierarchygetDeeper(sonIndex-1);
        break;
    }
    case 3: {
        data->hierarchyreturnUp();
        break;
    }
    default:
        break;
    }

    return true;
}


bool hierarchyPickControl(ds::amt::MultiWayExplicitHierarchy<Storage*>::PreOrderHierarchyIterator begin, ds::amt::MultiWayExplicitHierarchy<Storage*>::PreOrderHierarchyIterator end) {
    bool contain = false;
    bool starts = false;
    bool typD = false;
    bool gym = false;

    while (true) {
        cout << endl;
        cout << "Vyber používanie predikátov pre výpis (0 = false / 1 = true)" << endl;
        cout << "0 : Zruš a vráť sa do možností ovládania" << endl;
        cout << "1 : Obsahuje {string} = " << contain << endl;
        cout << "2 : Začína s {string} = " << starts << endl;
        cout << "3 : Je typom {kraj/okres/obec} = " << typD << endl;
        cout << "4 : Má aspoň 1 gymnázium (bonusový predikát) = " << gym << endl;
        cout << "5 : Vypíš údaje " << endl;

        int index = 0;
        cin >> index;

        if (index == 0) break;
        if (index == 1) { contain = !contain; continue; }
        if (index == 2) { starts = !starts; continue; }
        if (index == 3) { typD = !typD; continue; }
        if (index == 4) { gym = !gym; continue; }

        if (index == 5) {

            string containString;
            if (contain) {
                cout << "Načítaj string, ktorý musí obsahovať" << endl;
                getline(cin >> ws, containString);
            }

            string startsString;
            if (starts) {
                cout << "Načítaj string, ktorým musí začínať" << endl;
                getline(cin >> ws, startsString);
            }

            int typeOfData;
            if (typD) {
                cout << "Vyber typ územnej jednotky (1 = kraje, 2 = okresy, 3 = obce) .. 1 is default value" << endl;
                cin >> typeOfData;
                if (!(typeOfData > 0 && typeOfData <= 3)) {
                    typeOfData = 1;
                }
            }

            ds::amt::ImplicitSequence<Storage*>* output = new ds::amt::ImplicitSequence<Storage*>;
            Algorithm::outputByPredicate([&](Storage* check)
                {
                    return ((!contain || Predicates::containsStr(containString, check->getData(2))) &&
                        (!starts || Predicates::startsWithStr(startsString, check->getData(2))) &&
                        (!typD || Predicates::hasType(typeOfData, check)) &&
                        (!gym || Predicates::hasGymnazium(check)));
                }, begin, end, output);

           outputSequence(output);
           delete output;
        }
    }
    return false;
}


bool tableControl(Data* data) {
    cout << "Vyber si z možností ovládania tabuľky: " << endl;
    cout << "0 = Vráť sa naspäť." << endl;
    cout << "1 = Hľadaj kraj: " << endl;
    cout << "2 = Hľadaj okres: " << endl;
    cout << "3 = Hľadaj obec: " << endl;
    cout << endl;

    int index = 0;
    cin >> index;

    switch (index)
    {
    case 0: {
        return false;
        break;
    }
    case 1: {
        string pattern;
        getline(cin >> ws,pattern);
        data->tabulkaPrintAll(pattern, 1);
        break;
    }
    case 2: {
        string pattern;
        getline(cin >> ws, pattern);
        data->tabulkaPrintAll(pattern, 2);
        break;
    }
    case 3: {
        string pattern;
        getline(cin >> ws, pattern);
        data->tabulkaPrintAll(pattern, 3);
        break;
    }
    default:
        return false;
    }

    return true;
}