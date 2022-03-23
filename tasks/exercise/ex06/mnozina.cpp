#ifndef __PROGTEST__
#include <cstring>
#include <iostream>
using namespace std;

class CLinkedSetTester;
#endif /* __PROGTEST__ */


class CLinkedSet {
private:
    struct CNode {
    private:
        char * value;

    public:
        CNode * m_Next;

        CNode(const char * v) {
            m_Next = nullptr;

            value = new char[strlen(v) + 1];
            strcpy(value, v);
        }

        CNode(const CNode & copy) {
            value = new char[strlen(copy.value) + 1];
            strcpy(value, copy.value);

            if (copy.m_Next != nullptr) {
                m_Next = new CNode(*(copy.m_Next));
            } else {
                m_Next = nullptr;
            }
        }

        ~CNode() {
            delete [] value;
        }

        const char * Value() const {
            return value;
        }
    };

    CNode * m_Begin;
    size_t size;

public:
    CLinkedSet() : size(0) {
        m_Begin = nullptr;
    }

    CLinkedSet(const CLinkedSet & toCopy) {
        m_Begin = new CNode(*(toCopy.m_Begin));

        size = toCopy.size;
    }

    CLinkedSet & operator = (CLinkedSet toCopy) {
        size = toCopy.size;

        CNode * tmpBegin = m_Begin;
        m_Begin = toCopy.m_Begin;
        toCopy.m_Begin = tmpBegin;

        return *this;
    }

    ~CLinkedSet() {
        CNode * iter = m_Begin;
        while (iter != nullptr) {
            CNode * tmp = iter;
            iter = iter->m_Next;
            delete tmp;
        }
    }

    bool Insert(const char * value) {
        CNode * iter = m_Begin;
        CNode * iterPrev = nullptr;
        while (iter != nullptr) {
            int comp = strcmp(value, iter->Value());
            if (comp > -1) {
                if (comp == 0) {
                    return false;
                }
                break;
            }
            iterPrev = iter;
            iter = iter->m_Next;
        }

        CNode * newNode = new CNode(value);
        if (iterPrev == nullptr) {
            m_Begin = newNode;
            newNode->m_Next = iter;
        } else {
            iterPrev->m_Next = newNode;
            newNode->m_Next = iter;
        }
        size++;
        return true;
    }


    bool Remove(const char * value) {

        CNode * iter = m_Begin;
        CNode * iterPrev = nullptr;
        while (iter != nullptr) {
            int comp = strcmp(value, iter->Value());
            if (comp > -1) {
                if (comp == 0) {
                    break;
                } else {
                    return false;
                }
            }
            iterPrev = iter;
            iter = iter->m_Next;
        }

        if (iterPrev == nullptr) {
            m_Begin = iter->m_Next;
        } else {
            iterPrev->m_Next = iter->m_Next;
        }
        delete iter;
        size--;
        return true;
    }

    bool Empty() const {
        return size == 0;
    }

    size_t Size() const {
        return size;
    }

    bool Contains(const char * value) const {
        CNode * iter = m_Begin;

        while (iter != nullptr) {
            int comp = strcmp(value, iter->Value());
            if (comp > -1) {
                if (comp == 0) {
                    return true;
                }
                break;
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

};

int main() {
    CLinkedSetTester::test0();
    CLinkedSetTester::test1();
    CLinkedSetTester::test2();
    return 0;
}
#endif /* __PROGTEST__ */
