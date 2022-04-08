/**
 * @file supermarket.cpp
 * @author Michal Dobeš
 * @date 2022-04-03
 *
 * @brief Storage management in supermarket
 *
 * Supports adding batches of items with expiration date, removal of items and counting the number of items that have an older expiration date.
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <queue>
#include <stack>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

// SECTION: Implementation

/**
 * @brief Date
 *
 * Stores year, month, day.
 * Contains comparison operators.
 *
 */
class CDate {
private:
    unsigned int year;
    unsigned int month;
    unsigned int day;

    /**
     * @brief Finds out if year is leap
     *
     * @param y Year
     * @return true
     * @return false
     */
    static bool leapYear(unsigned int y) {
        return ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0 && y % 4000 != 0));
    }

    /**
     * @brief Calculate days in month (accounting for differences in leap years)
     *
     * @param y Year
     * @param m Month
     * @return unsigned int Days in month
     */
    static unsigned int daysInMonth(unsigned int y, unsigned int m) {
        unsigned int inMonth[12] = { 31, leapYear(y) ? (unsigned)(29) : (unsigned)(28), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        return inMonth[m - 1];
    }

public:
    /**
     * @brief Construct a new CDate object
     *
     * Throws if date is invalid according to Gregorian calendar
     *
     * @param y Year
     * @param m Month
     * @param d Day
     */
    CDate(unsigned int y, unsigned int m, unsigned int d) {
        if (m > 12 || m == 0 ||
            d > daysInMonth(y, m) || d == 0) {
            throw "Invalid CDate";
        }

        year = y;
        month = m;
        day = d;
    }

    // SECTION: Comparison operators

    friend bool operator < (const CDate & lhs, const CDate & rhs) {
        return tie(lhs.year, lhs.month, lhs.day) < tie(rhs.year, rhs.month, rhs.day);
    }

    friend bool operator > (const CDate & lhs, const CDate & rhs) { return rhs < lhs; }

    friend bool operator <= (const CDate & lhs, const CDate & rhs) { return !(lhs > rhs); }

    friend bool operator >= (const CDate & lhs, const CDate & rhs) { return !(rhs < lhs); }

    friend bool operator == (const CDate & lhs, const CDate & rhs) {
        return tie(lhs.year, lhs.month, lhs.day) == tie(rhs.year, rhs.month, rhs.day);
    }

    friend bool operator != (const CDate & lhs, const CDate & rhs) { return !(rhs == lhs); }

    // !SECTION
};

/**
 * @brief Supermarket storage management
 *
 */
class CSupermarket {
    /*
        Items are separated into batches, with each batch having different expiration date and different amount.

        Names of items are stored in CSupermarket::keys. -> If the name is entered inaccurately, it is searched for here
        Batches are stored in CSupermarket::items and in CSupermarket::expiring. -> This dual information storage allows for both fast adding/removing batches and fast lookup of expired batches.
            In CSupermarket::items, they are stored by their name, in priority queues where batch with the oldest expiration date has biggest priority.
            In CSupermarket::expiring they are stored by their expiration date.
    */

private:
    // SECTION: Data storage helper classes

    /**
     * @brief Batch storage for priority queue in CSupermarket::items map
     *
     */
    struct CItemBatch {
        CDate date; //< Expiration date
        int amount; //< Amount

        unsigned int expiringId; //< Helper id for lookup in CSupermarket::expiring map, should be unique in combination with Date

        CItemBatch(CDate d, int a, unsigned int s) : date(d), amount(a), expiringId(s) { }

        /**
         * @brief Comparison operator for priority queue ordering
         *
         * Batch with older date is larger
         *
         * @param rhs Item
         * @return true Date is younger (or same) than rhs
         * @return false Date is older than rhs
         */
        bool operator < (const CItemBatch & rhs) const {
            return date > rhs.date;
        }
    };

