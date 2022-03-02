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

/**
 * @brief Class representing person entity
 * 
 */
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

/**
 * @brief Reads person entries from file and performs search on their name or surname
 * 
 * Each person needs to be on one line, with name, surname and number in succesion.
 * After empty line requests follow.
 * Request is one word to look for (either name or surname).
 * Output is each entry that matches specified request and number of entries that match
 * 
 * @param fileName File to look in
 * @param out Stream to output to
 * @return true All requests have been fulfilled
 * @return false Failed, either wrong file, or error in file formatting 
 */
bool report (const string & fileName, ostream & out) {
    
    fstream filestream(fileName, ios::in);
    if (filestream.fail()) {
        return false;
    }

    vector<Person> personArray;
    bool readingData = true;

	while(true) {
        if (filestream.peek() == EOF) {
            filestream.close();
            return true;
        }

        string line;
        getline(filestream, line);
        stringstream linestream;

        string buffer;

        if (readingData) { /* Read persons from file and add them to vector */
            if (line.empty()) { /* If found empty line, switch to request mode */
                readingData = false;
                continue;
            }

            linestream << line;

            string name;
            string surname;
            unsigned long number;

            if (!(linestream >> name >> surname >> number)
            || number < 100000000 
            || number > 999999999
                || linestream >> buffer) { /* Check if input is valid */
                break;
            }

            Person newPerson(name, surname, number);
            personArray.push_back(newPerson);
        } else { /* Fulfull search requests */
            linestream << line;

            string query;
            if (!(linestream >> query)) { /* Check for empty line */
                out << "-> 0" << endl;
                continue;
            }
            if (linestream >> buffer) { /* Check for valid input */
                break;
            }

            int matches = 0;

            for(Person & person : personArray) { /* Iterate through vector and print matches */
                if (person.name == query || person.surname == query) {
                    out << person.name << " " << person.surname << " " << person.number << endl;
                    matches++;
                }
            }
            out << "-> " << matches << endl;
        }
    }
    filestream.close();
    return false;
}

#ifndef __PROGTEST__
int main() {
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
    return 0;
}
#endif /* __PROGTEST__ */
