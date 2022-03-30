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
        return tie(year, month, day) == (rhs.year, rhs.month, rhs.day)
    }

    bool operator > (const CDate & rhs) const {
        return tie(year, month, day) > tie(rhs.year, rhs.month, rhs.day);
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
        return tie(hour, minute, second) == tie(otherTime.hour, otherTime.minute, otherTime.second);
    }

    bool operator > (const CTime & rhs) const {
        return tie(hour, minute, second) > tie(rhs.hour, rhs.minute, rhs.second);
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

    bool operator >= (const CTimeStamp & rhs) const {
        return tie(date, time) > tie(rhs.date, rhs.time);
    }
};

/**
 * @brief Class representing contact between two persons, including date and time when this contact happend
 *
 */
class CContact {
    const CTimeStamp timestamp;
    const unsigned long person1; // ID of person1
    const unsigned long person2; // ID of person2

public:
    CContact(const CTimeStamp & ts, int p1, int p2) : timestamp(ts), person1(p1), person2(p2) { }

    /**
     * @brief Method to determine if contact contains person id
     *
     * @param p Person id to look for
     * @return true Person is part of this contact
     * @return false Person is not part of this contact
     */
    bool containsPerson(unsigned int p) const {
        return ((person1 == p) || (person2 == p));
    }

    /**
     * @brief Method to determine if contact contains person id and is between specified timestamps
     *
     * @param p Person id to look for
     * @param fromT Timestamp to look from
     * @param toT Timestamp to look to
     * @return true Person is part of this contact
     * @return false Person is not part of this contact
     */
    bool containsPerson(unsigned int p, const CTimeStamp & fromT, const CTimeStamp & toT) const {
        return (containsPerson(p) && (timestamp >= fromT && toT >= timestamp));
    }

    /**
     * @brief Method to get the second person of this contact
     *
     * Throws if contact does not contain searched person, or if contact contains only searched person .
     *
     * @param to Person in this contact
     * @return unsigned long Person in this contact other than person in 'to'
     */
    unsigned long otherPerson(unsigned int to) const {
        if (to != person1 && to == person2) {
            return person1;
        } else if (to == person1 && to != person2) {
            return person2;
        }

        throw "No other person";
    }
};


class CEFaceMask {
public:
  // default constructor
  // addContact ( contact )
  // getSuperSpreaders ( from, to )
  // TODO
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
