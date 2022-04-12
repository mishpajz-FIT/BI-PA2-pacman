#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#include <algorithm>
#include <functional>

#include <stdexcept>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cstdint>
#include <cctype>
#include <cassert>
using namespace std;
#endif /* __PROGTEST__ */


template <typename T>
class CSet {
private:

    struct CNode {
        CNode * next;

        T * value;

        CNode(const T & val) : next(nullptr) {
            value = new T(val);
        }

        CNode(const CNode & copyFrom) : next(nullptr), value(nullptr) {
            if (copyFrom.value != nullptr) {
                value = new T(*copyFrom.value);
            }
        }

        ~CNode() {
            if (value != nullptr) {
                delete value;
                value = nullptr;
            }
        }

        const T & Value() const {
            return *value;
        }
    };

    CNode * begin;

    size_t size;

public:
    CSet() : begin(nullptr), size(0) { }

    CSet(const CSet & copyFrom) : size(copyFrom.size) {
        if (copyFrom.begin != nullptr) {
            begin = new CNode(*(copyFrom.begin));
            CNode * iter = begin;
            CNode * iterCopy = copyFrom.begin->next;
            while (iterCopy != nullptr) {
                iter->next = new CNode(*(iterCopy));
                iter = iter->next;
                iterCopy = iterCopy->next;
            }
        } else {
            begin = nullptr;
        }
    }

    CSet & operator = (const CSet & toCopy) {
        if (this == &toCopy) {
            return *this;
        }

        CSet copy(toCopy);

        size = copy.size;

        CNode * tmpBegin = copy.begin;
        copy.begin = begin;
        begin = tmpBegin;

        return *this;
    }

    ~CSet() {
        CNode * iter = begin;
        while (iter != nullptr) {
            CNode * tmp = iter->next;
            delete iter;
            iter = tmp;
        }
        begin = nullptr;
        size = 0;
    }

    bool Insert(const T & value) {
        CNode * iter = begin;
        CNode * prevIter = nullptr;
        while (iter != nullptr) { // Search for correct place to insert new value to (set is sorted)
            if (!(iter->Value() < value)) {
                if (!(value < iter->Value())) { // If value is already contained return false
                    return false;
                }
                break;
            }
            prevIter = iter;
            iter = iter->next;
        }

        CNode * newNode = new CNode(value); //Insert new node into linked list
        if (prevIter == nullptr) {
            begin = newNode;
            newNode->next = iter;
        } else {
            prevIter->next = newNode;
            newNode->next = iter;
        }

        size++;
        return true;
    }

    bool Remove(const T & value) {
        if (begin == nullptr) {
            return false;
        }

        CNode * iter = begin;
        CNode * prevIter = nullptr;
        while (iter != nullptr) { // Search for node with value to remove in set (set is sorted)
            if (!(iter->Value() < value)) {
                if (!(value < iter->Value())) { //Node is found, remove it from linked list
                    if (prevIter == nullptr) {
                        begin = iter->next;
                    } else {
                        prevIter->next = iter->next;
                    }
                    delete iter;

                    size--;
                    return true;
                }
                break;
            }

            prevIter = iter;
            iter = iter->next;
        }

        return false;
    }

    bool Contains(const T & value) const {
        CNode * iter = begin;
        while (iter != nullptr) { // Search for node with value set (set is sorted)
            if (!(iter->Value() < value)) {
                if (!(value < iter->Value())) { //Node was found
                    return true;
                }
                break; //Got to max possible location where value could be but was not found
            }
            iter = iter->next;
        }
        return false;
    }

    size_t Size() const {
        return size;
    }
};

#ifndef __PROGTEST__
#include <cassert>

struct CSetTester {
    static void test0() {
        CSet<string> x0;
        assert(x0.Insert("Jerabek Michal"));
        assert(x0.Insert("Pavlik Ales"));
        assert(x0.Insert("Dusek Zikmund"));
        assert(x0.Size() == 3);
        assert(x0.Contains("Dusek Zikmund"));
        assert(!x0.Contains("Pavlik Jan"));
        assert(x0.Remove("Jerabek Michal"));
        assert(!x0.Remove("Pavlik Jan"));
    }

    static void test1() {
        CSet<string> x0;
        assert(x0.Insert("Jerabek Michal"));
        assert(x0.Insert("Pavlik Ales"));
        assert(x0.Insert("Dusek Zikmund"));
        assert(x0.Size() == 3);
        assert(x0.Contains("Dusek Zikmund"));
        assert(!x0.Contains("Pavlik Jan"));
        assert(x0.Remove("Jerabek Michal"));
        assert(!x0.Remove("Pavlik Jan"));
        CSet<string> x1(x0);
        assert(x0.Insert("Vodickova Saskie"));
        assert(x1.Insert("Kadlecova Kvetslava"));
        assert(x0.Size() == 3);
        assert(x1.Size() == 3);
        assert(x0.Contains("Vodickova Saskie"));
        assert(!x1.Contains("Vodickova Saskie"));
        assert(!x0.Contains("Kadlecova Kvetslava"));
        assert(x1.Contains("Kadlecova Kvetslava"));
    }

    static void test2() {
        CSet<string> x0;
        CSet<string> x1;
        assert(x0.Insert("Jerabek Michal"));
        assert(x0.Insert("Pavlik Ales"));
        assert(x0.Insert("Dusek Zikmund"));
        assert(x0.Size() == 3);
        assert(x0.Contains("Dusek Zikmund"));
        assert(!x0.Contains("Pavlik Jan"));
        assert(x0.Remove("Jerabek Michal"));
        assert(!x0.Remove("Pavlik Jan"));
        x1 = x0;
        assert(x0.Insert("Vodickova Saskie"));
        assert(x1.Insert("Kadlecova Kvetslava"));
        assert(x0.Size() == 3);
        assert(x1.Size() == 3);
        assert(x0.Contains("Vodickova Saskie"));
        assert(!x1.Contains("Vodickova Saskie"));
        assert(!x0.Contains("Kadlecova Kvetslava"));
        assert(x1.Contains("Kadlecova Kvetslava"));
    }

    static void test4() {
        CSet<int> x0;
        assert(x0.Insert(4));
        assert(x0.Insert(8));
        assert(x0.Insert(1));
        assert(x0.Size() == 3);
        assert(x0.Contains(4));
        assert(!x0.Contains(5));
        assert(!x0.Remove(5));
        assert(x0.Remove(4));
    }

};

int main() {
    CSetTester::test0();
    CSetTester::test1();
    CSetTester::test2();
    CSetTester::test4();
    return 0;
}
#endif /* __PROGTEST__ */
