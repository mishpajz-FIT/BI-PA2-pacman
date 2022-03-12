#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream> 
#include <iomanip> 
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */


class LittleHeap {
private:
    vector<unsigned int> nodes;
    bool max;

    bool compare(unsigned int & lhs, unsigned int rhs) {
        if (max) {
            return lhs < rhs;
        } else {
            return lhs > rhs;
        }
    }

public:

    LittleHeap(bool isMaxHeap) : max(isMaxHeap) { }

    void push(unsigned int value) {
        size_t currentIndex = nodes.size();

        nodes.push_back(value);

        size_t parentIndex;
        while (true) {
            parentIndex = (currentIndex - 1) / 2;

            if (currentIndex == 0) {
                break;
            }

            if (compare(nodes[parentIndex], value)) {
                swap(nodes[parentIndex], nodes[currentIndex]);
            } else {
                break;
            }

            currentIndex = parentIndex;
        }
    }

    unsigned int pop() {
        if (nodes.size() == 0) {
            throw (-1);
        }

        unsigned int value = nodes.front();

        nodes.front() = nodes.back();
        nodes.pop_back();

        if (nodes.size() < 2) {
            return value;
        }

        size_t currentIndex = 0;
        size_t leftChild;
        size_t rightChild;

        size_t greaterNode;
        size_t secondGreaterNode;

        while (true) {

            leftChild = (2 * currentIndex) + 1;
            rightChild = (2 * currentIndex) + 2;

            if (nodes.size() > rightChild) {
                if (compare(nodes[leftChild], nodes[rightChild])) {
                    greaterNode = rightChild;
                    secondGreaterNode = leftChild;
                } else {
                    greaterNode = leftChild;
                    secondGreaterNode = rightChild;
                }

                if (compare(nodes[currentIndex], nodes[greaterNode])) {
                    swap(nodes[currentIndex], nodes[greaterNode]);

                    currentIndex = greaterNode;
                    continue;
                } else if (compare(nodes[currentIndex], nodes[secondGreaterNode])) {
                    swap(nodes[currentIndex], nodes[secondGreaterNode]);

                    currentIndex = secondGreaterNode;

                    continue;
                } else {
                    break;
                }
            } else if (nodes.size() > leftChild) {
                if (compare(nodes[currentIndex], nodes[leftChild])) {
                    swap(nodes[currentIndex], nodes[leftChild]);

                    currentIndex = leftChild;
                    continue;
                } else {
                    break;
                }
            } else {
                break;
            }
        }

        return value;
    }

    unsigned int top() {
        if (nodes.size() == 0) {
            throw (-1);
        }
        return nodes.front();
    }

    size_t size() {
        return nodes.size();
    }
};

class CVATRegister {
private:

    struct CompanyContact {
        string name;
        string address;

        unsigned int * invoices;

        CompanyContact(string newName, string newAddress) : name(newName), address(newAddress) { }

        bool operator == (const CompanyContact & rhs) const {
            return ((strcasecmp(name.c_str(), rhs.name.c_str()) == 0) && (strcasecmp(address.c_str(), rhs.address.c_str()) == 0));
        }

        bool operator != (const CompanyContact & rhs) const {
            return !(*this == rhs);
        }

        bool operator < (const CompanyContact & rhs) const {
            if (strcasecmp(name.c_str(), rhs.name.c_str()) == 0) {
                return strcasecmp(address.c_str(), rhs.address.c_str()) < 0;
            }
            return strcasecmp(name.c_str(), rhs.name.c_str()) < 0;
        }
    };

    struct CompanyID {
        string id;

        unsigned int * invoices;

        CompanyID(string newId) : id(newId) { }

        bool operator == (const CompanyID & rhs) const {
            return (id == rhs.id);
        }

        bool operator != (const CompanyID & rhs) const {
            return !(*this == rhs);
        }

        bool operator < (const CompanyID & rhs) const {
            return id < rhs.id;
        }
    };

    vector<CompanyContact> companyContacts;
    vector<CompanyID> companyIds;

    const vector<CompanyContact>::const_iterator getContactIter(const string & name, const string & addr) const {
        CompanyContact newContact(name, addr);

        auto contactBound = lower_bound(companyContacts.begin(), companyContacts.end(), newContact);
        if (contactBound == companyContacts.end() || newContact != (*contactBound)) {
            throw (-1);
        }

        return contactBound;
    }

