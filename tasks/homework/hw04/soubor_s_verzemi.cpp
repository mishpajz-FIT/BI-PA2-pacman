/**
 * @file soubor_s_verzemi.cpp
 * @author Michal Dobeš
 * @date 2022-03-30
 *
 * @brief Binary file represented as class, with write, read and seek abilites. For faster copyiing supports copy-on-write on several levels.
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __PROGTEST__
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
using namespace std;
#endif /* __PROGTEST__ */

/**
 * @brief Data container
 *
 * Supports O(1) insertion and removal.
 * Does not have fast random access.
 *
 * @tparam T Datatype
 */
template <typename T>
class List {
    // Implemented as doubly linked list


    /**
     * @brief List Node which stores element of List
     *
     * Linked list node.
     *
     */
    struct Node {
        T value; // Data
        Node * next; // Next node
        Node * prev; // Previous node

        Node(T newValue) : value(newValue), next(nullptr), prev(nullptr) { }

        Node(const Node & toCopy) : value(toCopy.value), next(nullptr), prev(nullptr) { }
    };

    Node * head; // First Node
    Node * tail; // Last Node

    size_t size; // Nodes in this list (updated manually by insertion/deletion)

    /**
     * @brief Insert element before Node
     *
     * If pointer to Node is nullptr, inserts at end.
     *
     * @param before Pointer to Node before which to insert element
     * @param value Element which should be inserted
     */
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

    /**
     * @brief Remove Node
     *
     * @param at Pointer to Node which should be removed
     * @return T Element that was removed
     */
    T remove(Node * at) {
        T value(at->value); // Store value that will be removed

        if (at->prev != nullptr) {
            at->prev->next = at->next; // Set next of previous Node
        } else {
            head = at->next; // Set new head if removing current head
        }

        if (at->next != nullptr) {
            at->next->prev = at->prev; // Set previous of next Node
        } else {
            tail = at->prev; // Set new tail if removing current tail
        }

        delete at;
        size--;

        return value;
    }

public:
    /**
     * @brief Iterator for List
     *
     * Support forward/backward movement by one, element is accessed by dereferencing.
     * Can also deduce if at end of the list, however, if at end can not be moved forward nor backward.
     *
     */
    struct ListIterator {
        Node * ptr; // Pointer to Node which this Iterator represents

        ListIterator(const List & list, size_t position) : ptr(list.head) {
            for (size_t i = 0; i < position; i++) {
                ptr = ptr->next;
            }
        }

