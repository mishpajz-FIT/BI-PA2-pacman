/**
 * @file erouskav2.cpp
 * @author Michal Dobeš
 * @date 2022-03-31
 *
 * @brief Implementation of timestamp class, contact class (represents contact between two persons and timestamp) and facemask class, which is used for managing contacts
 * Implements search for the most occuring value in vector of pairs, pairs need to be unique
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <numeric>
#include <vector>
#include <array>
#include <set>
#include <map>
#include <deque>
#include <queue>
#include <stack>
#include <cassert>
using namespace std;
#endif /* __PROGTEST__ */

/**
 * @brief Class for storing and comparing dates
 *
 * Values are not sanitized
 */
class CDate {
public:
    int year;
    int month;
    int day;

    CDate(unsigned int y, unsigned int m, unsigned int d) : year(y), month(m), day(d) { }

    bool operator == (const CDate & rhs) const {
        return tie(year, month, day) == tie(rhs.year, rhs.month, rhs.day);
    }

    bool operator < (const CDate & rhs) const {
        return tie(year, month, day) < tie(rhs.year, rhs.month, rhs.day);
    }
};

/**
 * @brief Class for stroring and comparing time
 *
 * Values are not sanitized
 *
 */
class CTime {
public:
    int hour;
    int minute;
    int second;

    CTime(unsigned int h, unsigned int m, unsigned int s) : hour(h), minute(m), second(s) { }

    bool operator == (const CTime & rhs) const {
        return tie(hour, minute, second) == tie(rhs.hour, rhs.minute, rhs.second);
    }

    bool operator < (const CTime & rhs) const {
        return tie(hour, minute, second) < tie(rhs.hour, rhs.minute, rhs.second);
    }
};

/**
 * @brief Class for storing and comparing date and time
 *
 */
class CTimeStamp {
public:
    CTime time;
    CDate date;

    CTimeStamp(unsigned int y, unsigned int mo, unsigned int d, unsigned int h, unsigned int mi, unsigned int s) : time(h, mi, s), date(y, mo, d) { }

    bool operator == (const CTimeStamp & rhs) const {
        return tie(date, time) == tie(rhs.date, rhs.time);
    }

    bool operator < (const CTimeStamp & rhs) const {
        return tie(date, time) < tie(rhs.date, rhs.time);
    }

    bool operator <= (const CTimeStamp & rhs) const {
        return (*this < rhs) || (*this == rhs);
    }
};

/**
 * @brief Class representing contact between two persons, including date and time when this contact happend
 *
 */
class CContact {
    CTimeStamp timestamp;
    int person1; // ID of person1, should always be smaller than person 2
    int person2; // ID of person2

public:
    CContact(const CTimeStamp & ts, int p1, int p2) : timestamp(ts), person1(p1), person2(p2) {
        if (p1 > p2) {
            std::swap(person1, person2);
        }
    }

    int firstPerson() const {
        return person1;
    }

    int secondPerson() const {
        return person2;
    }

    CTimeStamp getTimestamp() const {
        return timestamp;
    }
};

class CEFaceMask {
    vector<CContact> contacts; // Storage of all contacts

public:
    CEFaceMask() { }

    /**
     * @brief Store new contact
     *
     * Note contact is not added if both persons in contact are not different
     *
     * @param contact Contact to store
     * @return CEFaceMask& this object
     */
    CEFaceMask & addContact(const CContact & contact) {
        if (contact.firstPerson() != contact.secondPerson()) {
            contacts.push_back(contact);
        }
        return *this;
    }

    /**
     * @brief Search through all contact between timestamps and get id of persons, that had most contacts
     *
     * Contact with two same persons is counted only once (even if happend at different time)
     *
     * @param from Timestamp to look from
     * @param to Tinmestamp to look to
     * @return vector<int>
     */
    vector<int> getSuperSpreaders(const CTimeStamp & from, const CTimeStamp & to) {
        vector<int> results; // Vector to return

        set<pair<int, int>> processedContacts; // Unique pairs of persons (contacts)
        for (auto & c : contacts) {
            if (from <= c.getTimestamp() && c.getTimestamp() <= to) { // Get all contacts between specified timestamps and create pairs out of persons in contact
                processedContacts.emplace(make_pair(c.firstPerson(), c.secondPerson())); // Insert pairs into set, so the contacts that are counted are uniqe and there are no duplicits
            }
        }

        map<int, unsigned int> processedPeople; // Create map and raise value for each person that occurs in some contact
        for (auto & t : processedContacts) {
            processedPeople[t.first] += 1;
            processedPeople[t.second] += 1;
        }

        unsigned int currentMax = 1;
        for (auto & p : processedPeople) { // Find largest values in map and store them into result vector
            if (p.second >= currentMax) {
                if (p.second > currentMax) {
                    results.clear();
                    currentMax = p.second;
                }
                results.push_back(p.first);
            }
        }

        return results;
    }
};

#ifndef __PROGTEST__
int main() {
    CEFaceMask test;

    test.addContact(CContact(CTimeStamp(2021, 1, 10, 12, 40, 10), 111111111, 222222222));
    test.addContact(CContact(CTimeStamp(2021, 1, 12, 12, 40, 10), 333333333, 222222222)).addContact(CContact(CTimeStamp(2021, 2, 14, 15, 30, 28), 222222222, 444444444));
    test.addContact(CContact(CTimeStamp(2021, 2, 15, 18, 0, 0), 555555555, 444444444));
    assert(test.getSuperSpreaders(CTimeStamp(2021, 1, 1, 0, 0, 0), CTimeStamp(2022, 1, 1, 0, 0, 0)) == (vector<int> {222222222}));
    test.addContact(CContact(CTimeStamp(2021, 3, 20, 18, 0, 0), 444444444, 666666666));
    test.addContact(CContact(CTimeStamp(2021, 3, 25, 0, 0, 0), 111111111, 666666666));
    assert(test.getSuperSpreaders(CTimeStamp(2021, 1, 1, 0, 0, 0), CTimeStamp(2022, 1, 1, 0, 0, 0)) == (vector<int> {222222222, 444444444}));
    return 0;
}
#endif /* __PROGTEST__ */
