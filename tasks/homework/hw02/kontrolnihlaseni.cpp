/**
 * @file kontrolnihlaseni.cpp
 * @author Michal Dobeš
 * @date 2022-03-13
 *
 * @brief Registry of values, each value is specified by two uniqe identificators
 * Calculation of median from inputted values.
 *
 * Optimised for speed of access to information.
 *
 * @copyright Copyright (c) 2022
 *
 */
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


/**
 * @brief Heap abstract template
 *
 */
template <typename T>
class LittleHeap {
private:
    vector<T> nodes; //Vector used for storing data

    virtual bool compare(T & lhs, T rhs) const = 0; //Compare function supplied by ancestor

public:

    /**
     * @brief Construct a new Heap object
     *
     */
    LittleHeap() { }

    virtual ~LittleHeap() = default;

    /**
     * @brief Add element into heap
     *
     * @param value
     */
    void push(const T & value) {
        size_t currentIndex = nodes.size();

        nodes.push_back(value); //Add to the back of the vectors

        size_t parentIndex;
        while (true) {
            parentIndex = (currentIndex - 1) / 2; //Calculate index of parent node

            if (currentIndex == 0) { //If in root, stop the loop
                break;
            }

            if (compare(nodes[parentIndex], value)) { //Compare value with parent, if parent is larger/smaller (depending on heap type) switch values
                swap(nodes[parentIndex], nodes[currentIndex]);
            } else {
                break;
            }

            currentIndex = parentIndex;
        }
    }

    /**
     * @brief Remove top element from heap
     *
     * Throws if heap is empty
     *
     * @return unsigned int Removed element
     */
    T pop() {
        if (nodes.size() == 0) {
            throw (-1);
        }

        T value = nodes.front();
        //Remove root (first value), add last value as root
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

        while (true) { //Reorder until in correct order

            leftChild = (2 * currentIndex) + 1; //Calculate left child of current node
            rightChild = (2 * currentIndex) + 2;

            if (nodes.size() > rightChild) { // If both childern are in the heap
                if (compare(nodes[leftChild], nodes[rightChild])) { // Get childern with larger/smaller (depending on heap type) value
                    greaterNode = rightChild;
                    secondGreaterNode = leftChild;
                } else {
                    greaterNode = leftChild;
                    secondGreaterNode = rightChild;
                }

                if (compare(nodes[currentIndex], nodes[greaterNode])) { //If current node larger/smaller (depending on heap type) than first childern, switch values
                    swap(nodes[currentIndex], nodes[greaterNode]);

                    currentIndex = greaterNode; //Move to the switched node for next loop
                    continue;
                } else if (compare(nodes[currentIndex], nodes[secondGreaterNode])) { //If current node larger/smaller (depending on heap type) than second childern, switch values
                    swap(nodes[currentIndex], nodes[secondGreaterNode]);

                    currentIndex = secondGreaterNode;   //Move to the switched node for next loop

                    continue;
                } else { //If cant switch nodes break (node is in its place)
                    break;
                }
            } else if (nodes.size() > leftChild) { //If only one childern in the heap
                if (compare(nodes[currentIndex], nodes[leftChild])) { //If current node larger/smaller  (depending on heap type) than childern, switch values
                    swap(nodes[currentIndex], nodes[leftChild]);

                    currentIndex = leftChild;
                    continue;
                } else { //If cant switch nodes break (node is in its place)
                    break;
                }
            } else { //If no childern break (node is in its place)
                break;
            }
        }

        return value;
    }

    /**
     * @brief Get top element in heap
     *
     * Throws if heap is empty
     *
     * For Max heap largest element, for Min heap smallest element
     *
     * @return unsigned int Top element
     */
    T top() const {
        if (nodes.size() == 0) {
            throw (-1);
        }
        return nodes.front();
    }

    /**
     * @brief Get amount of elements in heap
     *
     * @return size_t amount of elements in heap
     */
    size_t size() const {
        return nodes.size();
    }
};

