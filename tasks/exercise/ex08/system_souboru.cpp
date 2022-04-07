/**
 * @file system_souboru.cpp
 * @author Michal Dobeš 
 * @date 2022-04-07
 *
 * @brief A set of classes that represents the file system, with inheritance from abstract class
 *
 * @copyright Copyright (c) 2022
 *
 */

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

// SECTION: Implementation

/**
 * @brief Abstract class from which all parts of the file system inherit
 *
 */
class CBase {
public:
    CBase() { }

    virtual ~CBase() { }

    /**
     * @brief Size of element
     *
     * @return unsigned int Size
     */
    virtual unsigned int Size() const = 0;

    /**
     * @brief Allocate new copy of this object and returns pointer to it
     *
     * @return CBase* Pointer to new copy
     */
    virtual CBase * Clone() const = 0;

    /**
     * @brief Print information about object into stream
     *
     * @param os Stream to print into
     */
    virtual void Print(ostream & os) const = 0;
};

/**
 * @brief Representation of file
 *
 */
class CFile : public CBase {
    string hash; //< Stored attribute of file, hash
    unsigned int filesize; //< Stored size
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

    /**
     * @brief Change object properties
     *
     * @param h Hash
     * @param f File size
     * @return CFile& This
     */
    CFile & Change(const string & h, unsigned int f) {
        hash = h;
        filesize = f;

        return (*this);
    }
};

/**
 * @brief Representation of symlink
 *
 */
class CLink : public CBase {
    string path; //< Stored attribute of symlink, path
public:
    CLink(const string & p) : CBase(), path(p) { }

    virtual ~CLink() { }

    virtual unsigned int Size() const override {
        return path.length() + 1; //< Size should be length of path + 1
    }

    virtual CLink * Clone() const override {
        return (new CLink(*this));
    }

    virtual void Print(ostream & os) const override {
        os << " -> " << path;
    }

    /**
     * @brief Change object properties
     *
     * @param p Path
     * @return CLink& This
     */
    CLink & Change(const string & p) {
        path = p;

        return (*this);
    }
};

/**
 * @brief Representation of directory
 *
 * Can store other parts of filesystem
 */
class CDirectory : public CBase {
    map<string, CBase *> files; //< Contents of this directory

    /**
     * @brief Deallocates all contents of this directory in CDirectory::files
     *
     */
    void deallocFiles() {
        for (auto iter = files.begin(); iter != files.end(); iter++) {
            delete (*iter).second;
        }
        files.clear();
    }
public:
    CDirectory() : CBase(), files() { }

    CDirectory(const CDirectory & toCopy) : CBase() { //< Copy constructor
        // Copy and allocate all contents of directory
        for (auto iter = toCopy.files.begin(); iter != toCopy.files.end(); iter++) {
            files[(*iter).first] = (*iter).second->Clone();
        }
    }

    virtual ~CDirectory() {
        deallocFiles();
    }

    CDirectory & operator = (const CDirectory & toCopy) {
        if (this == &toCopy) { //< Self-assignment check
            return *this;
        }

        deallocFiles(); //< Deallocate current content of directory

        for (auto iter = toCopy.files.begin(); iter != toCopy.files.end(); iter++) { //< Copy and allocate all contents of directory
            files[(*iter).first] = (*iter).second->Clone();
        }

        return *this;
    }

    virtual unsigned int Size() const override {
        unsigned int sum = 0;
        for (const auto & x : files) { //< Size is sum of path length and size of contents
            sum += x.first.length();
            sum += x.second->Size();
        }
        return sum;
    }

    virtual CDirectory * Clone() const override {
        return (new CDirectory(*this));
    }

    /**
     * @brief Changes (or adds) file to this directory
     *
     * @param filename Name of file
     * @param file File to add/change
     * @return CDirectory& This object
     */
    CDirectory & Change(const string & filename, const CBase & file) {
        CBase * copy = file.Clone(); //< Allocate file first (because of self-addition, copy needs to be created before any other action)

        Change(filename, nullptr); //< If file with this name exists, deallocate it

        files[filename] = copy; //< Assign copy of the file
        return (*this);
    }

    /**
     * @brief Removes file from this directory
     *
     * @param filename Name of file
     * @param file Nullptr if the file should be removed
     * @return CDirectory& This object
     */
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

    /**
     * @brief Get const reference to file in folder
     *
     * @param filename Name of file
     * @return CBase& Const reference to file
     */
    const CBase & Get(const string & filename) const {
        auto iter = files.find(filename);
        if (iter == files.end()) {
            throw std::out_of_range("");
        }
        return *((*iter).second);
    }

    /**
     * @brief Get reference to file in folder
     *
     * @param filename Name of file
     * @return CBase& Reference to file
     */
    CBase & Get(const string & filename) {
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
        for (const auto & f : directory.files) { //< Print each file
            os << f.second->Size() << "\t" << f.first;
            f.second->Print(os);
            os << endl;
        }
        return os;
    }
};

// !SECTION
// SECTION: tests

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

// !SECTION