#ifndef POSITIONROTATION_H
#define POSITIONROTATION_H

struct Position {
public:
    Position();

    Position(int newX, int newY);

    int x;
    int y;
};

struct Rotation {
private:
    int direction;
public:
    Rotation();

    Rotation(int dir);
};


#endif /* POSITIONROTATION_H */