    /**
     * @brief Item data storage helper for processing selling
     *
     */
    struct CShoplistItem {
        pair<string, int> nameAndAmount; //< Sell request (Item name (can contain mismatch) and amount)
        bool exists; //< Does unique item with name or (similar enough name) exist in CSupermarket::items
        string key; //< Correct name of item (without mismatches)

        CShoplistItem(pair<string, int> & na, bool e = false, string k = "") : nameAndAmount(na), exists(e), key(move(k)) { }
    };

    /**
     * @brief Key storage for CSupermarket::keys set
     *
     * Includes semi-hash functions for faster comparison
     */
    struct CValidKey {
    private:
        string key; //< Key itself 
        unsigned int asciiValue; //< Precalculated ascii values

        /**
         * @brief Calculate sum of ascii values of key
         *
         * @return unsigned int Ascii value
         */
        unsigned int calculateAsciiValue() {
            unsigned int result = 0;
            for (auto & c : key) {
                result += c;
            }
            return result;
        }


    public:

        /**
         * @brief Construct a new Valid Key object
         *
         * @param k String that serves as key
         * @param hashModif Modify calculated hash by this value
         */
        CValidKey(string k, int hashModif = 0) : key(move(k)) {
            unsigned int calculatedHash = calculateAsciiValue();
            if (hashModif < 0 && static_cast<unsigned int>(hashModif < 0 ? -hashModif : hashModif) > calculatedHash) {
                calculatedHash = 0;
            } else {
                calculatedHash += hashModif;
            }
            asciiValue = calculatedHash;
        }

        /**
         * @brief Comparison for set ordering
         *
         * Values are firstly compared by key length, then ascii value and finally by whole keys
         *
         * @param lhs CValidKey
         * @param rhs CValidKey
         * @return true lhs is smaller
         * @return false lhs is equal or greater
         */
        friend bool operator < (const CValidKey & lhs, const CValidKey & rhs) {
            size_t lhsLength = lhs.key.length();
            size_t rhsLength = rhs.key.length();

            return tie(lhsLength, lhs.asciiValue, lhs.key) < tie(rhsLength, rhs.asciiValue, rhs.key);
        }

        const string & getKey() const {
            return key;
        }

        unsigned int getAsciiValue() const {
            return asciiValue;
        }

        size_t getKeyLength() const {
            return key.length();
        }

        /**
         * @brief Get char in key
         *
         * @param at Index of char
         * @return const char& Char
         */
        const char & operator[] (size_t at) const {
            return key[at];
        }
    };

    /**
     * @brief Key in map CSupermarket::expiring map
     *
     * Keys are sorted by expiration date, then by serial
     */
    struct CExpiredKey {
    private:
        static unsigned int newExpiringSerial; //< Unique value, should be retrieved by CSupermarket::getNewExpiringSerial
        /**
        * @brief Get the CExpiredKey::newExpiringSerial value and update it for next use
        *
        * Raises expiring serial value so it is kept unique
        *
        * @return unsigned int
        */
        unsigned int getNewExpiringSerial() {
            return newExpiringSerial++;
        }

    public:
        CDate date;
        unsigned int expiredSerialID; //< Helper id for lookup, should be unique in combination with Date

        /**
         * @brief Construct a new Expired Key object
         *
         * @param d Date
         * @param s Serial, should be unique in combination with Date
         */
        CExpiredKey(CDate d, unsigned int s) : date(move(d)), expiredSerialID(s) { }

        /**
         * @brief Construct a new Expired Key object
         *
         * Serial is generated to be unique
         *
         * @param d Date
         */
        CExpiredKey(CDate d) : date(move(d)), expiredSerialID(getNewExpiringSerial()) { }

        /**
         * @brief Comparison for keys, compared first by date, then by serial
         *
         * @param rhs
         * @return true
         * @return false
         */
        bool operator < (const CExpiredKey & rhs) const {
            return tie(date, expiredSerialID) < tie(rhs.date, rhs.expiredSerialID);
        }
    };

    /**
     * @brief Batch storage for CSupermarket::expiring map
     *
     */
    struct CExpiredBatch {
        string name; //< Name of item
        int amount; //< Amount

