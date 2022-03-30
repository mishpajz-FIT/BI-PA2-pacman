/**
 * @file mnozina.cpp
 * @author Michal Dobeš
 * @date 2022-03-25
 *
 * @brief Implementation of Set (of C strings) using linked list
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef __PROGTEST__
#include <cstring>
using namespace std;

class CLinkedSetTester;
#endif /* __PROGTEST__ */

/**
 * @brief Set of C strings
 *
 * Set is kept sorted
 *
 */
class CLinkedSet {
private:
    /**
     * @brief Node of linked list
     *
     */
    struct CNode {
        CNode * m_Next;

        char * value;

        /**
         * @brief Construct a new CNode object
         *
         * @param v Pointer to char-array (C string) to copy
         */
        CNode(const char * v) : m_Next(nullptr) {
            value = new char[strlen(v) + 1];
            strcpy(value, v);
        }

        /**
         * @brief Construct a new CNode object
         *
         * C String is copied, pointer to next CNode is set to null.
         *
         * @param copyFrom CNode to copy
         */
        CNode(const CNode & copyFrom) : m_Next(nullptr) {
            if (copyFrom.value != nullptr) {
                value = new char[strlen(copyFrom.value) + 1];
                strcpy(value, copyFrom.value);
            } else {
                value = nullptr;
            }
        }

        ~CNode() {
            if (value != nullptr) {
                delete [] value;
                value = nullptr;
            }
        }

        const char * Value() const {
            return value;
        }
    };

    CNode * m_Begin;

    size_t size;

public:

    CLinkedSet() : m_Begin(nullptr), size(0) { }

    CLinkedSet(const CLinkedSet & copyFrom) : size(copyFrom.size) {
        if (copyFrom.m_Begin != nullptr) {
            m_Begin = new CNode(*(copyFrom.m_Begin));
            CNode * iter = m_Begin;
            CNode * iterCopy = copyFrom.m_Begin->m_Next;
            while (iterCopy != nullptr) {
                iter->m_Next = new CNode(*(iterCopy));
                iter = iter->m_Next;
                iterCopy = iterCopy->m_Next;
            }
        } else {
            m_Begin = nullptr;
        }
    }

    CLinkedSet & operator = (const CLinkedSet & toCopy) {
        if (this == &toCopy) {
            return *this;
        }

        // Assign using copying and then swapping all variables with the copy 
        CLinkedSet copy(toCopy);

        size = copy.size;

        CNode * tmpBegin = copy.m_Begin;
        copy.m_Begin = m_Begin;
        m_Begin = tmpBegin;

        return *this;
    }

    ~CLinkedSet() {
        CNode * iter = m_Begin;
        while (iter != nullptr) {
            CNode * tmp = iter->m_Next;
            delete iter;
            iter = tmp;
        }
        m_Begin = nullptr;
        size = 0;
    }

    bool Insert(const char * value) {
        CNode * iter = m_Begin;
        CNode * prevIter = nullptr;
        while (iter != nullptr) { // Search for correct place to insert new value to (set is sorted)
            int cmp = strcmp(iter->Value(), value);
            if (cmp >= 0) {
                if (cmp == 0) { // If value is already contained return false
                    return false;
                }
                break;
            }
            prevIter = iter;
            iter = iter->m_Next;
        }

        CNode * newNode = new CNode(value); //Insert new node into linked list
        if (prevIter == nullptr) {
            m_Begin = newNode;
            newNode->m_Next = iter;
        } else {
            prevIter->m_Next = newNode;
            newNode->m_Next = iter;
        }

        size++;
        return true;
    }

    bool Remove(const char * value) {
        if (m_Begin == nullptr) {
            return false;
        }

        CNode * iter = m_Begin;
        CNode * prevIter = nullptr;
        while (iter != nullptr) { // Search for node with value to remove in set (set is sorted)
            int cmp = strcmp(iter->Value(), value);
            if (cmp >= 0) {
                if (cmp == 0) { //Node is found, remove it from linked list
                    if (prevIter == nullptr) {
                        m_Begin = iter->m_Next;
                    } else {
                        prevIter->m_Next = iter->m_Next;
                    }
                    delete iter;

                    size--;
                    return true;
                }
                break;
            }

            prevIter = iter;
            iter = iter->m_Next;
        }

        return false;
    }

    bool Empty() const {
        return size == 0;
    }

