#include "Utilities/FileManagers/GameSettingsRecordsFileManager.h"

const std::array<std::string, 8> GameSettingsRecordsFileManager::keys = {
    "player_periodspeed>",
    "ghost_periodspeed>",
    "scatter_duration>",
    "chase_duration>",
    "frighten_duration>",
    "kill_duration>",
    "bonus_period>",
    "ghost_begin_period>"
};

GameSettingsRecordsFileManager::GameSettingsRecordsFileManager(const std::string & filepath, bool write) : FileManager(filepath, write) { }

GameSettingsRecordsFileManager::~GameSettingsRecordsFileManager() { }