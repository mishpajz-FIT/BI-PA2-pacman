/****************************************************************
 * @file BoardFileLoader.h
 * @author Michal Dobes
 * @brief File loader for board
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 *****************************************************************/

#ifndef BOARDFILELOADER_H
#define BOARDFILELOADER_H

#include "Utilities/FileManagers/FileManager.h"
#include "GameLogic/Board.h"
#include "Structures/Transforms/Transform.h"
#include "Structures/Matrix.h"

/**
 * @brief File loader for Board
 *
 * Used for loading Board object from file.
 *
 */
class BoardFileLoader : public FileManager {
private:
    typedef Matrix<Board::Tile::Type> TileMatrix;

    Position playerSpawn; //< Loaded player spawn
    Position enemySpawn; //< Loaded enemy spawn

    /**
     * @brief Check if char is special tile and load it into position at x, y coords
     *
     * Special tiles are spawn positions marked by 'P' for player and 'E' for enemy
     *
     * @throws FileLoaderException duplicit player or enemy spawn
     *
     * @param c char to load
     * @param x x coord of position to load to
     * @param y y coord of position to load to
     * @return true char is special and was loaded
     * @return false char is not specal and wasn't loaded
     */
    bool checkForSpecialCharacter(char c, size_t x, size_t y);

    /**
     * @brief Create matrix of tiles from lines from file
     *
     * Also processes special characters using checkForSpecialCharacter
     *
     * @throws FileLoaderException missing player or enemy spawn
     *
     * @param lines lines from file
     * @return TileMatrix
     */
    TileMatrix createTilesOutOfData(std::list<std::string> & lines);

    /**
     * @brief Convert char from loaded file to Board::Tile::Type
     *
     * @param c char from file
     * @return Board::Tile::Type
     */
    static Board::Tile::Type dataCharToType(char c);

public:
    /**
     * @brief Construct a new Board File Loader object
     *
     * @throw FileLoaderException error utilizing file
     *
     * @param filePath path to settings file
     */
    BoardFileLoader(const std::string & filePath);

    /**
     * @brief Attempts to load Board from file
     *
     * @return Board
     */
    Board loadBoard();

};

#endif /* BOARDFILELOADER_H */