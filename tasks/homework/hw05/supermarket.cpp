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

class CDate {
private:
    unsigned int year;
    unsigned int month;
    unsigned int day;

    static bool leapYear(unsigned int y) {
        return ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0 && y % 4000 != 0));
    }

    static unsigned int daysInMonth(unsigned int y, unsigned int m) {
        unsigned int inMonth[12] = { 31, leapYear(y) ? (unsigned)(29) : (unsigned)(28), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        return inMonth[m - 1];
    }

public:
    CDate(unsigned int y, unsigned int m, unsigned int d) {
        if (m > 12 || m == 0 ||
            d > daysInMonth(y, m) || d == 0) {
            throw "Invalid CDate";
        }

        year = y;
        month = m;
        day = d;
    }

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
    struct Item {
        CDate date;
        int amount;

        unsigned int expiringSerial;

        Item(CDate d, int a, unsigned int s) : date(d), amount(a), expiringSerial(s) { }

        bool operator < (const Item & rhs) const {
            return date > rhs.date;
        }
    };

    struct ShoplistItem {
        pair<string, int> nameAndAmount;
        bool exists;
        string key;

        ShoplistItem(pair<string, int> & na, bool e = false, string k = "") : nameAndAmount(na), exists(e), key(move(k)) { }
    };

    struct ExpiredItem {
        string name;
        int amount;

        ExpiredItem() { }

        ExpiredItem(string & n, int a) : name(n), amount(a) { }
    };

    struct KeysCompare {
    private:
        static unsigned int calculateHash(const string & s) {
            unsigned int result = 0;
            for (auto c : s) {
                result += c;
            }
            return result;
        }
    public:
        bool operator() (const string & lhs, const string & rhs) const {
            if (lhs.length() == rhs.length()) {
                return lhs < rhs;
            }
            return lhs.length() < rhs.length();
        }
    };
    set<string, KeysCompare> keys;

    unordered_map<string, priority_queue<Item, vector<Item>>> items;

    struct ExpiredKey {
        CDate date;
        unsigned int expiredSerial;

        ExpiredKey(CDate d, unsigned int s) : date(move(d)), expiredSerial(s) { }

        bool operator < (const ExpiredKey & rhs) const {
            return tie(date, expiredSerial) < tie(rhs.date, rhs.expiredSerial);
        }
    };
    map<ExpiredKey, ExpiredItem> expiring;
    unsigned int expiringSerial;
    unsigned int getExpiringSerial() {
        return expiringSerial++;
    }

    static bool hasStringMaxMismatch(const string & s, const string & compareTo) {
        if (s.length() != compareTo.length()) {
            return false;
        }

        int mismatches = 0;
        for (size_t i = 0; i < s.length(); i++) {
            if (s[i] != compareTo[i]) {
                mismatches++;
            }
            if (mismatches > 1) {
                return false;
            }
        }
        return true;
    }

    void findInKeys(string & key, bool & found) {
        auto lowerIter = keys.lower_bound(string(key.length(), 0));
        auto upperIter = keys.upper_bound(string(key.length(), 127));

        found = false;
        string keyToFind = key;
        while (lowerIter != upperIter) {

            if (hasStringMaxMismatch(keyToFind, (*lowerIter))) {
                if (!found) {
                    found = true;
                    key = (*lowerIter);
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