        CExpiredBatch(string n, int a) : name(move(n)), amount(a) { }
    };

    //!SECTION
    //SECTION: Variables

    set<CValidKey> keys; //< Set containing keys for map CSupermarket::items
    unordered_map<string, priority_queue<CItemBatch, vector<CItemBatch>>> items; // Map with keys as item names and values as priority queues of batches, prioritised by oldest (by expiration date) batches
    map<CExpiredKey, CExpiredBatch> expiring; //< Map with keys as expire dates and values as batches with names of item and and their amout, (is sorted by date for fast expired batches lookup)

    //!SECTION
    //SECTION: Methods

    /**
     * @brief Check if CValidKeys have maximum of one mismatch
     *
     * Keys need to have same length.
     *
     * @param key CValidKey
     * @param compareTo CValidKey
     * @return true Zero or one mismatches
     * @return false More mismatches than one
     */
    static bool hasKeyMaxMismatch(const CValidKey & key, const CValidKey & compareTo) {
        if (key.getKeyLength() != compareTo.getKeyLength()) {
            return false;
        }

        int mismatches = 0;
        for (size_t i = 0; i < key.getKeyLength(); i++) {
            if (key[i] != compareTo[i]) {
                mismatches++;
                if (key.getAsciiValue() - key[i] != compareTo.getAsciiValue() - compareTo[i]) { //< If ascii values are different after removing mismatch, rest of the string surely cannot be same
                    return false;
                }
            }
            if (mismatches > 1) {
                return false;
            }
        }
        return true;
    }

    /**
     * @brief Find key in CSupermarket::keys
     *
     * Key can contain max one mismatch.
     * Key must match only one key, if more keys match, key is not found.
     *
     * @param [inout] key Key to find/found key
     * @param [inout] found Has unique key been found
     */
    void findInKeys(string & key, bool & found) {
        auto lowerIter = keys.lower_bound(CValidKey(key, -0x7F)); //< Lowest key to search is searched key with substracted ascii value of 0x7f, which is max possible difference between two mismatching chars in case of a single mismatch
        CValidKey upper(key, +0x7F); //< Highest key to search for is searched key with added ascii value of 0x7f (max possible difference between two chars in case of one mismatch)

        found = false;
        CValidKey keyToFind(key);
        while ((lowerIter != keys.end()) && !(upper < (*lowerIter))) {
            if (hasKeyMaxMismatch(keyToFind, (*lowerIter))) {
                if (!found) {
                    found = true;
                    key = (*lowerIter).getKey();
                } else { //< If more than one key is matching return false
                    found = false;
                    break;
                }
            }
            lowerIter++;
        }
    }

public:
    CSupermarket() { }

    /**
     * @brief Store batch of item
     *
     * @param name Name of item
     * @param expireDate Expire date of batch
     * @param count Amount in batch
     * @return CSupermarket& this
     */
    CSupermarket & store(string name, CDate expireDate, int count) {
        CExpiredKey newExpiredKey(expireDate); // Create new key for CSupermarket::expiring

        expiring.emplace(make_pair(newExpiredKey, CExpiredBatch(name, count))); //< Add batch into CSupermarket::expiring
        items[name].push(CItemBatch(move(expireDate), count, newExpiredKey.expiredSerialID)); //< Add batch into appropriate priority queue in CSupermarket::items
        keys.emplace(move(name)); //< Add key (if is new) into CSupermarket::keys
        return (*this);
    }