/**
 * @brief Min heap
 *
 * Adds compare function, rest of logic is handled by LittleHeap
 * @see LittleHeap
 *
 * @tparam T Heap data type
 */
template <typename T>
class MinHeap : public LittleHeap<T> {
    bool compare(T & lhs, T rhs) const override {
        return lhs > rhs;
    }
};

/**
 * @brief Max heap
 *
 * Adds compare function, rest of logic is handled by LittleHeap
 * @see LittleHeap
 *
 * @tparam T Heap data type
 */
template <typename T>
class MaxHeap : public  LittleHeap<T> {
    bool compare(T & lhs, T rhs) const override {
        return lhs < rhs;
    }
};

/**
 * @brief Registry of values, each value is specified by two uniqe identificators (name and address) and (id)
 *
 * Supports calculation of the median of all values by which the values in the register have increased.
 *
 */
class CVATRegister {
private:

    /**
     * @brief Unique identification representation (name and address)
     *
     * When comparing values, upper and lower case letters are not distinguished.
     *
     * Complementary to CVATRegister#CompanyID.
     *
     */
    struct CompanyContact {
        string name;
        string address;

        unsigned int * invoices; //Pointer to value

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

    /**
     * @brief Unique identification representation (ID)
     *
     * Complementary to CVATRegister#CompanyContact.
     *
     */
    struct CompanyID {
        string id;

        unsigned int * invoices; //Pointer to value

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

    vector<CompanyContact> companyContacts; //Storage of unique names and addresses (with pointers to values), sorted alphabetically
    vector<CompanyID> companyIds; //Storage of unique ids (with pointers to values), sorted alphabetically

    //The median is calculated quickly thanks to splitting all values into two halves
    MaxHeap<unsigned int> medianLowerHalf; //Values from the lower half are stored in the Max heap.
    MinHeap<unsigned int> medianUpperHalf; //values from the upper half are stored in the Min heap.

    /**
     * @brief Get iterator to searched contact from CVATRegister#companyContacts
     *
     * Throws if contact was not found.
     *
     * @param name Name value of contact
     * @param addr Address value of contact
     * @return const vector<CompanyContact>::const_iterator Iterator to CompanyContact in vector
     */
    const vector<CompanyContact>::const_iterator getContactIter(const string & name, const string & addr) const {
        CompanyContact newContact(name, addr); //Create temporary object

        //Use binary search to find values in the sorted vector
        auto contactBound = lower_bound(companyContacts.begin(), companyContacts.end(), newContact);
        if (contactBound == companyContacts.end() || newContact != (*contactBound)) {
            throw (-1);
        }

        return contactBound;
    }

    /**
     * @brief Get iterator to searched ID from CVATRegister#companyIds
     *
     * Throws if id was not found.
     *
     * @param taxID Id value of id
     * @return const vector<CompanyContact>::const_iterator Iterator to CompanyID in vector
     */
    const vector<CompanyID>::const_iterator getIDIter(const string & taxID) const {
        CompanyID newId(taxID); //Create temporary object

        //Use binary search to find values in the sorted vector
        auto idBound = lower_bound(companyIds.begin(), companyIds.end(), newId);
        if (idBound == companyIds.end() || newId != (*idBound)) {
            throw (-1);
        }

        return idBound;
    }

    /**
     * @brief Add a new value to memory to be taken into account when calculating the median
     *
     * @param amount New value
     */
    void addInvoiceToMedian(unsigned int amount) {
        //The median is calculated quickly thanks to splitting all values into two halves

        if (medianLowerHalf.size() == 0 && medianUpperHalf.size() == 0) { //If both halves are empty, add to lower
            medianLowerHalf.push(amount);
            return;
        }

        if (amount < medianLowerHalf.top()) { //If new value is smaller than largest value in bottom half, add to bottom half, else add to upper half
            medianLowerHalf.push(amount);
        } else {
            medianUpperHalf.push(amount);
        }

        //If one or the other half is too big (difference is bigger than 1), move biggest (or smallest) elements to the other half
        while (medianLowerHalf.size() > medianUpperHalf.size() + 1) {
            medianUpperHalf.push(medianLowerHalf.pop());
        }
        while (medianUpperHalf.size() > medianLowerHalf.size() + 1) {
            medianLowerHalf.push(medianUpperHalf.pop());
        }
    }

public:
    CVATRegister(void) : medianLowerHalf(), medianUpperHalf() { }

