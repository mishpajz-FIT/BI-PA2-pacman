
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

    virtual CBase * Clone() const = 0;

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

    virtual CFile * Clone() const override {
        return (new CFile(*this));
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

    virtual CLink * Clone() const override {
        return (new CLink(*this));
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
    map<string, CBase *> files;

    void deallocFiles() {
        for (auto iter = files.begin(); iter != files.end(); iter++) {
            delete (*iter).second;
        }
        files.clear();
    }
public:
    CDirectory() : CBase(), files() { }

    CDirectory(const CDirectory & toCopy) : CBase() {
        for (auto iter = toCopy.files.begin(); iter != toCopy.files.end(); iter++) {
            files[(*iter).first] = (*iter).second->Clone();
        }
    }

    virtual ~CDirectory() {
        deallocFiles();
    }

    CDirectory & operator = (const CDirectory & toCopy) {
        if (this == &toCopy) {
            return *this;
        }

        deallocFiles();

        for (auto iter = toCopy.files.begin(); iter != toCopy.files.end(); iter++) {
            files[(*iter).first] = (*iter).second->Clone();
        }

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

    virtual CDirectory * Clone() const override {
        return (new CDirectory(*this));
    }

    CDirectory & Change(const string & filename, const CBase & file) {
        CBase * copy = file.Clone();

        auto iter = files.find(filename);
        if (iter != files.end()) {
            delete (*iter).second;
        }

        files[filename] = copy;
        return (*this);
    }

    CDirectory & Change(const string & filename, void * file) {
        if (file == nullptr) {
            auto iter = files.find(filename);
            if (iter != files.end()) {
                delete (*iter).second;
            }

            files.erase(filename);
        }
        return (*this);
    }

    CBase & Get(const string & filename) {
        auto iter = files.find(filename);
        if (iter == files.end()) {
            throw std::out_of_range("");
        }
        return *((*iter).second);
    }

    const CBase & Get(const string & filename) const {
        auto iter = files.find(filename);
        if (iter == files.end()) {
            throw std::out_of_range("");
        }
        return *((*iter).second);
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



    CDirectory dir1;
    dir1.Change("testfile", CFile("jhwadkhawkdhajwdhawhdaw=", 1623));
    dir1.Change("testfile", nullptr);


    dir1.Change("testfile", CFile("jhwadkhawkdhajwdhawhdaw=", 1623));
    CDirectory dir2;
    dir2 = dir1;
    dir2.Change("testfile", nullptr);
    assert(dir1.Size() == 1631);
    assert(dir2.Size() == 0);

    CDirectory dir3;
    dir3.Change("self", dir3);
    dir3.Change("self", dir3);

    CDirectory dir4;
    dir4.Change("folder", CDirectory());
    CDirectory & dir4Folder = dynamic_cast<CDirectory &>(dir4.Get("folder"));
    dir4Folder.Change("folder", nullptr);
    dir4Folder.Change("folder", dir4);
    dir4.Change("folder", dir4);

    return 0;
}
#endif /* __PROGTEST__ */
