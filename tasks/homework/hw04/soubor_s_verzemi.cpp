#ifndef __PROGTEST__
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <iostream>
using namespace std;
#endif /* __PROGTEST__ */


template <typename T>
class List {

    struct Node {
        T value;
        Node * next;

        Node(T newValue) : value(newValue), next(nullptr) { }

        Node(const Node & toCopy) : value(toCopy.value) {
            next = nullptr;
        }
    };

    Node * head;
    Node * tail;

    size_t size;

public:
    struct ListIterator {
        Node * ptr;

        ListIterator(List & list, size_t position) : ptr(list.head) {
            for (size_t i = 0; i < position; i++) {
                ptr = ptr->next;
            }
        }

        ListIterator(Node * pointer) : ptr(pointer) { }

        T & operator * () {
            return (*ptr).value;
        }

        const T & operator * () const {
            return (*ptr).value;
        }

        ListIterator & operator ++() {
            ptr = ptr->next;
            return (*this);
        }

        ListIterator operator ++(int) {
            ListIterator old(*this);
            ++(*this);
            return old;
        }
    };

    List() : head(nullptr), tail(nullptr), size(0) { }

    List(const List & copyFrom) : size(copyFrom.size) {
        if (copyFrom.head != nullptr) {
            head = new Node(*(copyFrom.head));
            Node * iter = head;
            Node * iterCopy = copyFrom.head->next;
            while (iterCopy != nullptr) {
                iter->next = new Node(*(iterCopy));
                iter = iter->next;
                iterCopy = iterCopy->next;
            }
            tail = iter;
        } else {
            head = nullptr;
            tail = nullptr;
        }
    }

