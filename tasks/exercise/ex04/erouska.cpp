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
 * @brief Class for storing and comparing dates
 *
 * Values are not sanitized
 */
class CDate {
public:
    int year;
    int month;
    int day;

    CDate(unsigned int y, unsigned int m, unsigned int d): year(y), month(m), day(d) {}

    bool isEqual(const CDate & otherDate) const {
        return ((year == otherDate.year) && (month == otherDate.month) && (day == otherDate.day));
    }

    bool operator == (const CDate & rhs) const {
        return isEqual(rhs);
    }

    bool operator > (const CDate & rhs) const {
        
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

    CTime(unsigned int h, unsigned int m, unsigned int s): hour(h), minute(m), second(s) {}

    bool isEqual(const CTime & otherTime) const {
        return ((hour == otherTime.hour) && (minute == otherTime.minute) && (second == otherTime.second));
    }

    bool operator == (const CTime & rhs) const {
        return isEqual(rhs);
    }

    bool operator > (const CTime& rhs) const {

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

/**
 * @brief Class for storing and comparing date and time
 * 
 */
class CTimeStamp {
public:
    CTime time;
    CDate date;

    CTimeStamp(unsigned int y, unsigned int mo, unsigned int d, unsigned int h, unsigned int mi, unsigned int s) : time(h, mi, s), date(y, mo, d) {}

    bool isEqual(const CTimeStamp & otherTimestamp) const {
        return ((time == otherTimestamp.time) && (date == otherTimestamp.date));
    }

    bool operator == (const CTimeStamp & rhs) const {
        return isEqual(rhs);
    }

    bool operator >= (const CTimeStamp & rhs) const {

        if (date == rhs.date) {
            if (time == rhs.time) {
                return true;
            }
            return time > rhs.time;
        }
        return date > rhs.date;
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
    CContact(const CTimeStamp& ts, int p1, int p2) : timestamp(ts), person1(p1), person2(p2) {}

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
    bool containsPerson(unsigned int p, const CTimeStamp& fromT, const CTimeStamp& toT) const {
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
    vector<CContact> contacts; // Storage of all contacts

    /**
     * @brief Method to add the second person from contact to list
     * 
     * If contact doesn't contain searched person, or contains only searched person, nothing is added to the list.
     * 
     * @param contact Contact to look in
     * @param person Person in this contact
     * @param list List to add this other person to
     */
    void addPersonToList(const CContact & contact, long unsigned person, vector<int> & list) const {
        try {
            unsigned long otherPerson = contact.otherPerson(person);
            if (find(list.begin(), list.end(), otherPerson) == list.end()) {
                list.push_back(otherPerson);
            }
        }
        catch (...) {}
    }

    /**
     * @brief Method that searches through all contacts of given person and outputs all other persons that this person has been in contact with
     * 
     * Duplicates and self contacts are not added to result.
     * 
     * @param person Person to look for in contacts
     * @param fromT Timestamp to look from
     * @param toT Timestamp to look to
     * @param withTimestamps Should the search be restricted with timestamps
     * @return vector<int> List of contacts of given person
     */
    vector<int> processContacts(long unsigned person, CTimeStamp fromT = CTimeStamp(0, 0, 0, 0, 0, 0), CTimeStamp toT = CTimeStamp(0, 0, 0, 0, 0, 0), bool withTimestamps = false) const {
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
    /**
     * @brief Method that stores new contact
     * 
     * @param contact Contact to store
     * @return CEFaceMask& this object
     */
    CEFaceMask & addContact(const CContact & contact) {
        contacts.push_back(contact);
        return *this;
    }

    /**
     * @brief Method that searches through all contacts of given person and outputs all other persons that this person has been in contact with
     * 
     * Duplicates and self contacts are not added to result.
     * 
     * @param person Person to look for in contacts
     * @return vector<int> IDs of persons specified person has been in contact with
     */
    vector<int> listContacts(int person) const {
        return processContacts(person);
    }

    /**
     * @brief Method that searches through all contacts of given person and outputs all other persons that this person has been in contact with
     * 
     * Restricts search beween given timestamps.
     * 
     * Duplicates and self contacts are not added to result.
     * 
     * @param person Person to look for in contacts
     * @param fromT Timestamp to look from
     * @param toT Tinmestamp to look to
     * @return vector<int> IDs of persons specified person has been in contact with
     */
    vector<int> listContacts(int person, CTimeStamp fromT, CTimeStamp toT) const {
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