    ~CVATRegister(void) {
        for (auto & a : companyIds) {
            delete a.invoices;
        }
    }

    /**
     * @brief Add new identifiers (name & address, id) for new value
     *
     * Name and address need to be (together) unique, is case-insensitive
     * ID needs to be unique, is case-sensitive
     *
     * @param name Name value of identifier for new value
     * @param addr Address value of identifier for new value
     * @param taxID ID value of identifier for new value
     * @return true The value with identifiers has been created and stored
     * @return false Value with identifiers could not be created
     */
    bool newCompany(const string & name, const string & addr, const string & taxID) {
        //Create temporary objects
        CompanyID newId(taxID);
        CompanyContact newContact(name, addr);

        unsigned int * newInvoice = new unsigned int(0);
        newId.invoices = newInvoice;
        newContact.invoices = newInvoice;

        // Using binary search, it tries to find same values in the sorted vector (if found exact match, it is a duplicate and the new value cannot be created), else it finds a new position for the new value so that the vector remains sorted
        auto idBound = lower_bound(companyIds.begin(), companyIds.end(), newId);
        if (idBound != companyIds.end() && newId == (*idBound)) {
            delete newInvoice;
            return false;
        }

        // Using binary search, it tries to find same values in the sorted vector (if found exact match, it is a duplicate and the new value cannot be created), else it finds a new position for the new value so that the vector remains sorted
        auto contactBound = lower_bound(companyContacts.begin(), companyContacts.end(), newContact);
        if (contactBound != companyContacts.end() && newContact == (*contactBound)) {
            delete newInvoice;
            return false;
        }

        companyIds.insert(idBound, move(newId));
        companyContacts.insert(contactBound, move(newContact));

        return true;
    }

    /**
     * @brief Remove values based on identifiers
     *
     * @param name Name value of identifier
     * @param addr Address value of identifier
     * @return true The value with given identifiers was removed from memory
     * @return false Value with identifiers could not be removed
     */
    bool cancelCompany(const string & name, const string & addr) {

        try {
            auto contactBound = getContactIter(name, addr); //Finds value of CompanyContact identifier

            //Finds (and removes) the complementary CompanyID identifier according to the pointer to the value (and removes value from memory)
            for (auto it = companyIds.begin(); it != companyIds.end(); it++) {
                if ((*it).invoices == (*contactBound).invoices) {
                    delete (*contactBound).invoices;
                    companyIds.erase(it);
                    companyContacts.erase(contactBound);
                    return true;
                }
            }
        }
        catch (...) {
            return false;
        }

        return false;
    }

    /**
     * @brief Remove values based on identifier
     *
     * @param taxID ID value of identifier
     * @return true The value with given identifier was removed from memory
     * @return false Value with identifier could not be removed
     */
    bool cancelCompany(const string & taxID) {

        try {
            auto idBound = getIDIter(taxID); //Finds value of CompanyID identifier

            //Finds (and removes) the complementary CompanyContact identifier according to the pointer to the value (and removes value from memory)
            for (auto it = companyContacts.begin(); it != companyContacts.end(); it++) {
                if ((*it).invoices == (*idBound).invoices) {
                    delete (*idBound).invoices;
                    companyIds.erase(idBound);
                    companyContacts.erase(it);
                    return true;
                }
            }
        }
        catch (...) {
            return false;
        }

        return false;
    }

    /**
     * @brief Increase value pointed to by identifier
     *
     * @param taxID ID value of identifier
     * @param amount Amount by which to increase the value
     * @return true The value with given identifier was increased
     * @return false The value with given identifier could not be increased
     */
    bool invoice(const string & taxID, unsigned int amount) {
        try {
            *(getIDIter(taxID)->invoices) += amount;
        }
        catch (...) {
            return false;
        }

        addInvoiceToMedian(amount);
        return true;
    }

