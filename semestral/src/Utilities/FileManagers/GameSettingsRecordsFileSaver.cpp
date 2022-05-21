#include "GameSettingsRecordsFileSaver.h"

GameSettingsRecordsFileSaver::GameSettingsRecordsFileSaver(const std::string & filePath) : GameSettingsRecordsFileManager(filePath, true) { }

void GameSettingsRecordsFileSaver::writeSettingsAndRecords(std::pair<GameSettings, GameRecords> data) {

    GameSettings settings = data.first;

    file << keys[0] << '\t' << settings.playerSpeed << '\n';
    file << keys[1] << '\t' << settings.enemySpeed << '\n';
    file << keys[2] << '\t' << settings.scatterDuration << '\n';
    file << keys[3] << '\t' << settings.chaseDuration << '\n';
    file << keys[4] << '\t' << settings.frightenDuration << '\n';
    file << keys[5] << '\t' << settings.killDuration << '\n';
    file << keys[6] << '\t' << settings.bonusPeriod << '\n';
    file << keys[7] << '\t' << settings.ghostComeOutPeriod << '\n';

    for (auto & r : data.second.records) {
        file << r.first.first << '\t' << r.first.second << '\t' << r.second << '\n';
    }

    if (!file.good()) {
        throw FileLoaderException("GameSettingsRecordsFileSaver: writeSettingsAndRecords - couldnt write");
    }
}