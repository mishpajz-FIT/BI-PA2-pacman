#include <ncurses.h>
#include <optional>

#include "Structures/Transforms/Rotation.h"

struct GameControl {
    static std::optional<Rotation> getPlayerRotationFromKey(int c);
};