    /**
     * @brief Increase value pointed to by identifier
     *
     * @param name Name value of identifier
     * @param addr Address value of identifier
     * @param amount Amount by which to increase the value
     * @return true The value with given identifier was increased
     * @return false The value with given identifier could not be increased
     */
    bool invoice(const string & name, const string & addr, unsigned int amount) {
        try {
            *(getContactIter(name, addr)->invoices) += amount;
        }
        catch (...) {
            return false;
        }

        addInvoiceToMedian(amount);
        return true;
    }

    /**
     * @brief Get value pointed to by identifier
     *
     * @param[in] name Name value of identifier
     * @param[in] addr Address value of identifier
     * @param[out] sumIncome Value pointed to by identifier
     * @return true The value was obtained
     * @return false The value could not be obtained
     */
    bool audit(const string & name, const string & addr, unsigned int & sumIncome) const {
        try {
            sumIncome = *(getContactIter(name, addr)->invoices);
        }
        catch (...) {
            return false;
        }
        return true;
    }

    /**
     * @brief Get value pointed to by identifier
     *
     * @param[in] taxID ID value of identifier
     * @param[out] sumIncome Value pointed to by identifier
     * @return true The value was obtained
     * @return false The value could not be obtained
     */
    bool audit(const string & taxID, unsigned int & sumIncome) const {
        try {
            sumIncome = *(getIDIter(taxID)->invoices);
        }
        catch (...) {
            return false;
        }
        return true;
    }

    /**
     * @brief Get the values of alphabetically first identifier
     *
     * Arranged alphabetically first by name, in case of a match second sort parameter is address
     *
     * @param[out] name Name value of identifier
     * @param[out] addr Address value of identifier
     * @return true First identifier was obtained
     * @return false First identifier could not be obtained
     */
    bool firstCompany(string & name, string & addr) const {
        if (companyContacts.size() == 0) {
            return false;
        }

        name = companyContacts.front().name;
        addr = companyContacts.front().address;
        return true;
    }

    /**
     * @brief Get the values of alphabetically next identifier after specified identifier
     *
     * @param name Name value of identifier
     * @param addr Address value of identifier
     * @return true Next identifier was obtained
     * @return false Next identifier could not be obtained
     */
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