    /**
     * @brief Sell items
     *
     * Removes items from CSupermarket.
     * Items are removed by oldest (by expiring date) batches first.
     * Performs name search for names that do not exactly match (up to one mismatch).
     * Transactionally executed, all items are first found, then sold.
     *
     * @param [inout] shoppingList List of pair<string, int>, where string is name and int is count
     */
    void sell(list<pair<string, int>> & shoppingList) {

        list<CShoplistItem> processedList; //< Helper struct for processing items
        for (auto & i : shoppingList) {
            if (items.find(i.first) == items.end()) { //< If item isn't found, try to find correct item name and process it into processedList
                bool found = false;
                string correctKey = i.first;
                findInKeys(correctKey, found);

                processedList.emplace_back(i, found, correctKey);

            } else { //< If item is found, process into processedList
                processedList.emplace_back(i, true, i.first);
            }
        }

        shoppingList.clear();

        for (auto & i : processedList) {
            if (!(i.exists)) { //< If item wasn't found, put it back into shopping list
                shoppingList.emplace_back(i.nameAndAmount);
                continue;
            }

            while (i.nameAndAmount.second >= 0) { //< Remove batches of item from storage until shopping request is fulfilled
                if (items[i.key].size() == 0) { //< If no more batches of item are stored, erase key in CSupermarket::keys and priority queue in CSupermarket::items
                    if (i.nameAndAmount.second > 0) { //< If shopping list requested larger amount, return what wasn't fulfilled to list
                        shoppingList.emplace_back(i.nameAndAmount);
                    }
                    items.erase(i.key);
                    keys.erase(i.key);
                    break;
                }

                if (i.nameAndAmount.second >= static_cast<int>(items[i.key].top().amount)) { //< If shopping list requests bigger amount than in oldest (by expiring date) batch of items, pop the batch from priority queue in CSupermarket::items (and remove from CSupermarket::expired)
                    i.nameAndAmount.second -= items[i.key].top().amount; //< Update remaining amount to sell
                    expiring.erase(CExpiredKey(items[i.key].top().date, items[i.key].top().expiringId));
                    items[i.key].pop();
                } else { //< If shopping list requests smaller amount than in oldest (by expiring date) batch of items, update amount in batch
                    const_cast<CItemBatch &>(items[i.key].top()).amount -= i.nameAndAmount.second; //< Update amount in priority queue in CSupermarket::items
                    expiring.at(CExpiredKey(items[i.key].top().date, items[i.key].top().expiringId)).amount = items[i.key].top().amount; //< Update amount in CSupermarket::expired
                    break;
                }
            }
        }
    }

    /**
     * @brief Get list with items and their amount in batches that expire before the specified date
     *
     * Batches expiring on date itself are not included.
     * Items are sorted descending by amount.
     *
     * @param date CDate to look before
     * @return list<pair<string, int>>
     */
    list<pair<string, int>> expired(const CDate & date) const {

        auto upper = expiring.lower_bound(CExpiredKey(date, 0)); //< Get batches up to this batch from CSupermarket::expiring

        unordered_map<string, int> processedExpiring; //< Map containing name of item as key and expired amount as value 
        auto iter = expiring.begin();
        while (iter != upper) {
            processedExpiring[(*iter).second.name] += (*iter).second.amount; //< Add each expired batch into map
            iter++;
        }

        vector<pair<string, int>> preparedResult(processedExpiring.begin(), processedExpiring.end()); //< Convert map into vector and sort by amount.
        sort(preparedResult.begin(), preparedResult.end(), [ ](const pair<string, int> & lhs, const pair<string, int> & rhs) { return lhs.second > rhs.second; });

        list<pair<string, int>> result(preparedResult.begin(), preparedResult.end()); //< Convert vector into list
        return result;
    }

    // !SECTION
};

unsigned int CSupermarket::CExpiredKey::newExpiringSerial = 1;

// !SECTION
// SECTION: Tests

