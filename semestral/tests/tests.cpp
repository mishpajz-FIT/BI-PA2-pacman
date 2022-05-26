#include <assert.h>

#include "Structures/Transforms/Transform.h"
#include "Structures/Matrix.h"

void matrixTests() {
    Matrix<int> m1(10, 10);

    for (size_t x = 0; x < m1.getSizeX(); x++) {
        for (size_t y = 0; y < m1.getSizeY(); y++) {
            m1.at(x, y) = 0;
        }
    }

    assert(m1.getSizeX() == 10);
    assert(m1.getSizeY() == 10);

    assert(m1.at(5, 5) == 0);

    Matrix<int> m2(m1);

    m2.at(5, 5) = 1;
    assert(m2.getSizeX() == 10);
    assert(m1.at(5, 5) == 0);
    assert(m1.at(5, 5) == 1);

    m2 = m1;
    assert(m2.at(5, 5) == 0);

    m2.at(5, 5) = 10;

    assert(m2.at(5, 5) == 10);
    assert(m1.at(5, 5) == 0);
}

void transformTests() {
    Transform t1(Position(1, 1), Rotation(0));

    t1.moveBy(3);
    assert(t1.rotation == Rotation::Direction::up);
    assert(t1.position == Position(1, -2));

    Transform t2;
    t2 = t1;

    t2.rotation = t2.rotation.opposite();
    t2.moveBy(3);
    assert(t2.position.x == 1 && t2.position.y == 1);

}

int main(void) {
    matrixTests();
    transformTests();
}