    /**
     * @brief Get the median of all inputted values
     *
     * @return unsigned int Median
     */
    unsigned int medianInvoice(void) const {
        //The median is calculated quickly thanks to splitting all values into two halves

        if (medianLowerHalf.size() == 0 && medianUpperHalf.size() == 0) {
            return 0;
        }

        //Get the largest/smallest value from the bigger half (or from the upper half if there has been even number of values inputted)
        if ((medianUpperHalf.size() > medianLowerHalf.size()) || (medianLowerHalf.size() == medianUpperHalf.size())) {
            return medianUpperHalf.top();
        } else {
            return medianLowerHalf.top();
        }
    }
};

#ifndef __PROGTEST__
int main(void) {
    string name, addr;
    unsigned int sumIncome;

    CVATRegister b1;
    assert(b1.newCompany("ACME", "Thakurova", "666/666"));
    assert(b1.newCompany("ACME", "Kolejni", "666/666/666"));
    assert(!b1.newCompany("aCME", "kOlejni", "999"));
    assert(!b1.newCompany("Ale", "Ne", "666/666"));
    assert(b1.newCompany("", "", ""));
    assert(b1.cancelCompany(""));
    assert(b1.newCompany("Dummy", "Thakurova", "123456"));
    assert(b1.invoice("666/666", 2000));
    assert(b1.medianInvoice() == 2000);
    assert(b1.invoice("666/666/666", 3000));
    assert(b1.medianInvoice() == 3000);
    assert(b1.invoice("123456", 4000));
    assert(b1.medianInvoice() == 3000);
    assert(b1.invoice("aCmE", "Kolejni", 5000));
    assert(b1.medianInvoice() == 4000);
    assert(b1.audit("ACME", "Kolejni", sumIncome) && sumIncome == 8000);
    assert(b1.audit("123456", sumIncome) && sumIncome == 4000);
    assert(b1.firstCompany(name, addr) && name == "ACME" && addr == "Kolejni");
    assert(b1.nextCompany(name, addr) && name == "ACME" && addr == "Thakurova");
    assert(b1.nextCompany(name, addr) && name == "Dummy" && addr == "Thakurova");
    assert(!b1.nextCompany(name, addr));
    assert(b1.cancelCompany("ACME", "KoLeJnI"));
    assert(b1.medianInvoice() == 4000);
    assert(b1.cancelCompany("666/666"));
    assert(b1.medianInvoice() == 4000);
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
    assert(b1.medianInvoice() == 2000);
    assert(b1.firstCompany(name, addr) && name == "Dummy" && addr == "Thakurova");
    assert(!b1.nextCompany(name, addr));
    assert(b1.cancelCompany("123456"));
    assert(!b1.firstCompany(name, addr));

    CVATRegister b2;
    assert(b2.newCompany("ACME", "Kolejni", "abcdef"));
    assert(b2.newCompany("Dummy", "Kolejni", "123456"));
    assert(!b2.newCompany("AcMe", "kOlEjNi", "1234"));
    assert(b2.newCompany("Dummy", "Thakurova", "ABCDEF"));
    assert(b2.medianInvoice() == 0);
    assert(b2.invoice("ABCDEF", 1000));
    assert(b2.medianInvoice() == 1000);
    assert(b2.invoice("abcdef", 2000));
    assert(b2.medianInvoice() == 2000);
    assert(b2.invoice("aCMe", "kOlEjNi", 3000));
    assert(b2.medianInvoice() == 2000);
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
    assert(b2.medianInvoice() == 2000);
    assert(!b2.cancelCompany("abcdef"));
    assert(b2.newCompany("ACME", "Kolejni", "abcdef"));
    assert(b2.cancelCompany("ACME", "Kolejni"));
    assert(!b2.cancelCompany("ACME", "Kolejni"));


    // Heap tests
    MaxHeap<int> lhMax;
    assert(lhMax.size() == 0);
    lhMax.push(1);
    assert(lhMax.size() == 1);
    assert(lhMax.top() == 1);
    assert(lhMax.pop() == 1);
    assert(lhMax.size() == 0);
    lhMax.push(10);
    lhMax.push(25);
    lhMax.push(5);
    lhMax.push(50);
    lhMax.push(25);
    lhMax.push(7);
    lhMax.push(9);
    assert(lhMax.size() == 7);
    assert(lhMax.top() == 50);
    lhMax.push(1000);
    lhMax.push(1000);
    lhMax.push(124);
    lhMax.push(145);
    lhMax.push(50);
    lhMax.push(347);
    lhMax.push(345);
    assert(lhMax.size() == 14);
    assert(lhMax.pop() == 1000);
    assert(lhMax.pop() == 1000);
    assert(lhMax.pop() == 347);

    MinHeap<int> lhMin;
    assert(lhMin.size() == 0);
    lhMin.push(1);
    assert(lhMin.size() == 1);
    assert(lhMin.top() == 1);
    assert(lhMin.pop() == 1);
    assert(lhMin.size() == 0);
    lhMin.push(10);
    lhMin.push(25);
    lhMin.push(5);
    lhMin.push(50);
    lhMin.push(25);
    lhMin.push(7);
    lhMin.push(9);
    assert(lhMin.size() == 7);
    assert(lhMin.top() == 5);
    lhMin.push(0);
    lhMin.push(0);
    lhMin.push(1);
    lhMin.push(4);
    lhMin.push(50);
    lhMin.push(10000);
    lhMin.push(564);
    assert(lhMin.size() == 14);
    assert(lhMin.pop() == 0);
    assert(lhMin.pop() == 0);
    assert(lhMin.pop() == 1);

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
