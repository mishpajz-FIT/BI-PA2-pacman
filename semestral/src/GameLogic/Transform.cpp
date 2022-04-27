#include "Transform.h"
#include <stdexcept>

Position::Position() : x(0), y(0) { }

Position::Position(int newX, int newY) : x(newX), y(newY) { }

Transform::Transform(const Position & pos = Position(), const Rotation & rot = Rotation::up) : position(pos), rotation(rot) { }
