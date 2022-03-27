#ifndef __PROGTEST__
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <iostream>
using namespace std;
#endif /* __PROGTEST__ */

template<typename T>
class Vector {
private:
    static const size_t growMultiplier;

    T * data;
    size_t size;
    size_t capacity;

    void elongateTo(size_t newCapacity) {
        T * newData = new T[newCapacity];
        for (size_t i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        capacity = newCapacity;
        delete [] data;
        data = newData;
    }

    void createNewCapacity() {
        if (size >= capacity) {
            elongateTo((capacity * growMultiplier) + 2);
        }
    }

public:
    Vector() : data(nullptr), size(0), capacity(0) { }

    Vector(const Vector & toCopy) : size(toCopy.size), capacity(toCopy.size) {
        data = new T[toCopy.size];
        for (size_t i = 0; i < size; i++) {
            data[i] = toCopy.data[i];
        }
    }

    Vector & operator = (const Vector & toAssign) {
        if (this == &toAssign) {
            return (*this);
        }

        Vector copy(toAssign);
        size = copy.size;
        capacity = copy.capacity;

        T * tmpData = copy.data;
        copy.data = data;
        data = tmpData;

        return (*this);
    }

    ~Vector() {
        delete [] data;
    }

    void insert(const T & t, size_t at) {
        if (at > size) {
            throw "Insert behind last element of vector!";
        }
        createNewCapacity();

        for (size_t i = size; i > at; i--) {
            data[i] = data[i - 1];
        }
        size++;

        data[at] = t;
    }

    void push(const T & t) {
        createNewCapacity();
        data[size++] = t;
    }

    T pop() {
        if (size == 0) {
            throw "Pop on empty vector!";
        }
        return data[--size];
    }

    T & at(size_t i) {
        if (i >= size) {
            throw "Insert behind last element of vector!";
        }
        return data[i];
    }

    T & top() {
        if (size == 0) {
            throw "Top on empty vector!";
        }
        return data[size - 1];
    }

    const T & top() const {
        return (const_cast<Vector &>(*this).top());
    }

    const T & at(size_t i) const {
        return (const_cast<Vector &>(*this).at(i));
    }

    T & operator [] (size_t i) {
        return data[i];
    }

    const T & operator [] (size_t i) const {
        return data[i];
    }

    size_t getSize() const {
        return size;
    }
};

template<typename T>
const size_t Vector<T>::growMultiplier = 2;

/*
class CFile {
private:
    struct Bucket;
    struct Version;

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
    uint32_t size;
    unsigned int refCount;
    uint8_t * bytes;
};

struct CFile::Version {
    unsigned int refCount;

    Vector<Bucket *> data;

    class Iterator {
    private:
        size_t posInData;
        size_t posInBucket;
    public:

    };
};*/

#ifndef __PROGTEST__
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

void vectorTest() {
    Vector<int> v0;
    v0.push(1);
    v0.push(2);
    v0.push(3);
    assert(v0.top() == 3);
    assert(v0.getSize() == 3);
    assert(v0.at(1) == 2);
    v0.insert(5, 1);
    assert(v0.top() == 3);
    assert(v0.getSize() == 4);
    assert(v0[0] == 1);
    assert(v0.at(1) == 5);
    assert(v0[2] == 2);
    Vector<int> v1;
    v1 = v0;
    v1.push(4);
    assert(v0.pop() == 3);
    assert(v1.getSize() == 5);
    assert(v0.getSize() == 3);
    assert(v1.pop() == 4);
}

int main(void) {

    vectorTest();

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
