#ifndef BOARDFILELOADER_H
#define BOARDFILELOADER_H

#include "FileLoader.h"
#include "Board.h"
#include "Transform.h"
#include "Matrix.h"

class BoardFileLoader : public FileLoader {
private:
    typedef Matrix<Board::Tile::Type> TileMatrix;

    Position playerSpawn;
    Position enemySpawn;


    bool checkForSpecialCharacter(char c, size_t x, size_t y);

    TileMatrix createTilesOutOfData(std::list<std::string> & lines);

    /**
    * @brief Imported file char to type conversion
    *
    * @exception BoardInvalidGrid char is unknown
    *
    * @param c char from file
    * @return Type Tile::Type the inputted char represents
    */
    static Board::Tile::Type dataCharToType(char c);

public:
    BoardFileLoader(const std::string & filePath);

    Board loadBoard();

};

#endif /* BOARDFILELOADER_H */