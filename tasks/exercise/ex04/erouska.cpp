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


class CDate {
public:
    const int year;
    const int month;
    const int day;

    CDate(unsigned int y, unsigned int m, unsigned int d): year(y), month(m), day(d) {}

    bool isEqual(CDate & otherDate) const {
        return ((year == otherDate.year) && (month == otherDate.month) && (day == otherDate.day));
    }

    bool operator == (CDate & rhs) const {
        return isEqual(rhs);
    }

    bool operator > (CDate & rhs) const {
        
        if (year == rhs.year) {
            if (month == rhs.month) {
                if (day == rhs.day) {
                    return false;
                }
                return day > rhs.day;
            }
            return month > rhs.month;
        }
        return year > rhs.year;
    }
};

class CTime {
public:
    const int hour;
    const int minute;
    const int second;

    CTime(unsigned int h, unsigned int m, unsigned int s): hour(h), minute(m), second(s) {}

    bool isEqual(CTime & otherTime) const {
        return ((hour == otherTime.hour) && (minute == otherTime.minute) && (second == otherTime.second));
    }

    bool operator == (CTime & rhs) const {
        return isEqual(rhs);
    }

    bool operator > (CTime& rhs) const {

        if (hour == rhs.hour) {
            if (minute == rhs.minute) {
                if (second == rhs.second) {
                    return false;
                }
                return second > rhs.second;
            }
            return minute > rhs.minute;
        }
        return hour > rhs.hour;
    }
};

class CTimeStamp {
public:
    CTime time;
    CDate date;

    CTimeStamp(unsigned int y, unsigned int mo, unsigned int d, unsigned int h, unsigned int mi, unsigned int s) : time(h, mi, s), date(y, mo, d) {}

    bool isEqual(CTimeStamp & otherTimestamp) const {
        return ((time == otherTimestamp.time) && (date == otherTimestamp.date));
    }

    bool operator == (CTimeStamp & rhs) const {
        return isEqual(rhs);
    }

    bool operator >= (CTimeStamp & rhs) const {

        if (date == rhs.date) {
            if (time == rhs.time) {
                return true;
            }
            return time > rhs.time;
        }
        return date > rhs.date;
    }

    bool operator >= (const CTimeStamp & rhs) const {
        return (*this) >= const_cast<CTimeStamp&>(rhs);
    }
};

class CContact {
    CTimeStamp timestamp;
    const unsigned long person1;
    const unsigned long person2;

    static size_t nextUniqueId;

public:
    CContact(const CTimeStamp& ts, unsigned long p1, unsigned long p2) : timestamp(ts), person1(p1), person2(p2) {}

    bool containsPerson(unsigned int p) const {
        return ((person1 == p) || (person2 == p));
    }

    bool containsPerson(unsigned int p, const CTimeStamp& fromT, const CTimeStamp& toT) {
        return (containsPerson(p) && (timestamp >= fromT && toT >= timestamp));
    }

    unsigned long otherPerson(unsigned int to) const {
        if (to != person1 && to == person2) {
            return person1;
        } else if (to == person1 && to != person2) {
            return person2;
        }

        throw "No other person";
    }
};

size_t CContact::nextUniqueId = 0;

class CEFaceMask {
    vector<CContact> contacts;

    void addPersonToList(CContact & contact, long unsigned person, vector<int> & list) {
        try {
            unsigned long otherPerson = contact.otherPerson(person);
            if (find(list.begin(), list.end(), otherPerson) == list.end()) {
                list.push_back(otherPerson);
            }
        }
        catch (...) {}
    }

    vector<int> processContacts(long unsigned person, CTimeStamp fromT = CTimeStamp(0, 0, 0, 0, 0, 0), CTimeStamp toT = CTimeStamp(0, 0, 0, 0, 0, 0), bool withTimestamps = false) {
        vector<int> listOfContacts;
        for (auto& contact : contacts) {
            if (withTimestamps) {
                if (contact.containsPerson(person, fromT, toT)) {
                    addPersonToList(contact, person, listOfContacts);
                }
            } else {
                if (contact.containsPerson(person)) {
                    addPersonToList(contact, person, listOfContacts);
                }
            }
        }
        return listOfContacts;
    }

public:
    CEFaceMask & addContact(const CContact & contact) {
        contacts.push_back(contact);
        return *this;
    }

    vector<int> listContacts(unsigned long person) {
        return processContacts(person);
    }

    vector<int> listContacts(unsigned long person, CTimeStamp fromT, CTimeStamp toT) {
        return processContacts(person, fromT, toT, true);
    }

};

#ifndef __PROGTEST__
int main() {
    CEFaceMask test;

    test.addContact(CContact(CTimeStamp(2021, 1, 10, 12, 40, 10), 123456789, 999888777));
    test.addContact(CContact(CTimeStamp(2021, 1, 12, 12, 40, 10), 123456789, 111222333))
        .addContact(CContact(CTimeStamp(2021, 2, 5, 15, 30, 28), 999888777, 555000222));
    test.addContact(CContact(CTimeStamp(2021, 2, 21, 18, 0, 0), 123456789, 999888777));
    test.addContact(CContact(CTimeStamp(2021, 1, 5, 18, 0, 0), 123456789, 456456456));
    test.addContact(CContact(CTimeStamp(2021, 2, 1, 0, 0, 0), 123456789, 123456789));
    assert(test.listContacts(123456789) == (vector<int>{999888777, 111222333, 456456456}));
    assert(test.listContacts(999888777) == (vector<int>{123456789, 555000222}));
    assert(test.listContacts(191919191) == (vector<int>{}));
    assert(test.listContacts(123456789, CTimeStamp(2021, 1, 5, 18, 0, 0), CTimeStamp(2021, 2, 21, 18, 0, 0)) == (vector<int>{999888777, 111222333, 456456456}));
    assert(test.listContacts(123456789, CTimeStamp(2021, 1, 5, 18, 0, 1), CTimeStamp(2021, 2, 21, 17, 59, 59)) == (vector<int>{999888777, 111222333}));
    assert(test.listContacts(123456789, CTimeStamp(2021, 1, 10, 12, 41, 9), CTimeStamp(2021, 2, 21, 17, 59, 59)) == (vector<int>{111222333}));
    return 0;
}
#endif /* __PROGTEST__ */
