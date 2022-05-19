#include "GameSettingsFileLoader.h"
#include <array>
#include <string>

GameSettingsFileLoader::GameSettingsFileLoader(const std::string & filePath) : FileLoader(filePath) { }

GameSettings GameSettingsFileLoader::loadSettings() {
    std::array<std::string, 8> keys = {
        "player_speed>",
        "ghost_speed>",
        "scatter_duration>",
        "chase_duration>",
        "frighten_duration>",
        "kill_duration>",
        "bonus_period>",
        "ghost_begin_period>"
    };

    std::array<unsigned int, 8> values;

    unsigned int valueBuffer;
    std::string keyBuffer;
    for (size_t i = 0; i < 8; i++) {
        file >> keyBuffer >> valueBuffer;
        if (file.fail()
            || keys[i] != keyBuffer
            || valueBuffer == 0) {
            throw FileLoaderException("GameFileLoader: loadGame - wrong format of values in file");
        }
        values[i] = valueBuffer;
    }

    return GameSettings(values[0], values[1], values[2], values[3], values[4], values[5], values[6], values[7]);
}