    const vector<CompanyID>::const_iterator getIDIter(const string & taxID) const {
        CompanyID newId(taxID);

        auto idBound = lower_bound(companyIds.begin(), companyIds.end(), newId);
        if (idBound == companyIds.end() || newId != (*idBound)) {
            throw (-1);
        }

        return idBound;
    }

public:
    CVATRegister(void) : medianLowerHalf(true), medianUpperHalf(false) { }

    ~CVATRegister(void) {
        for (auto & a : companyIds) {
            delete a.invoices;
        }
    }

    bool newCompany(const string & name, const string & addr, const string & taxID) {
        CompanyID newId(taxID);
        CompanyContact newContact(name, addr);

        unsigned int * newInvoice = new unsigned int(0);
        newId.invoices = newInvoice;
        newContact.invoices = newInvoice;

        auto idBound = lower_bound(companyIds.begin(), companyIds.end(), newId);
        if (idBound != companyIds.end() && newId == (*idBound)) {
            delete newInvoice;
            return false;
        }

        auto contactBound = lower_bound(companyContacts.begin(), companyContacts.end(), newContact);
        if (contactBound != companyContacts.end() && newContact == (*contactBound)) {
            delete newInvoice;
            return false;
        }

        companyIds.insert(idBound, move(newId));
        companyContacts.insert(contactBound, move(newContact));

        return true;
    }

    bool cancelCompany(const string & name, const string & addr) {
        CompanyContact newContact(name, addr);

        auto contactBound = lower_bound(companyContacts.begin(), companyContacts.end(), newContact);
        if (contactBound == companyContacts.end() || newContact != (*contactBound)) {
            return false;
        }

        for (auto it = companyIds.begin(); it != companyIds.end(); it++) {
            if ((*it).invoices == (*contactBound).invoices) {
                delete (*contactBound).invoices;
                companyIds.erase(it);
                companyContacts.erase(contactBound);
                return true;
            }
        }

        return false;
    }

    bool cancelCompany(const string & taxID) {
        CompanyID newId(taxID);

        auto idBound = lower_bound(companyIds.begin(), companyIds.end(), newId);
        if (idBound == companyIds.end() || newId != (*idBound)) {
            return false;
        }

        for (auto it = companyContacts.begin(); it != companyContacts.end(); it++) {
            if ((*it).invoices == (*idBound).invoices) {
                delete (*idBound).invoices;
                companyIds.erase(idBound);
                companyContacts.erase(it);
                return true;
            }
        }

        return false;
    }

    bool invoice(const string & taxID, unsigned int amount) {
        try {
            *(getIDIter(taxID)->invoices) += amount;
        }
        catch (...) {
            return false;
        }
        return true;
    }

    bool invoice(const string & name, const string & addr, unsigned int amount) {
        try {
            *(getContactIter(name, addr)->invoices) += amount;
        }
        catch (...) {
            return false;
        }
        return true;
    }

    bool audit(const string & name, const string & addr, unsigned int & sumIncome) const {
        try {
            sumIncome = *(getContactIter(name, addr)->invoices);
        }
        catch (...) {
            return false;
        }
        return true;
    }

    bool audit(const string & taxID, unsigned int & sumIncome) const {
        try {
            sumIncome = *(getIDIter(taxID)->invoices);
        }
        catch (...) {
            return false;
        }
        return true;
    }

    bool firstCompany(string & name, string & addr) const {
        if (companyContacts.size() == 0) {
            return false;
        }

        name = companyContacts.front().name;
        addr = companyContacts.front().address;
        return true;
    }

    bool nextCompany(string & name, string & addr) const {
        try {
            auto contactBound = getContactIter(name, addr);

            contactBound++;
            if (contactBound == companyContacts.end()) {
                return false;
            }

            name = (*contactBound).name;
            addr = (*contactBound).address;
        }
        catch (...) {
            return false;
        }
        return true;
    }

    //unsigned int medianInvoice(void) const;
};