        ListIterator(Node * pointer = nullptr) : ptr(pointer) { }

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
            head = new Node(*(copyFrom.head)); // Copy head
            Node * iter = head;
            Node * iterCopy = copyFrom.head->next;
            while (iterCopy != nullptr) { // Iteratively copy all nodes
                iter->next = new Node(*(iterCopy)); // Set next and allocate new Node
                iter->next->prev = iter; // Set prev
                iter = iter->next; // Move iterators
                iterCopy = iterCopy->next;
            }
            tail = iter;
        }
    }

    ~List() {
        Node * iter = head;
        while (iter != nullptr) { // Iteratively dealloc nodes
            Node * tmp = iter->next;
            delete iter;
            iter = tmp;
        }
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    List & operator = (const List & toAssign) {
        if (this == &toAssign) {
            return *this;
        }

        // Copy list and swap all values
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

    /**
     * @brief Insert element before Iterator
     *
     * Moves Iterator to inserted element.
     *
     * @param before Iterator before which to insert element
     * @param value Element which should be inserted
     */
    void insert(ListIterator & before, const T & value) {
        insert(before.ptr, value);

        // Move iterator to inserted Node
        if (before.ptr == nullptr) {
            before.ptr = tail;
        } else {
            before.ptr = (before.ptr)->prev;
        }
    }

    /**
     * @brief Remove element selected by Iterator
     *
     * Moves Iterator to next element after removed element.
     *
     * @param at Iterator which element should be removed
     * @return T Element that was removed
     */
    T remove(ListIterator & at) {
        ListIterator old = at;
        at++;
        return remove(old.ptr);
    }

    /**
     * @brief Insert element at beggining of List
     *
     * @param value Element which should be inserted
     */
    void pushFront(const T & value) {
        insert(head, value);
    }

    /**
     * @brief Remove element at beggining of List
     *
     * @return T Element that was removed
     */
    T popFront() {
        return remove(head);
    }

    /**
     * @brief Insert element at end of List
     *
     * @param value Element which should be inserted
     */
    void pushBack(const T & value) {
        insert(nullptr, value);
    }

    /**
     * @brief Remove element at end of List
     *
     * @return T Element that was removed
     */
    T popBack() {
        return remove(tail);
    }

    /**
     * @brief Get last element in list
     *
     * @return T& Reference to last element
     */
    T & back() {
        return tail->value;
    }

    const T & back() const {
        return tail->value;
    }

    /**
     * @brief Get first element in list
     *
     * @return T& Reference to first element
     */
    T & front() {
        return head->value;
    }

    const T & front() const {
        return head->value;
    }

    /**
     * @brief Get iterator to beggining of list
     *
     * @return ListIterator Iterator
     */
    ListIterator begin() const {
        return ListIterator(head);
    }

    /**
     * @brief Get iterator to end of list
     *
     * Note this iterator points behind last element of list, can't be increased or decreased and does not point to any value.
     *
     * @return ListIterator Iterator
     */
    ListIterator end() const {
        return ListIterator(nullptr);
    }

    /**
     * @brief
     *
     * @param at
     * @return ListIterator
     */
    ListIterator iterator(size_t at = 0) const {
        return ListIterator(*this, at);
    }

    /**
     * @brief Amount of elements in List
     *
     * @return size_t Amount of elements
     */
    size_t getSize() const {
        return size;
    }
};

/**
 * @brief Representation of binary file.
 *
 * Supports operations as seek, write, read, truncate.
 * Has the ability of simple version control, with saving and reverting to versions.
 *
 * Prioritised for copy speed.
 *
 */
class CFile {
    /**
     * File contains List of Versions. This contains all Versions including current one (at back).
     * Each Version has Data. Data is reference counted and is copied on write.
     * Data contains List of Buckets. Bucket is reference counted and is copied on write. Each Bucket contains array of bytes.
     */

private:
    /**
     * @brief Bucket containing array of bytes
     *
     */
    struct Bucket {
        unsigned int refCount; // Reference counter for copy-on-write

        uint32_t size; // Size of array of bytes
        uint8_t * bytes; // Array of bytes

        void createBytesFrom(const uint8_t * fromBytes, uint32_t inSize) {
            bytes = new uint8_t[size];
            for (uint32_t i = 0; i < size; i++) {
                bytes[i] = fromBytes[i];
            }
        }

        Bucket(const uint8_t * fromBytes, uint32_t inSize) : refCount(0), size(inSize) {
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

    /**
     * @brief Container for List of Buckets
     *
     */
    struct Data {
        unsigned int refCount; // Reference counter for copy-on-write

        List<CFile::Bucket *> buckets; // List of Buckets
        uint32_t totalBytes; // Total bytes in every bucket (update at each write/delete)

        /**
         * @brief Update reference counter of Bucket in List of Buckets (buckets)
         *
         * If needed bucket is deallocated.
         *
         * @param bucket Pointer to bucket which should be released
         */
        void releaseBucket(CFile::Bucket * bucket) {
            if (--(bucket->refCount) == 0) {
                delete bucket;
            }
        }

        Data() : refCount(0), buckets(), totalBytes(0) { }

        Data(const Data & toCopy) : refCount(0), buckets(toCopy.buckets), totalBytes(toCopy.totalBytes) { // Copy buckets and update references
            auto iter = buckets.begin();
            while (!iter.isAtEnd()) {
                (*(iter++))->refCount++;
            }
        }

        ~Data() {
            auto iter = buckets.begin(); // Release all buckets
            while (!iter.isAtEnd()) {
                releaseBucket(*(iter++));
            }
        }

        Data & operator = (const Data & toAssign) {
            if (this == &toAssign) {
                return (*this);
            }

            auto iter = buckets.begin(); // Release all buckets
            while (!iter.isAtEnd()) {
                releaseBucket(*(iter++));
            }

            buckets = toAssign.buckets; // Copy buckets and update references
            iter = buckets.begin();
            while (!iter.isAtEnd()) {
                (*(iter++))->refCount++;
            }

            totalBytes = toAssign.totalBytes;

            return (*this);
        }

        /**
         * @brief Create new bucket and insert it before Iterator
         *
         * Iterator is moved to inserted bucket.
         *
         * @param to Iterator before which new bucket should be inserted
         * @param data Data with whom should new bucket be created
         * @param size Size of data
         */
        void insertNewBucket(List<CFile::Bucket *>::ListIterator & to, const uint8_t * data, uint32_t size) {
            Bucket * newBucket = new Bucket(data, size);
            newBucket->refCount = 1; // Update buckets reference count 
            buckets.insert(to, newBucket);
            totalBytes += size;
        }

        /**
         * @brief Remove bucket at Iterator
         *
         * Iterator is moved to next bucket.
         *
         * @param at Iterator of bucket which should be removed
         */
        void removeBucket(List<CFile::Bucket *>::ListIterator & at) {
            totalBytes -= ((*at)->size);
            releaseBucket(*at);
            buckets.remove(at);
        }
    };

    class Version {
        Data * data; // Data of version

        List<CFile::Bucket *>::ListIterator posInData; // List Iterator for List of Buckets in data
        uint32_t posInBucket; // Position in bucket pointed to by posInData

        uint32_t posAtByteInVersion; // Total position in data by bytes

        /**
         * @brief Update reference counter in data and dealloc if needed
         *
         */
        void releaseData() {
            if (--(data->refCount) == 0) {
                delete data;
            }
        }

        /**
         * @brief Copy data if needed
         *
         * Should be called before operation that edits data.
         * By using copy-on-write principle copy data if needed.
         *
         */
        void prepareEdit() {
            if ((*data).refCount > 1) { // If data is referenced by more versions, copy data and update reference count
                Data * newData = new Data(*data);
                (data->refCount)--;
                data = newData;
                data->refCount = 1;
                seek(posAtByteInVersion); // Update posInData Iterator to be in newly allocated data
            }
        }

    public:
        Version() : posInBucket(0), posAtByteInVersion(0) {
            data = new Data();
            data->refCount = 1;
            posInData = data->buckets.begin();
        }

        Version(const Version & toCopy) : posInData(toCopy.posInData), posInBucket(toCopy.posInBucket), posAtByteInVersion(toCopy.posAtByteInVersion) {
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

            posInData = toAssign.posInData;
            posInBucket = toAssign.posInBucket;
            posAtByteInVersion = toAssign.posAtByteInVersion;

            return(*this);
        }

        /**
         * @brief Move current position to byte
         *
         * Byte needs to be in range [0, data size in bytes]
         *
         * @param to Byte to which move position to
         * @return true Position is in range
         * @return false Position is out of range and was not moved
         */
        bool seek(uint32_t to) {
            if (to > data->totalBytes) {
                return false;
            }

            posInBucket = to;
            posInData = (data->buckets).begin();
            while (!posInData.isAtEnd()) {
                if ((posInBucket >= ((*posInData)->size))) {
                    posInBucket -= ((*(posInData)++)->size);
                } else {
                    break;
                }
            }
            posAtByteInVersion = to;
            return true;
        }

        /**
         * @brief Read bytes from data
         *
         * If requested amount of bytes is larger than bytes available, stops at end.
         *
         * @param dst Where should be read bytes copied to
         * @param bytes Amount of bytes to read
         * @return size_t Bytes read
         */
        size_t read(uint8_t * dst, uint32_t bytes) {
            if (posAtByteInVersion == data->totalBytes) { // If position is at end of file return 0
                return 0;
            }

            uint32_t i = 0;
            while (i < bytes) { // Copy bytes while possible and while requested
                dst[i++] = (*posInData)->bytes[posInBucket++]; // Copy byte
                posAtByteInVersion++; // Update current position in file
                if (posInBucket >= (*posInData)->size) { // If reached end of bucket in data, move to next bucket
                    posInBucket = 0;
                    posInData++;
                }

                if (posAtByteInVersion >= data->totalBytes) { // If reached end of file, return
                    break;
                }
            }
            return i;
        }

        /**
         * @brief Write bytes into file
         *
         * Writes from current position, bytes are overwritten or (if at end of file) appended
         *
         * @param src Source of bytes that should be written
         * @param bytes Size of src
         */
        void write(const uint8_t * src, uint32_t bytes) {
            prepareEdit(); // If needed copy data (copy-on-write principle)

            uint32_t bytesRemainingToOverwrite = bytes;
            if (posInBucket != 0) { // If position is in middle of bucket, create new bucket out of the part of bucket that should not be overwritten
                data->insertNewBucket(posInData, (*posInData)->bytes, posInBucket);
                posInData++;
            }

            if (!posInData.isAtEnd()) {
                while (bytesRemainingToOverwrite > 0) { // While there are bytes that should be overwritten
                    if (((*posInData)->size - posInBucket) > bytesRemainingToOverwrite) { // If current bucket is larger than bytes that remain to be overwriten
                        data->insertNewBucket(posInData, (*posInData)->bytes + posInBucket + bytesRemainingToOverwrite, ((*posInData)->size - (posInBucket + bytesRemainingToOverwrite))); // Create new bucket from part that should be kept. (posInBucket is included in case all overwritten data are in one bucket, thus data that were already copied cannot be copied again)
                        auto removeBucketAt = posInData; // After insertion of new bucket, old bucket is after new bucket, threfore the Iterator needs to be temporarily raised
                        data->removeBucket(++removeBucketAt); // Remove the old bucket
                        break;
                    } else { // If bucket is smaller than bytes that remain to overwriten, remove bucket
                        bytesRemainingToOverwrite -= ((*posInData)->size - posInBucket);
                        data->removeBucket(posInData);
                    }

                    posInBucket = 0;
                    if (posInData.isAtEnd()) {
                        break;
                    }
                }
            }

            data->insertNewBucket(posInData, src, bytes); // Create new bucket from bytes that should be written and insert it into data (if overwritten data ends in middle of bucket, new bucket is inserted before this bucket, else is at end)
            posInData++; // Update Iterators
            posInBucket = 0;
            posAtByteInVersion += bytes;
        }

        /**
         * @brief Get count of bytes that is stored in data
         *
         * @return uint32_t Stored bytes
         */
        uint32_t size() const {
            return data->totalBytes;
        }

        /**
         * @brief Remove all bytes after current position
         *
         */
        void truncate() {
            prepareEdit(); // If needed copy data (copy-on-write principle)

            if (posInBucket != 0) { // If position is in middle of bucket, create new bucket out of the part of bucket that should not be overwritten
                data->insertNewBucket(posInData, (*posInData)->bytes, posInBucket);
                posInData++;
            }
            posInBucket = 0;

            while (!posInData.isAtEnd()) { // Remove all other buckets in data
                data->removeBucket(posInData);
            }
        }
    };

    List<Version> versions; // List of all versions, current version is at back

public:
    CFile() {
        versions.pushBack(Version());
    }

    CFile(const CFile & toCopy) : versions(toCopy.versions) { }

    CFile & operator = (const CFile & toAssign) {
        if (this == &toAssign) {
            return (*this);
        }
        versions = toAssign.versions;
        return (*this);
    }

    /**
     * @brief Seek in current version
     *
     * Move current position to byte.
     * Byte needs to be in range [0, data size in bytes].
     *
     * @param offset Byte to which move position to
     * @return true Position is in range
     * @return false Position is out of range and was not moved
     */
    bool seek(uint32_t offset) {
        return versions.back().seek(offset);
    }

    /**
     * @brief Read bytes from current version
     *
     * Reads from current position.
     * If requested amount of bytes is larger than bytes available, stops at end.
     *
     * @param dst Where should be read bytes copied to
     * @param bytes Amount of bytes to read
     * @return uint32_t Bytes read
     */
    uint32_t read(uint8_t * dst, uint32_t bytes) {
        return versions.back().read(dst, bytes);
    }

    /**
     * @brief Write bytes into current version
     *
     * Writes from current position, bytes are overwritten or (if at end of file) appended.
     *
     * @param src Source of bytes that should be written
     * @param bytes Size of src
     * @return uint32_t Amount of bytes that were written
     */
    uint32_t write(const uint8_t * src, uint32_t bytes) {
        versions.back().write(src, bytes);
        return bytes;
    }

    /**
     * @brief Remove all bytes after current position in version
     *
     */
    void truncate(void) {
        versions.back().truncate();
    }

    /**
     * @brief Get count of bytes that is stored in current version
     *
     * @return uint32_t Stored bytes
     */
    uint32_t fileSize(void) const {
        return versions.back().size();
    }

    /**
     * @brief Archives the current version and sets its copy as the current version
     *
     */
    void addVersion(void) {
        versions.pushBack(versions.back());
    }

    /**
     * @brief Removes the current version and sets the previous version as the current version
     *
     * Available only if archived version exists.
     *
     * @return true Version was reverted
     * @return false Version could not be reverted
     */
    bool undoVersion(void) {
        if (versions.getSize() <= 1) {
            return false;
        }
        versions.popBack();
        return true;
    }
};

#ifndef __PROGTEST__
#include <iostream>

// @section Tests

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
}

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
    auto it2 = l2.begin(); //1, 3, 4
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

    CFile f001;
    assert(f001.fileSize() == 0);
    assert(!f001.seek(1));
    assert(f001.seek(0));
    assert(writeTest(f001, { 1, 2, 3 }, 3));
    assert(f001.fileSize() == 3);
    assert(writeTest(f001, { 4, 5, 6 }, 3));
    assert(f001.fileSize() == 6);
    assert(writeTest(f001, { 7, 8, 9, 10 }, 4));
    assert(f001.fileSize() == 10);
    assert(f001.seek(4));
    assert(readTest(f001, { 5, 6, 7, 8, 9, 10 }, 6));
    assert(f001.seek(0));
    assert(writeTest(f001, { 10, 20 }, 2));
    assert(f001.fileSize() == 10);
    assert(f001.seek(0));
    assert(readTest(f001, { 10, 20, 3, 4, 5, 6, 7, 8, 9, 10 }, 15));
    assert(f001.seek(3));
    assert(writeTest(f001, { 40, 50, 60 }, 3));
    assert(f001.seek(3));
    assert(readTest(f001, { 40, 50, 60 }, 3));
    assert(f001.fileSize() == 10);
    assert(f001.seek(0));
    assert(readTest(f001, { 10, 20, 3, 40, 50, 60, 7, 8, 9, 10 }, 15));
    assert(f001.seek(7));
    assert(writeTest(f001, { 80 }, 1));
    assert(f001.seek(0));
    assert(readTest(f001, { 10, 20, 3, 40, 50, 60, 7, 80, 9, 10 }, 15));
    assert(f001.seek(9));
    assert(writeTest(f001, { 100 }, 1));
    assert(writeTest(f001, { 110 }, 1));
    assert(f001.fileSize() == 11);
    assert(f001.seek(0));
    assert(readTest(f001, { 10, 20, 3, 40, 50, 60, 7, 80, 9, 100, 110 }, 15));
    assert(f001.seek(2));
    assert(writeTest(f001, { 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 13));
    assert(f001.fileSize() == 15);
    assert(f001.seek(0));
    assert(readTest(f001, { 10, 20, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 15));
    assert(!f001.seek(16));
    assert(f001.seek(15));
    assert(readTest(f001, { }, 100));
    assert(f001.seek(14));
    assert(readTest(f001, { 0 }, 100));

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

    CFile f002;
    f002 = f001;
    f002.seek(0);
    f002.truncate();
    assert(f001.fileSize() == 15);
    assert(f002.fileSize() == 0);
    f002.truncate();

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
