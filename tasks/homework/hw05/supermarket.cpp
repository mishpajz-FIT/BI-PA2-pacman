/**
 * @file supermarket.cpp
 * @author Michal Dobeš
 * @date 2022-04-03
 *
 * @brief
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

/**
 * @brief Date
 *
 * Storage for year, month, days.
 * Has comparison operators.
 *
 */
class CDate {
private:
    unsigned int year;
    unsigned int month;
    unsigned int day;

    /**
     * @brief Is year leap
     *
     * @param y Year
     * @return true Is leap
     * @return false Isn't leap
     */
    static bool leapYear(unsigned int y) {
        return ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0 && y % 4000 != 0));
    }

    /**
     * @brief Days in month (calculates with differce for leap year)
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
     * Throws if parameters are invalid
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

};

class CSupermarket {
private:

    /**
     * @brief Item data storage in CSupermarket::items map
     *
     */
    struct Item {
        CDate date;
        int amount;

        unsigned int expiringSerial; //< Helper id for lookup in CSupermarket::expiring map, should be unique in combination with Date

        Item(CDate d, int a, unsigned int s) : date(d), amount(a), expiringSerial(s) { }

        /**
         * @brief Comparison operator for priority queue ordering
         *
         * Item with older date is larger
         *
         * @param rhs another Item
         * @return true Date is younger (or same) than rhs
         * @return false Date is older than rhs
         */
        bool operator < (const Item & rhs) const {
            return date > rhs.date;
        }
    };

    /**
     * @brief Item data storage for processing selling
     *
     */
    struct ShoplistItem {
        pair<string, int> nameAndAmount; //< Item name (can contain mismatch) and count - Inputted variables
        bool exists; //< Does unique item with same name or similar name exist in  CSupermarket::items
        string key; //< Correct name of item (without mismatches)

        ShoplistItem(pair<string, int> & na, bool e = false, string k = "") : nameAndAmount(na), exists(e), key(move(k)) { }
    };

    /**
     * @brief Item data storage for CSupermarket::expiring map
     *
     */
    struct ExpiredItem {
        string name; //< Name of item
        int amount; //< Amount of item

        ExpiredItem() { } //< Empty constructor (needed for easier handling of map elements)

        ExpiredItem(string & n, int a) : name(n), amount(a) { }
    };

    /**
     * @brief Storage for key stored in CSupermarket::keys
     *
     * Includes semi-hash functions for faster comparison
     */
    struct ValidKey {
    private:
        string key; //< Key itself 
        unsigned int asciiValue; //< Precalculated ascii values

        /**
         * @brief Calculate sum of ascii values of key
         *
         * @return unsigned int
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
        ValidKey(const string & k, int hashModif = 0) : key(k) {
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
         * Values are firstly compared by key length, then hash and finally by whole keys
         *
         * @param lhs ValidKey
         * @param rhs ValidKey
         * @return true lhs is smaller
         * @return false lhs is equal or greater
         */
        friend bool operator < (const ValidKey & lhs, const ValidKey & rhs) {
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
    };

    set<ValidKey> keys; //< Set containing correct keys for map CSupermarket::items

    unordered_map<string, priority_queue<Item, vector<Item>>> items;

    /**
     * @brief Key in map CSupermarket::expiring
     *
     * Keys are sorted by date, then serial
     */
    struct ExpiredKey {
        CDate date;
        unsigned int expiredSerial; //< Helper id for lookup, should be unique in combination with Date

        /**
         * @brief Construct a new Expired Key object
         *
         * @param d Date
         * @param s Serial, should be unique in combination with Date
         */
        ExpiredKey(CDate d, unsigned int s) : date(move(d)), expiredSerial(s) { }

        /**
         * @brief Comparison for keys, compared first by date, then by serial
         *
         * @param rhs
         * @return true
         * @return false
         */
        bool operator < (const ExpiredKey & rhs) const {
            return tie(date, expiredSerial) < tie(rhs.date, rhs.expiredSerial);
        }
    };
    map<ExpiredKey, ExpiredItem> expiring; //< Map containing all items and their count, sorted by date 
    unsigned int expiringSerial; //< Unique value, should be retrieved by CSupermarket::getExpiringSerial
    /**
     * @brief Get the expiringSerial value and update it for next use
     *
     * Raises expiring serial value so it is kept unique
     *
     * @return unsigned int
     */
    unsigned int getExpiringSerial() {
        return expiringSerial++;
    }

    /**
     * @brief Check if strings have maximum of one mismatch
     *
     * Strings need to have same length.
     *
     * @param s String
     * @param compareTo String
     * @return true Zero or one mismatches
     * @return false More mismatches than one
     */
    static bool hasKeyMaxMismatch(const ValidKey & key, const ValidKey & compareTo) {
        if (key.getKey().length() != compareTo.getKey().length()) {
            return false;
        }

        int mismatches = 0;
        for (size_t i = 0; i < key.getKey().length(); i++) {
            if (key.getKey()[i] != compareTo.getKey()[i]) {
                mismatches++;
                if (key.getAsciiValue() - key.getKey()[i] != compareTo.getAsciiValue() - compareTo.getKey()[i]) {
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
     * Key must match only one string, else is not found.
     *
     * @param key Key to find
     * @param found Has been found
     */
    void findInKeys(string & key, bool & found) {
        auto lowerIter = keys.lower_bound(ValidKey(key, -0x7F));
        auto upperIter = keys.upper_bound(ValidKey(key, +0x7F));

        found = false;
        ValidKey keyToFind(key);
        while (lowerIter != upperIter) {
            if (hasKeyMaxMismatch(keyToFind, (*lowerIter))) {
                if (!found) {
                    found = true;
                    key = (*lowerIter).getKey();
                } else {
                    found = false;
                    break;
                }
            }
            lowerIter++;
        }
    }

public:
    CSupermarket() : expiringSerial(0) { }

    CSupermarket & store(string name, CDate expireDate, int count) {
        unsigned int generatedExpiringSerial = getExpiringSerial();

        expiring[ExpiredKey(expireDate, generatedExpiringSerial)] = ExpiredItem(name, count);
        items[name].push(Item(move(expireDate), count, generatedExpiringSerial));
        keys.emplace(move(name));
        return (*this);
    }

    void sell(list<pair<string, int>> & shoppingList) {

        list<ShoplistItem> processedList;
        for (auto & i : shoppingList) {
            if (items.find(i.first) == items.end()) {
                bool found = false;
                string correctKey = i.first;
                findInKeys(correctKey, found);

                processedList.emplace_back(i, found, correctKey);

            } else {
                processedList.emplace_back(i, true, i.first);
            }
        }

        shoppingList.clear();
        for (auto & i : processedList) {

            if (!(i.exists)) {
                shoppingList.emplace_back(i.nameAndAmount);
                continue;
            }

            while (i.nameAndAmount.second >= 0) {
                if (items[i.key].size() == 0) {
                    if (i.nameAndAmount.second > 0) {
                        shoppingList.emplace_back(i.nameAndAmount);
                    }
                    items.erase(i.key);
                    keys.erase(i.key);
                    break;
                }

                if (i.nameAndAmount.second >= static_cast<int>(items[i.key].top().amount)) {
                    i.nameAndAmount.second -= items[i.key].top().amount;
                    expiring.erase(ExpiredKey(items[i.key].top().date, items[i.key].top().expiringSerial));
                    items[i.key].pop();
                } else {
                    const_cast<Item &>(items[i.key].top()).amount -= i.nameAndAmount.second;
                    expiring[ExpiredKey(items[i.key].top().date, items[i.key].top().expiringSerial)].amount = items[i.key].top().amount;
                    break;
                }
            }
        }
    }

    list<pair<string, int>> expired(const CDate & date) const {

        auto upper = expiring.lower_bound(ExpiredKey(date, 0));

        unordered_map<string, int> processedExpiring;
        auto iter = expiring.begin();
        while (iter != upper) {
            processedExpiring[(*iter).second.name] += (*iter).second.amount;
            iter++;
        }

        vector<pair<string, int>> preparedResult;
        preparedResult.resize(processedExpiring.size());
        copy(processedExpiring.begin(), processedExpiring.end(), preparedResult.begin());
        sort(preparedResult.begin(), preparedResult.end(), [ ](const pair<string, int> & lhs, const pair<string, int> & rhs) { return lhs.second > rhs.second; });

        list<pair<string, int>> result;
        copy(preparedResult.begin(), preparedResult.end(), back_inserter(result));

        return result;
    }
};

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
