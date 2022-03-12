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

class CVATRegister {
private:
    struct TaxInvoice {
        unsigned int amount;

        TaxInvoice() : amount(0) { }
    };

    struct CompanyContact {
        string name;
        string address;

        TaxInvoice * invoice;

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

        TaxInvoice * invoice;

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
    /*CVATRegister(void);
    ~CVATRegister(void);*/

    bool newCompany(const string & name, const string & addr, const string & taxID) {
        CompanyID newId(taxID);
        CompanyContact newContact(name, addr);

        TaxInvoice * newInvoice = new TaxInvoice;
        newId.invoice = newInvoice;
        newContact.invoice = newInvoice;

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
            if ((*it).invoice == (*contactBound).invoice) {
                delete (*contactBound).invoice;
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
            if ((*it).invoice == (*idBound).invoice) {
                delete (*idBound).invoice;
                companyIds.erase(idBound);
                companyContacts.erase(it);
                return true;
            }
        }

        return false;
    }

    bool invoice(const string & taxID, unsigned int amount) {
        try {
            (*getIDIter(taxID)).invoice->amount += amount;
        }
        catch (...) {
            return false;
        }
        return true;
    }

    bool invoice(const string & name, const string & addr, unsigned int amount) {
        try {
            (*getContactIter(name, addr)).invoice->amount += amount;
        }
        catch (...) {
            return false;
        }
        return true;
    }

    bool audit(const string & name, const string & addr, unsigned int & sumIncome) const {
        try {
            sumIncome = (*getContactIter(name, addr)).invoice->amount;
        }
        catch (...) {
            return false;
        }
        return true;
    }

    bool audit(const string & taxID, unsigned int & sumIncome) const {
        try {
            sumIncome = (*getIDIter(taxID)).invoice->amount;
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
