#ifndef MATRIX_H
#define MATRIX_H

#include <exception>

template <typename T>
class Matrix {
private:
    T * data;

    size_t sizeX;
    size_t sizeY;

    size_t getIndexFor(size_t x, size_t y) {
        return (x + (y * sizeX));
    }

public:

    Matrix(size_t dimensionX, size_t dimensionY) : sizeX(dimensionX), sizeY(dimensionY) {
        if (dimensionX <= 0 || dimensionY <= 0) {
            throw std::invalid_argument("matrix: invalid size");
        }

        size_t totalSize = dimensionX * dimensionY;
        data = new T[totalSize];
    }

    Matrix(const Matrix & toCopy) : sizeX(toCopy.sizeX), sizeY(toCopy.sizeY) {
        size_t totalSize = toCopy.sizeX * toCopy.sizeY;
        data = new T[totalSize];

        for (size_t i = 0; i < totalSize; i++) {
            data[i] = toCopy.data[i];
        }
    }

    Matrix & operator = (const Matrix & toCopy) {
        if (this == &toCopy) {
            return (*this);
        }

        Matrix copiedMatrix(toCopy);
        std::swap(copiedMatrix.data, data);
        std::swap(copiedMatrix.sizeX, sizeX);
        std::swap(copiedMatrix.sizeY, sizeY);

        return (*this);
    }

    ~Matrix() {
        delete [] data;
        data = nullptr;
    }

    size_t getSizeX() const {
        return sizeX;
    }
    size_t getSizeY() const {
        return sizeY;
    }

    const T & at(size_t x, size_t y) const {
        return data[getIndexFor(x, y)];
    }

    T & at(size_t x, size_t y) {
        return data[getIndexFor(x, y)];
    }
};

#endif /* MATRIX_H */