#ifndef __PROGTEST__
int main(void) {
    string name, addr;
    unsigned int sumIncome;

    CVATRegister b1;
    assert(b1.newCompany("ACME", "Thakurova", "666/666"));
    assert(b1.newCompany("ACME", "Kolejni", "666/666/666"));
    assert(b1.newCompany("Dummy", "Thakurova", "123456"));
    assert(b1.invoice("666/666", 2000));
    //assert(b1.medianInvoice() == 2000);
    assert(b1.invoice("666/666/666", 3000));
    //assert(b1.medianInvoice() == 3000);
    assert(b1.invoice("123456", 4000));
    //assert(b1.medianInvoice() == 3000);
    assert(b1.invoice("aCmE", "Kolejni", 5000));
    //assert(b1.medianInvoice() == 4000);
    assert(b1.audit("ACME", "Kolejni", sumIncome) && sumIncome == 8000);
    assert(b1.audit("123456", sumIncome) && sumIncome == 4000);
    assert(b1.firstCompany(name, addr) && name == "ACME" && addr == "Kolejni");
    assert(b1.nextCompany(name, addr) && name == "ACME" && addr == "Thakurova");
    assert(b1.nextCompany(name, addr) && name == "Dummy" && addr == "Thakurova");
    assert(!b1.nextCompany(name, addr));
    assert(b1.cancelCompany("ACME", "KoLeJnI"));
    //assert(b1.medianInvoice() == 4000);
    assert(b1.cancelCompany("666/666"));
    /*assert(b1.medianInvoice() == 4000);
    assert(b1.invoice("123456", 100));
    assert(b1.medianInvoice() == 3000);
    assert(b1.invoice("123456", 300));
    assert(b1.medianInvoice() == 3000);
    assert(b1.invoice("123456", 200));
    assert(b1.medianInvoice() == 2000);
    assert(b1.invoice("123456", 230));
    assert(b1.medianInvoice() == 2000);
    assert(b1.invoice("123456", 830));
    assert(b1.medianInvoice() == 830);
    assert(b1.invoice("123456", 1830));
    assert(b1.medianInvoice() == 1830);
    assert(b1.invoice("123456", 2830));
    assert(b1.medianInvoice() == 1830);
    assert(b1.invoice("123456", 2830));
    assert(b1.medianInvoice() == 2000);
    assert(b1.invoice("123456", 3200));
    assert(b1.medianInvoice() == 2000);*/
    assert(b1.firstCompany(name, addr) && name == "Dummy" && addr == "Thakurova");
    assert(!b1.nextCompany(name, addr));
    assert(b1.cancelCompany("123456"));
    assert(!b1.firstCompany(name, addr));

    CVATRegister b2;
    assert(b2.newCompany("ACME", "Kolejni", "abcdef"));
    assert(b2.newCompany("Dummy", "Kolejni", "123456"));
    assert(!b2.newCompany("AcMe", "kOlEjNi", "1234"));
    assert(b2.newCompany("Dummy", "Thakurova", "ABCDEF"));
    //assert(b2.medianInvoice() == 0);
    assert(b2.invoice("ABCDEF", 1000));
    //assert(b2.medianInvoice() == 1000);
    assert(b2.invoice("abcdef", 2000));
    //assert(b2.medianInvoice() == 2000);
    assert(b2.invoice("aCMe", "kOlEjNi", 3000));
    //assert(b2.medianInvoice() == 2000);
    assert(!b2.invoice("1234567", 100));
    assert(!b2.invoice("ACE", "Kolejni", 100));
    assert(!b2.invoice("ACME", "Thakurova", 100));
    assert(!b2.audit("1234567", sumIncome));
    assert(!b2.audit("ACE", "Kolejni", sumIncome));
    assert(!b2.audit("ACME", "Thakurova", sumIncome));
    assert(!b2.cancelCompany("1234567"));
    assert(!b2.cancelCompany("ACE", "Kolejni"));
    assert(!b2.cancelCompany("ACME", "Thakurova"));
    assert(b2.cancelCompany("abcdef"));
    //assert(b2.medianInvoice() == 2000);
    assert(!b2.cancelCompany("abcdef"));
    assert(b2.newCompany("ACME", "Kolejni", "abcdef"));
    assert(b2.cancelCompany("ACME", "Kolejni"));
    assert(!b2.cancelCompany("ACME", "Kolejni"));

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
