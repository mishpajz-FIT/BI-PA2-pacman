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
        Node * prev;

        Node(T newValue) : value(newValue), next(nullptr), prev(nullptr) { }

        Node(const Node & toCopy) : value(toCopy.value), next(nullptr), prev(nullptr) { }
    };

    Node * head;
    Node * tail;

    size_t size;

public:
    struct ListIterator {
        Node * ptr;

        ListIterator(const List & list, size_t position) : ptr(list.head) {
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

        bool isAtEnd() {
            return (ptr == nullptr);
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

        ListIterator & operator --() {
            ptr = ptr->prev;
            return (*this);
        }

        ListIterator operator --(int) {
            ListIterator old(*this);
            --(*this);
            return old;
        }
    };

    List() : head(nullptr), tail(nullptr), size(0) { }

    List(const List & copyFrom) : head(nullptr), tail(nullptr), size(copyFrom.size) {
        if (copyFrom.head != nullptr) {
            head = new Node(*(copyFrom.head));
            Node * iter = head;
            Node * iterCopy = copyFrom.head->next;
            while (iterCopy != nullptr) {
                iter->next = new Node(*(iterCopy));
                iter->next->prev = iter;
                iter = iter->next;
                iterCopy = iterCopy->next;
            }
            tail = iter;
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

    void insert(Node * before, const T & value) {
        Node * newNode = new Node(value);

        if (before == nullptr) { //If inserting at end, set new tail
            newNode->prev = tail;
            tail = newNode;
        } else { //Set new node's prev from 'before' node's prev, set before's node prev as new node
            newNode->prev = before->prev;
            before->prev = newNode;
        }

        if (newNode->prev == nullptr) { //If at begin, set new hail
            head = newNode;
        } else { //Set next of new node's prev to new node
            (newNode->prev)->next = newNode;
        }
        newNode->next = before; //Set next of new node to before

        size++;
    }

    void insert(ListIterator before, const T & value) {
        insert(before.ptr, value);
    }

    T remove(Node * at) {
        T value(at->value);

        if (at->prev != nullptr) {
            at->prev->next = at->next;
        } else {
            head = at->next;
        }

        if (at->next != nullptr) {
            at->next->prev = at->prev;
        } else {
            tail = at->prev;
        }

        delete at;
        size--;

        return value;
    }

    T remove(ListIterator & at) {
        ListIterator old = at;
        if (at.ptr == tail) {
            at--;
        } else {
            at++;
        }
        return remove(old.ptr);
    }

    void pushFront(const T & value) {
        insert(head, value);
    }

    T popFront() {
        return remove(head);
    }

    void pushBack(const T & value) {
        insert(nullptr, value);
    }

    T popBack() {
        return remove(tail);
    }

    T & back() {
        return tail->value;
    }

    const T & back() const {
        return tail->value;
    }

    T & front() {
        return head->value;
    }

    const T & front() const {
        return head->value;
    }

    ListIterator begin() const {
        return ListIterator(head);
    }

    ListIterator end() const {
        return ListIterator(nullptr);
    }

    ListIterator iterator(size_t at = 0) const {
        return ListIterator(*this, at);
    }

    size_t getSize() const {
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
    assert(l0.getSize() == 0);
    l0.pushBack(1);
    assert(l0.getSize() == 1);
    l0.pushFront(0);
    l0.pushBack(2);
    l0.pushBack(3);
    auto it0 = l0.begin();
    assert(*(it0++) == 0);
    assert(*it0 == 1);
    assert(*(++it0) == 2);
    assert(*(it0--) == 2);
    assert(*(it0) == 1);
    assert(*(--it0) == 0);

    List<int> l1(l0);
    l1.pushBack(4);
    assert(l0.getSize() == 4);
    assert(l1.getSize() == 5);
    assert(l0.back() == 3);
    assert(l1.back() == 4);

    List<int> l2;
    l2 = l1;
    assert(l2.popFront() == 0);
    assert(l1.getSize() == 5);
    assert(l2.getSize() == 4);

    auto it1 = l2.iterator(1); //1, 2, 3, 4
    l2.remove(it1);
    assert(l2.getSize() == 3);
    auto it2 = l2.begin();
    assert(*(it2++) == 1);
    assert(*(it2) == 3);
    l2.insert(it2, 10);
    it2 = l2.begin();
    assert(*(it2++) == 1);
    assert(*(it2++) == 10);
    assert(*(it2) == 3);

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