#ifndef __PROGTEST__
int main(void) {
    CSupermarket s;
    s.store("bread", CDate(2016, 4, 30), 100)
        .store("butter", CDate(2016, 5, 10), 10)
        .store("beer", CDate(2016, 8, 10), 50)
        .store("bread", CDate(2016, 4, 25), 100)
        .store("okey", CDate(2016, 7, 18), 5);

    list<pair<string, int> > l0 = s.expired(CDate(2018, 4, 30));
    assert(l0.size() == 4);
    assert((l0 == list<pair<string, int> > { { "bread", 200 }, { "beer", 50 }, { "butter", 10 }, { "okey", 5 } }));

    list<pair<string, int> > l1 { { "bread", 2 }, { "Coke", 5 }, { "butter", 20 } };
    s.sell(l1);
    assert(l1.size() == 2);
    assert((l1 == list<pair<string, int> > { { "Coke", 5 }, { "butter", 10 } }));

    list<pair<string, int> > l2 = s.expired(CDate(2016, 4, 30));
    assert(l2.size() == 1);
    assert((l2 == list<pair<string, int> > { { "bread", 98 } }));

    list<pair<string, int> > l3 = s.expired(CDate(2016, 5, 20));
    assert(l3.size() == 1);
    assert((l3 == list<pair<string, int> > { { "bread", 198 } }));

    list<pair<string, int> > l4 { { "bread", 105 } };
    s.sell(l4);
    assert(l4.size() == 0);
    assert((l4 == list<pair<string, int> > {  }));

    list<pair<string, int> > l5 = s.expired(CDate(2017, 1, 1));
    assert(l5.size() == 3);
    assert((l5 == list<pair<string, int> > { { "bread", 93 }, { "beer", 50 }, { "okey", 5 } }));

    s.store("Coke", CDate(2016, 12, 31), 10);

    list<pair<string, int> > l6 { { "Cake", 1 }, { "Coke", 1 }, { "cake", 1 }, { "coke", 1 }, { "cuke", 1 }, { "Cokes", 1 } };
    s.sell(l6);
    assert(l6.size() == 3);
    assert((l6 == list<pair<string, int> > { { "cake", 1 }, { "cuke", 1 }, { "Cokes", 1 } }));

    list<pair<string, int> > l7 = s.expired(CDate(2017, 1, 1));
    assert(l7.size() == 4);
    assert((l7 == list<pair<string, int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 7 }, { "okey", 5 } }));

    s.store("cake", CDate(2016, 11, 1), 5);

    list<pair<string, int> > l8 { { "Cake", 1 }, { "Coke", 1 }, { "cake", 1 }, { "coke", 1 }, { "cuke", 1 } };
    s.sell(l8);
    assert(l8.size() == 2);
    assert((l8 == list<pair<string, int> > { { "Cake", 1 }, { "coke", 1 } }));

    list<pair<string, int> > l9 = s.expired(CDate(2017, 1, 1));
    assert(l9.size() == 5);
    assert((l9 == list<pair<string, int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 6 }, { "okey", 5 }, { "cake", 3 } }));

    list<pair<string, int> > l10 { { "cake", 15 }, { "Cake", 2 } };
    s.sell(l10);
    assert(l10.size() == 2);
    assert((l10 == list<pair<string, int> > { { "cake", 12 }, { "Cake", 2 } }));

    list<pair<string, int> > l11 = s.expired(CDate(2017, 1, 1));
    assert(l11.size() == 4);
    assert((l11 == list<pair<string, int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 6 }, { "okey", 5 } }));

    list<pair<string, int> > l12 { { "Cake", 4 } };
    s.sell(l12);
    assert(l12.size() == 0);
    assert((l12 == list<pair<string, int> > {  }));

    list<pair<string, int> > l13 = s.expired(CDate(2017, 1, 1));
    assert(l13.size() == 4);
    assert((l13 == list<pair<string, int> > { { "bread", 93 }, { "beer", 50 }, { "okey", 5 }, { "Coke", 2 } }));

    list<pair<string, int> > l14 { { "Beer", 20 }, { "Coke", 1 }, { "bear", 25 }, { "beer", 10 } };
    s.sell(l14);
    assert(l14.size() == 1);
    assert((l14 == list<pair<string, int> > { { "beer", 5 } }));

    s.store("ccccb", CDate(2019, 3, 11), 100)
        .store("ccccd", CDate(2019, 6, 9), 100)
        .store("dcccc", CDate(2019, 2, 14), 100);

    list<pair<string, int> > l15 { { "ccccc", 10 } };
    s.sell(l15);
    assert(l15.size() == 1);
    assert((l15 == list<pair<string, int> > { { "ccccc", 10 } }));



    CSupermarket s01;
    s01.store("aaaaaa", CDate(2001, 10, 14), 10);
    s01.store("aaaaaa", CDate(2020, 4, 3), 10);

    list<pair<string, int>> l001 { { "aaaaaa", 10 } };
    s01.sell(l001);
    assert(l001.size() == 0);
    l001 = s01.expired(CDate(2022, 1, 1));
    assert(l001.size() == 1);
    assert((l001 == list<pair<string, int> > { { "aaaaaa", 10 } }));
    l001 = s01.expired(CDate(2020, 1, 1));
    assert(l001.size() == 0);

    l001 = s01.expired(CDate(2020, 4, 3));
    assert(l001.size() == 0);
    l001 = s01.expired(CDate(2020, 4, 4));
    assert(l001.size() == 1);


    CSupermarket s02;
    list<pair<string, int>> l002 { { "aaaaaa", 10 }, { "hhhhhhh", 10 }, { "oooooo", 10 }, { "jjjjjjj", 10 } };
    s02.sell(l002);
    assert((l002 == list<pair<string, int> > { { "aaaaaa", 10 }, { "hhhhhhh", 10 }, { "oooooo", 10 }, { "jjjjjjj", 10 } }));

    s02.store("aaaa", CDate(2200, 10, 10), 35);
    l002 = { { "aaab", 10 }, { "aaac", 10 }, { "aaad", 10 }, { "aaaf", 10 }, { "aaaa", 10 } };
    s02.sell(l002);
    assert((l002 == list<pair<string, int> > { { "aaaf", 5 }, { "aaaa", 10 } }));

    l002 = s02.expired(CDate(4000, 10, 10));
    assert(l002.size() == 0);
    l002 = s02.expired(CDate(1890, 10, 10));
    assert(l002.size() == 0);

    s02.store("aaaa", CDate(2200, 10, 10), 35);
    l002 = { { "aaaa", 35 }, { "aaac", 100 } };
    s02.sell(l002);
    assert((l002 == list<pair<string, int> > { { "aaac", 100 } }));

    s02.store("aaaa", CDate(2200, 10, 10), 20);
    l002 = { { "aaac", 100 }, { "aaac", 35 } };
    s02.sell(l002);
    assert((l002 == list<pair<string, int> > { { "aaac", 80 }, { "aaac", 35 } }));

    s02.store("a", CDate(2200, 10, 10), 20)
        .store("b", CDate(2200, 10, 10), 20)
        .store("c", CDate(2200, 10, 10), 20)
        .store("d", CDate(2200, 10, 10), 20);

    l002 = { { "a", 20 }, { "e", 20 }, { "b", 25 } };
    s02.sell(l002);
    assert((l002 == list<pair<string, int> > { { "e", 20 }, { "b", 5 } }));

    l002 = { { "a", 20 }, { "c", 20 }, { "e", 25 } };
    s02.sell(l002);
    assert((l002 == list<pair<string, int> > { { "a", 20 }, { "e", 25 } }));

    l002 = { { "a", 20 } };
    s02.sell(l002);
    assert((l002 == list<pair<string, int> > { }));

    s02.store("a", CDate(2200, 10, 10), 20);
    l002 = { { "a", 0 } };
    s02.sell(l002);
    assert((l002 == list<pair<string, int> > { }));

    l002 = s02.expired(CDate(4000, 10, 10));
    assert((l002 == list<pair<string, int> > { { "a", 20 } }));

    l002 = { { "a", 20 }, { "e", 20 } };
    s02.sell(l002);
    assert((l002 == list<pair<string, int> > { { "e", 20 } }));

    l002 = s02.expired(CDate(4000, 10, 10));
    assert(l002.size() == 0);
    assert((l002 == list<pair<string, int> > { }));

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */

// !SECTION