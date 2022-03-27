#ifndef __PROGTEST__
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <iostream>
using namespace std;
#endif /* __PROGTEST__ */

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
        size_t posIn
    };
};

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

int main(void) {

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