    size_t Size() const {
        return size;
    }

    bool Contains(const char * value) const {
        CNode * iter = m_Begin;
        while (iter != nullptr) { // Search for node with value set (set is sorted)
            int cmp = strcmp(iter->Value(), value);
            if (cmp >= 0) {
                if (cmp == 0) { //Node was found
                    return true;
                }
                break; //Gotto max possible location where value could be but was not found
            }
            iter = iter->m_Next;
        }
        return false;
    }

    friend class ::CLinkedSetTester;
};

#ifndef __PROGTEST__
#include <cassert>

struct CLinkedSetTester {
    static void test0() {
        CLinkedSet x0;
        assert(x0.Insert("Jerabek Michal"));
        assert(x0.Insert("Pavlik Ales"));
        assert(x0.Insert("Dusek Zikmund"));
        assert(x0.Size() == 3);
        assert(x0.Contains("Dusek Zikmund"));
        assert(!x0.Contains("Pavlik Jan"));
        assert(x0.Remove("Jerabek Michal"));
        assert(!x0.Remove("Pavlik Jan"));
        assert(!x0.Empty());
    }

    static void test1() {
        CLinkedSet x0;
        assert(!x0.Remove("Karel"));
        assert(x0.Insert("Jerabek Michal"));
        assert(x0.Insert("Pavlik Ales"));
        assert(x0.Insert("Dusek Zikmund"));
        assert(x0.Size() == 3);
        assert(x0.Contains("Dusek Zikmund"));
        assert(!x0.Contains("Pavlik Jan"));
        assert(x0.Remove("Jerabek Michal"));
        assert(!x0.Remove("Pavlik Jan"));
        CLinkedSet x1(x0);
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
        CLinkedSet x0;
        CLinkedSet x1;
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

    static void test3() {
        CLinkedSet x0;
        CLinkedSet x1;
        assert(x0.Insert("Jerabek Michal"));
        assert(x0.Insert("Pavlik Ales"));
        assert(x0.Insert("Dusek Zikmund"));
        assert(x0.Remove("Dusek Zikmund"));
        assert(!x0.Remove("Pavlik Jan"));
        assert(x0.Remove("Jerabek Michal"));
        assert(!x0.Remove("Pavlik Jan"));
        x1 = x0;
        assert(x0.Insert("Vodickova Saskie"));
        assert(x1.Insert("Kadlecova Kvetslava"));
        assert(x0.Remove("Vodickova Saskie"));
        assert(!x1.Remove("Vodickova Saskie"));
        assert(!x0.Remove("Kadlecova Kvetslava"));
        assert(x1.Remove("Kadlecova Kvetslava"));
    }

    static void test4() {
        CLinkedSet x0;
        assert(x0.Insert("adasdsadsadas"));
        assert(x0.Insert("jfkhaJKDAlkj"));
        assert(x0.Insert("sfgddfg"));
        assert(x0.Insert("jfajdjklassjdlk"));
        assert(x0.Insert("ndhdf"));
        assert(x0.Insert("sbfgsfg"));
        assert(x0.Insert("sfdgsfgf"));
        assert(x0.Insert("xbcvbxcvb"));
        assert(x0.Insert("sfdgsfdg"));
        assert(x0.Insert("sfdggsgdf"));
        assert(x0.Insert("xvcbbxcvbx"));
        assert(x0.Insert("xvcbbcxvb"));
        assert(x0.Insert("sfdgsgs"));
        assert(x0.Insert("sgferqq"));
        assert(x0.Insert("fgafdfadddd"));
        CLinkedSet x1(x0);
        assert(x0.Size() == 15);
        assert(x1.Size() == 15);
        assert(x1.Remove("sfdgsfgf"));
        assert(x1.Remove("xvcbbxcvbx"));
        assert(x1.Remove("sfdgsgs"));
        assert(x1.Remove("jfkhaJKDAlkj"));
        assert(x1.Size() == 11);
        x0 = x1;
        assert(x0.Size() == 11);
        assert(x0.Remove("jfajdjklassjdlk"));
        assert(x0.Size() == 10);
        assert(x1.Size() == 11);
    }

};

int main() {
    CLinkedSetTester::test0();
    CLinkedSetTester::test1();
    CLinkedSetTester::test2();
    CLinkedSetTester::test3();
    CLinkedSetTester::test4();
    return 0;
}
#endif /* __PROGTEST__ */