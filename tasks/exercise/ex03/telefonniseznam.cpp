#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <array>
#include <cassert>
using namespace std;
#endif /* __PROGTEST__ */

class Person {
    public:
        string name;
        string surname;
        unsigned long number;

        Person(string newName, string newSurname, long newNumber) {
            name = newName;
            surname = newSurname;
            number = newNumber;
        }
};

bool report (const string & fileName, ostream & out) {
    fstream filestream(fileName, ios::in);

    if (filestream.fail()) {
        return false;
    }

    vector<Person> personArray;

    bool readingData = true;

	while(true) {
        if (readingData) {
            string line;
            getline(filestream, line);

            if (line.empty()) {
                readingData = false;
                continue;
            }

            stringstream ss;
            ss << line;

            string name;
            string surname;
            unsigned long number;

            string buffer;

            if (!(ss >> name >> surname >> number) 
            || number < 100000000 
            || number > 999999999
            || ss >> buffer) {
                break;
            }

            Person newPerson(name, surname, number);
            personArray.push_back(newPerson);
        } else {
            for(Person & person : personArray) {
                cout << person.name << " " << person.surname << " " << person.number << endl;
            }
            break;
        }

    }
    filestream.close();
    return false;
}

#ifndef __PROGTEST__
int main() {
    ostringstream oss;
    report("tests/test0_in.txt", oss);
    /*
    ostringstream oss;
    oss.str("");
    assert(report("tests/test0_in.txt", oss) == true);
    assert(oss.str() ==
	    "John Christescu 258452362\n"
	    "John Harmson 861647702\n"
	    "-> 2\n"
	    "-> 0\n"
	    "Josh Dakhov 264112084\n"
	    "Dakhov Speechley 865216101\n"
	    "-> 2\n"
	    "John Harmson 861647702\n"
	    "-> 1\n");
    oss.str("");
    assert(report("tests/test1_in.txt", oss) == false);
    */
    return 0;
}
#endif /* __PROGTEST__ */
