#ifndef MATRIX_H
#define MATRIX_H

#include <exception>

template <typename T>
class Matrix {
private:
    data * T;

    size_t sizeX;
    size_t sizeY;

public:

    Matrix(size_t dimensionX, size_t dimensionY);
    Matrix(const Matrix & toCopy);
    ~Matrix();
    Matrix & operator = (const Matrix & toCopy);

    size_t getSizeX() const;
    size_t getSizeY() const;

    const T & at(size_t x, size_t y) const;
    T & at(size_t x, size_t y);
};

template <typename T>
Matrix<T>::Matrix(size_t dimensionX, size_t dimensionY) : sizeX(dimensionX), sizeY(dimensionY) {
    if (dimensionX <= 0 || dimensionY <= 0) {
        throw std::invalid_argument("matrix: invalid size");
    }

    size_t totalSize = dimensionX * dimensionY;
    data = new T[totalSize];
}

template <typename T>
Matrix::Matrix(const Matrix & toCopy) {

}

#endif /* MATRIX_H */