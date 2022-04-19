/**
 * @file vector.cpp
 * @author Michal Dobeš
 * @date 2022-03-31
 *
 * @brief Implementation of std-like Vector template
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <cstdlib>
#include <cassert>

using namespace std;

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

    void remove(size_t at) {
        if (at >= size) {
            throw "Remove behind last element of vector!";
        }

        for (size_t i = at; i + 1 < size; i++) {
            data[i] = data[i + 1];
        }
        size--;
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
    Vector<int> v2;
    v2.push(0);
    v2.push(1);
    v2.push(2);
    v2.push(3);
    v2.push(4);
    v2.push(5);
    v2.push(6);
    v2.remove(0);
    assert(v2.at(0) == 1);
    assert(v2.top() == 6);
    assert(v2.getSize() == 6);
    v2.remove(2);
    assert(v2.at(2) == 4);
    v2.remove(4);
    assert(v2.top() == 5);
    assert(v2.getSize() == 4);

}