    ~List() {
        Node * iter = head;
        while (iter != nullptr) {
            Node * tmp = iter->next;
            delete iter;
            iter = tmp;
        }
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    List & operator = (List & toAssign) {
        if (this == &toAssign) {
            return *this;
        }

        List copy(toAssign);

        size = copy.size;

        Node * tmp = head;
        head = copy.head;
        copy.head = tmp;

        tmp = tail;
        tail = copy.tail;
        copy.tail = tmp;

        return (*this);
    }

    void insert(const ListIterator & after, const T & value) {
        Node * newNode = new Node(value);

        if (after.ptr == nullptr) {
            newNode->next = head;
            head = newNode;
        } else {
            newNode->next = (after.ptr)->next;
            (after.ptr)->next = newNode;
        }

        if (newNode->next == nullptr) {
            tail = newNode;
        }

        size++;
    }

    T remove(const ListIterator & after) {
        Node * tmp;

        if (after.ptr == nullptr) {
            tmp = head;
            head = head->next;
        } else {
            tmp = (after.ptr)->next;
            (after.ptr)->next = tmp->next;
        }

        if (tmp->next == nullptr) {
            tail = (after.ptr);
        }

        T value(tmp->value);
        delete tmp;
        size--;
        return value;
    }

    void pushFront(const T & value) {
        insert(ListIterator(nullptr), value);
    }

    T popFront() {
        return remove(ListIterator(nullptr));
    }

    void pushBack(const T & value) {
        insert(ListIterator(tail), value);
    }

    T & back() {
        return tail->value;
    }
    T & front() {
        return head->value;
    }

    size_t getSize() {
        return size;
    }
};

/*
class CFile {
private:
    struct Bucket;
    struct Data;
    class Version;

public:
    CFile(void);

    // TODO: copy cons, dtor, op=
    bool seek(uint32_t offset);

    uint32_t read(uint8_t * dst, uint32_t bytes);

    uint32_t write(const uint8_t * src, uint32_t bytes);

    void truncate(void);

    uint32_t fileSize(void) const;

    void addVersion(void);

    bool undoVersion(void);
};

struct CFile::Bucket {
    unsigned int refCount;

    uint32_t size;
    uint8_t * bytes;

    void createBytesFrom(uint8_t * fromBytes, uint32_t inSize) {
        bytes = new uint8_t[size];
        for (uint32_t i = 0; i < size; i++) {
            bytes[i] = fromBytes[i];
        }
    }

    Bucket(uint8_t * fromBytes, uint32_t inSize) : size(inSize), refCount(0) {
        createBytesFrom(fromBytes, inSize);
    }

    Bucket(const Bucket & toCopy) : refCount(0) {
        createBytesFrom(toCopy.bytes, toCopy.size);
    }

    ~Bucket() {
        delete [] bytes;
    }

    Bucket & operator = (const Bucket & toAssign) {
        if (this == &toAssign) {
            return (*this);
        }

        delete [] bytes;
        createBytesFrom(toAssign.bytes, toAssign.size);

        return (*this);
    }
};

struct CFile::Data {
    unsigned int refCount;

    Vector<CFile::Bucket *> * buckets;

    Data() : refCount(0) {
        buckets = new Vector<CFile::Bucket *>();
    }

    Data(const Data & toCopy) : refCount(0) {
        buckets = new Vector<CFile::Bucket *>(*(toCopy.buckets));
        for (size_t i = 0; i < (*buckets).getSize(); i++) {
            ((*buckets)[i]->refCount)++;
        }
    }

    ~Data() {
        delete [] buckets;
    }

    void releaseBucket(CFile::Bucket * bucket) {
        if (--(bucket->refCount) == 0) {
            delete bucket;
        }
    }

    Data & operator = (const Data & toAssign) {
        if (this == &toAssign) {
            return (*this);
        }

        for (size_t i = 0; i < (*buckets).getSize(); i++) {
            releaseBucket((*buckets)[i]);
        }
        delete buckets;

        buckets = new Vector<CFile::Bucket *>(*(toAssign.buckets));
        for (size_t i = 0; i < (*buckets).getSize(); i++) {
            ((*buckets)[i]->refCount)++;
        }
    }
};

class CFile::Version {
    Data * data;

    void releaseData() {
        if (--(data->refCount) == 0) {
            delete data;
        }
    }

public:

    Version() {
        data = new Data();
        data->refCount = 1;
    }

    Version(const Version & toCopy) {
        data = toCopy.data;
        (data->refCount)++;
    }

    ~Version() {
        releaseData();
    }

    Version & operator = (const Version & toAssign) {
        if (this == &toAssign) {
            return *this;
        }

        releaseData();
        data = toAssign.data;
        (data->refCount)++;

        return(*this);
    }

    void prepareEdit() {
        if ((*data).refCount > 1) {
            Data * newData = new Data(*data);
            (data->refCount)--;
            data = newData;
        }
    }

    class Iterator {
        size_t posInData;
        size_t posInBucket;
    };
};

*/
#ifndef __PROGTEST__
#include <iostream>
/*
bool writeTest(CFile & x, const initializer_list<uint8_t> & data, uint32_t wrLen) {
    return x.write(data.begin(), data.size()) == wrLen;
}

bool readTest(CFile & x, const initializer_list<uint8_t> & data, uint32_t rdLen) {
    uint8_t tmp[100];
    uint32_t idx = 0;

    if (x.read(tmp, rdLen) != data.size()) {
        return false;
    }
    for (auto v : data) {
        if (tmp[idx++] != v) {
            return false;
        }
    }
    return true;
}*/


void listTest() {

    List<int> l0;
    l0.pushBack(0);
    l0.pushBack(1);
    l0.pushBack(2);
    l0.pushBack(3);
    assert(l0.back() == 3);
    assert(l0.getSize() == 4);
    List<int>::ListIterator i0(l0, 1);
    assert(*(i0++) == 1);
    assert(*i0 == 2);
    assert(*(++i0) == 3);
    i0 = List<int>::ListIterator(l0, 1);
    l0.insert(i0, 10);
    assert(*(++i0) == 10);
    assert(l0.getSize() == 5);

    List<int> l1(l0);
    l1.remove(List<int>::ListIterator(l1, 1));
    assert(l0.getSize() == 5);
    assert(l1.getSize() == 4);
    assert(l1.remove(nullptr) == 0);
    assert(l1.getSize() == 3);
    assert(l1.remove(List<int>::ListIterator(l1, 1)));

    List<int> l2;

    l2 = l1;
    l2.insert(List<int>::ListIterator(l2, 1), 3);
    assert(l1.getSize() == 2);
    assert(l2.getSize() == 3);
    assert(l2.popFront() == 1);
    assert(l2.popFront() == 2);
    assert(l2.popFront() == 3);
    assert(l2.getSize() == 0);
    l2.pushFront(10);
    l2.pushFront(5);
    assert(l2.back() == 10);
    assert(l2.front() == 5);
    assert(l2.getSize() == 2);
}

int main(void) {

    listTest();

    /*
    CFile f0;
    assert(writeTest(f0, { 10, 20, 30 }, 3));
    assert(f0.fileSize() == 3);
    assert(writeTest(f0, { 60, 70, 80 }, 3));
    assert(f0.fileSize() == 6);
    assert(f0.seek(2));
    assert(writeTest(f0, { 5, 4 }, 2));
    assert(f0.fileSize() == 6);
    assert(f0.seek(1));
    assert(readTest(f0, { 20, 5, 4, 70, 80 }, 7));
    assert(f0.seek(3));
    f0.addVersion();
    assert(f0.seek(6));
    assert(writeTest(f0, { 100, 101, 102, 103 }, 4));
    f0.addVersion();
    assert(f0.seek(5));
    CFile f1(f0);
    f0.truncate();
    assert(f0.seek(0));
    assert(readTest(f0, { 10, 20, 5, 4, 70 }, 20));
    assert(f0.undoVersion());
    assert(f0.seek(0));
    assert(readTest(f0, { 10, 20, 5, 4, 70, 80, 100, 101, 102, 103 }, 20));
    assert(f0.undoVersion());
    assert(f0.seek(0));
    assert(readTest(f0, { 10, 20, 5, 4, 70, 80 }, 20));
    assert(!f0.seek(100));
    assert(writeTest(f1, { 200, 210, 220 }, 3));
    assert(f1.seek(0));
    assert(readTest(f1, { 10, 20, 5, 4, 70, 200, 210, 220, 102, 103 }, 20));
    assert(f1.undoVersion());
    assert(f1.undoVersion());
    assert(readTest(f1, { 4, 70, 80 }, 20));
    assert(!f1.undoVersion());
    */
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
