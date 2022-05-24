#include <array>
#include <string>

#include "Utilities/FileManagers/GameSettingsRecordsFileLoader.h"

GameSettingsRecordsFileLoader::GameSettingsRecordsFileLoader(const std::string & filePath) : GameSettingsRecordsFileManager(filePath) { }

std::pair<GameSettings, GameRecords> GameSettingsRecordsFileLoader::loadSettingsAndRecords() {
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
    GameSettings loadedSettings(values[0], values[1], values[2], values[3], values[4], values[5], values[6], values[7]);

    GameRecords loadedRecords;
    std::string bufferMapName;
    unsigned int bufferDifficulty;
    unsigned int bufferScore;
    while (file >> bufferMapName >> bufferDifficulty >> bufferScore) {
        loadedRecords.addScore(bufferMapName, bufferDifficulty, bufferScore);
    }

    return std::make_pair(loadedSettings, loadedRecords);
}