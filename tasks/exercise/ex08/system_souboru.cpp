
#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <functional>
#include <memory>
#include <stdexcept>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cctype>
#include <cstdint>
#include <cassert>
using namespace std;
#endif /* __PROGTEST__ */

class CBase {
public:
    CBase() { }

    virtual ~CBase() { }

    virtual unsigned int Size() const = 0;

    virtual shared_ptr<CBase> Clone() const = 0;

    virtual void Print(ostream & os) const = 0;
};

class CFile : public CBase {
    string hash;
    unsigned int filesize;
public:
    CFile(const string & h, unsigned int f) : CBase(), hash(h), filesize(f) { }

    virtual ~CFile() { }

    virtual unsigned int Size() const override {
        return filesize;
    }

    virtual shared_ptr<CBase> Clone() const override {
        return shared_ptr<CFile>(new CFile(*this));
    }

    virtual void Print(ostream & os) const override {
        os << " " << hash;
    }

    CFile & Change(const string & h, unsigned int f) {
        hash = h;
        filesize = f;

        return (*this);
    }
};

class CLink : public CBase {
    string path;
public:
    CLink(const string & p) : CBase(), path(p) { }

    virtual ~CLink() { }

    virtual unsigned int Size() const override {
        return path.length() + 1;
    }

    virtual shared_ptr<CBase> Clone() const override {
        return shared_ptr<CLink>(new CLink(*this));
    }

    virtual void Print(ostream & os) const override {
        os << " -> " << path;
    }

    CLink & Change(const string & p) {
        path = p;

        return (*this);
    }
};

class CDirectory : public CBase {
    map<string, shared_ptr<CBase>> files;
public:
    CDirectory() : CBase(), files() { }

    CDirectory(const CDirectory & toCopy) : CBase(), files(toCopy.files) { }

    virtual ~CDirectory() { }

    CDirectory & operator = (const CDirectory & toCopy) {
        files.clear();
        files = toCopy.files;
        return *this;
    }

    virtual unsigned int Size() const override {
        unsigned int sum = 0;
        for (const auto & x : files) {
            sum += x.first.length();
            sum += x.second->Size();
        }
        return sum;
    }

    virtual shared_ptr<CBase> Clone() const override {
        return shared_ptr<CDirectory>(new CDirectory(*this));
    }

    CDirectory & Change(const string & filename, const CBase & file) {
        files[filename] = file.Clone();
        return (*this);
    }

    CDirectory & Change(const string & filename, CBase * file) {
        CBase * filePtr = dynamic_cast<CBase *>(file);
        if (filePtr == nullptr) {
            files.erase(filename);
        } else {
            files[filename] = filePtr->Clone();
        }
        return (*this);
    }

    CBase & Get(const string & filename) {
        auto iter = files.find(filename);
        if (iter == files.end()) {
            throw std::out_of_range("err");
        }
        return *((*iter).second);
    }

    const CBase & Get(const string & filename) const {
        return const_cast<const CBase &>(Get(filename));
    }

    virtual void Print(ostream & os) const override {
        os << "/";
    }

    friend ostream & operator << (ostream & os, const CDirectory & directory) {
        for (const auto & f : directory.files) {
            os << f.second->Size() << "\t" << f.first;
            f.second->Print(os);
            os << endl;
        }
        return os;
    }
};

#ifndef __PROGTEST__
int main() {
    CDirectory root;
    stringstream sout;

    root.Change("file.txt", CFile("jhwadkhawkdhajwdhawhdaw=", 1623))
        .Change("file.ln", CLink("").Change("file.txt"))
        .Change("folder", CDirectory()
            .Change("fileA.txt", CFile("", 0).Change("skjdajdakljdljadkjwaljdlaw=", 1713))
            .Change("fileB.txt", CFile("kadwjkwajdwhoiwhduqwdqwuhd=", 71313))
            .Change("fileC.txt", CFile("aihdqhdqudqdiuwqhdquwdqhdi=", 8193))
        );

    sout.str("");
    sout << root;
    assert(sout.str() ==
        "9\tfile.ln -> file.txt\n"
        "1623\tfile.txt jhwadkhawkdhajwdhawhdaw=\n"
        "81246\tfolder/\n");
    assert(root.Size() == 82899);

    string filename = "folder";
    const CDirectory & inner = dynamic_cast<const CDirectory &>(root.Get(filename));

    sout.str("");
    sout << inner;
    assert(sout.str() ==
        "1713\tfileA.txt skjdajdakljdljadkjwaljdlaw=\n"
        "71313\tfileB.txt kadwjkwajdwhoiwhduqwdqwuhd=\n"
        "8193\tfileC.txt aihdqhdqudqdiuwqhdquwdqhdi=\n");
    assert(inner.Size() == 81246);



    CDirectory dir;
    dir.Change("testfile", CFile("jhwadkhawkdhajwdhawhdaw=", 1623));
    dir.Change("testfile", nullptr);


    dir.Change("testfile", CFile("jhwadkhawkdhajwdhawhdaw=", 1623));
    CDirectory dir2;
    dir2 = dir;
    dir2.Change("testfile", nullptr);
    assert(dir.Size() == 1631);
    assert(dir2.Size() == 0);


    return 0;
}
#endif /* __PROGTEST__ */
