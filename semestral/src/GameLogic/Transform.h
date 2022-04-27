#ifndef TRANSFORM_H
#define TRANSFORM_H

struct Position {
public:
    Position();

    Position(int newX, int newY);

    int x;
    int y;
};

enum class Rotation {
    up,
    down,
    right,
    left
};

struct Transform {
public:
    Position position;
    Rotation rotation;

    Transform(const Position & pos = Position(), const Rotation & rot = Rotation::up);
};


#endif /* TRANSFORM_H */