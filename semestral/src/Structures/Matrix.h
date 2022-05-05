/****************************************************************
 * @file Matrix.h
 * @author Michal Dobes
 * @version 0.1
 * @date 2022-04-27
 * @brief Matrix container
 *
 * @copyright Copyright (c) 2022
 ****************************************************************/

#ifndef MATRIX_H
#define MATRIX_H

#include <stdexcept>

/**
 * @brief Matrix container
 *
 * Two dimensional array.
 * Size is not changeable after initialization (except for assigning and copying).
 * Capable of random access of elements, indexed from 0.
 *
 * Template paremeter T needs to have at least:
 *  - Default constructor (without explicit parameters)
 *  - Assignment operator
 *
 * @tparam T Type of data to be stored in matrix
 */
template <typename T>
class Matrix {
private:
    T * data; //< Array where data is stored (rows are stored lineary in sucession )

    size_t sizeX; //< Size in x dimension
    size_t sizeY; //< Size in y dimension

    /**
     * @brief Get index in data array for element at coordinates
     *
     * @exception std::out_of_range Index is not in Matrix::data array
     *
     * @param x Coordinate x
     * @param y Coordinate y
     * @return size_t Index of element in Matrix::data array
     */
    size_t getIndexFor(size_t x, size_t y) const {
        if (x >= sizeX || y >= sizeY) {
            throw std::out_of_range("matrix: index out of range");
        }

        return (x + (y * sizeX));
    }

public:

    /**
     * @brief Construct a new Matrix object
     *
     * @exception std::invalid_argument Size is not valid
     *
     * @param dimensionX Size in dimension x
     * @param dimensionY Size in dimension y
     */
    Matrix(size_t dimensionX, size_t dimensionY) : sizeX(dimensionX), sizeY(dimensionY) {
        if (dimensionX <= 0 || dimensionY <= 0) {
            throw std::invalid_argument("matrix: invalid size");
        }

        size_t totalSize = dimensionX * dimensionY;
        data = new T[totalSize];
    }

    /**
     * @brief Copy a Matrix object
     *
     * @param toCopy Matrix to copy
     */
    Matrix(const Matrix & toCopy) : sizeX(toCopy.sizeX), sizeY(toCopy.sizeY) {
        size_t totalSize = toCopy.sizeX * toCopy.sizeY;
        data = new T[totalSize];

        for (size_t i = 0; i < totalSize; i++) {
            data[i] = toCopy.data[i];
        }
    }

    /**
     * @brief Assign a Matrix object to this object
     *
     * Matrix is copyied and then assigned.
     *
     * @param toCopy Matrix object to assign
     * @return Matrix& This object
     */
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

    /**
     * @brief Destroy the Matrix object
     *
     */
    ~Matrix() {
        delete [] data;
        data = nullptr;
    }

    /**
     * @brief Get size in x dimension
     *
     * @return size_t Size in x dimension
     */
    size_t getSizeX() const {
        return sizeX;
    }

    /**
     * @brief Get size in y dimension
     *
     * @return size_t Size in y dimension
     */
    size_t getSizeY() const {
        return sizeY;
    }

    /**
     * @brief Get element at coordinates
     *
     * @exception std::out_of_range Coordinates are not in range
     *
     * @param x Coordinate x
     * @param y Coordinate y
     * @return T& Element
     */
    T & at(size_t x, size_t y) {
        return data[getIndexFor(x, y)];
    }

    const T & at(size_t x, size_t y) const {
        return data[getIndexFor(x, y)];
    }
};

#endif /* MATRIX_H */